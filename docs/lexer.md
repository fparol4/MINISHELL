# Lexer

**Entry point:** `lexer(const char *input)` → `t_manager *`  
**Free:** `lexer_free(t_manager *)`  
**Files:** `source/lexer/`

## What it does

The lexer turns a raw input string into a doubly-linked list of `t_list_token` nodes. Each token represents one semantic unit: a word, a pipe operator, or a redirection operator.

## Token types

| Enum | String that produces it | Meaning |
|---|---|---|
| `TOKEN_WORD` | any non-operator sequence | argument or filename |
| `TOKEN_PIPE` | `\|` | pipeline separator |
| `TOKEN_REDIR_IN` | `<` | redirect stdin |
| `TOKEN_REDIR_OUT` | `>` | redirect stdout (truncate) |
| `TOKEN_APPEND` | `>>` | redirect stdout (append) |
| `TOKEN_HEREDOC` | `<<` | heredoc delimiter follows |
| `TOKEN_NONE` | anything unrecognised | error placeholder |

## Token flags

Each `t_list_token` carries two boolean flags set at creation:

- `expand` — `true` if the raw value contains `$`. The runner uses this to skip expansion when there is no dollar sign, avoiding unnecessary processing.
- `quoted` — `true` if the raw value contains any quote character (`'` or `"`). The parser uses this to decide whether heredoc expansion should be disabled.

## Architecture

```
lexer()
  ├─ scanner_init()     point cursor at start of input
  ├─ lexer_init()       allocate t_manager + t_rules
  ├─ rules_init()       build two 256-entry lookup tables
  └─ state_machine()    consume input character by character
       ├─ extract_operator()   on | < >
       └─ extract_word()       on everything else (letters, symbols, quotes)
```

### Scanner (`source/lexer/scanner/`)

A lightweight cursor over the input string:

```c
typedef struct s_scanner {
    char   *input;   // original string (not owned)
    size_t  cursor;  // current position
    size_t  len;     // total length
    size_t  start;   // start of current token
} t_scanner;
```

Key operations:
- `scanner_advance()` — move cursor forward by one.
- `scanner_mark_start()` — record current position as token start.
- `scanner_extract()` — `ft_substr(input, start, cursor - start)` — copy the current span.
- `scanner_peek(sc, offset)` — look ahead without advancing.
- `scanner_until(sc, f)` — advance while predicate `f` is true.
- `scanner_is_end()` — cursor >= len.
- `scanner_current()` — byte value at cursor (cast to `size_t` for table indexing).

### Lookup tables (`source/lexer/scanner/table.c`)

Each `t_scanner_table` is an array of 256 `unsigned int`, one entry per byte value. Flags are ORed together so one array holds two layers of classification:

**Character-class flags** (low 8 bits, `t_table_char`):

| Flag | Bits | Characters |
|---|---|---|
| `P_ALPHA` | 1<<0 | a-z A-Z |
| `P_DIGIT` | 1<<1 | 0-9 |
| `P_SPACE` | 1<<2 | space, tab, CR, LF, FF, VT |
| `P_SQUOTE` | 1<<3 | `'` |
| `P_DQUOTE` | 1<<4 | `"` |
| `P_ESCAPE` | 1<<5 | `\` |
| `P_SYMBOL` | 1<<6 | anything else |

**Token-class flags** (bits 8+, `t_token_table`, offset 8):

| Flag | Characters |
|---|---|
| `L_WORD` | alphanumeric |
| `L_PIPE` | `\|` |
| `L_REDIR_IN` | `<` |
| `L_REDIR_OUT` | `>` |
| `L_DOLLAR` | `$` |
| `L_NONE` | everything else |

### Rules (`source/lexer/rules.c`)

`t_rules` bundles the table with two masks:

```c
rules->start_word     = L_WORD | P_DQUOTE | P_SQUOTE | P_SYMBOL;
rules->start_operator = L_PIPE | L_REDIR_IN | L_REDIR_OUT;
```

The state machine checks these masks against `table.props[current_char]` to decide which extraction path to take.

### State machine (`source/lexer/state.c`)

```
while not end:
    props = table[current_char]
    if props & start_operator  → extract_operator()
    if props & start_word      → extract_word()
    else                       → advance() (skip whitespace)
```

### Word extraction (`source/lexer/extract.c — extract_word`)

1. `scanner_mark_start()`.
2. Track quote state (`P_NONE`, `P_SQUOTE`, or `P_DQUOTE`).
3. Advance while not at a word-end: word-end is space or operator **and** quote state is `P_NONE`.
4. If quote state is still open at the end → return `NULL` (unclosed quote → `lexer()` emits `SNTX_UNCLOSED_QUOTE` and returns `NULL`).
5. `scanner_extract()` → `token_add()`.

Quotes do not split tokens: `he"llo"` is one `TOKEN_WORD` with value `he"llo"`. Quote removal happens later in the runner/expander.

### Operator extraction (`source/lexer/extract.c — extract_operator`)

1. `scanner_mark_start()`.
2. Advance while `props & start_operator` and count < `OPERATOR_MAX_LEN` (2).
3. `scanner_extract()` → string-compare to map `">"/"<"/"|"/">>/"<<"` to the correct enum via `operator_type()`.

### Token creation (`source/lexer/token.c — token_add`)

- `malloc(t_list_token)`.
- `ft_strdup(value)` — the token owns its value string.
- `expand = (ft_strchr(value, '$') != NULL)`.
- `quoted = sh_has_quotes(value)`.
- Append to `t_manager` doubly-linked list tail.

## Error handling

The only lexer error is an unclosed quote. When `extract_word` detects it, it returns `NULL`, `state_machine` returns `0`, and `lexer()`:
1. Calls `sh_stxerr(SNTX_UNCLOSED_QUOTE)` — prints to stderr.
2. Calls `lexer_cleanup()` — frees all tokens and the manager.
3. Returns `NULL`.

`core_process_line` treats a `NULL` token list as a status-2 error.

## Example

Input: `echo "hello $NAME" > out.txt`

| Token | Type | expand | quoted |
|---|---|---|---|
| `echo` | `TOKEN_WORD` | false | false |
| `"hello $NAME"` | `TOKEN_WORD` | true | true |
| `>` | `TOKEN_REDIR_OUT` | false | false |
| `out.txt` | `TOKEN_WORD` | false | false |
