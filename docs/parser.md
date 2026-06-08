# Parser

**Entry point:** `parser_controller(t_manager *manager)` → `t_ast *`  
**Free:** `parser_free_ast(t_ast *)`  
**Files:** `source/parser/`

## What it does

The parser consumes the token list produced by the lexer and builds an Abstract Syntax Tree (AST). The grammar it recognises is:

```
pipeline  ::= simple_command (PIPE simple_command)*
simple_command ::= (WORD | redir)*
redir     ::= (REDIR_IN | REDIR_OUT | APPEND | HEREDOC) WORD
```

The result is a `t_ast` containing either a root `t_command` node (on success) or an error flag with a `t_syntax_error` code.

## AST node types

```
t_command
  ├─ PNODE_CMD   → t_simple { t_array args; t_bool expand; t_array redirs; }
  └─ PNODE_PIPE  → t_pipe   { t_command *left; t_command *right; }
```

Pipes are **left-associative**: `a | b | c` builds:

```
PIPE
├─ PIPE
│   ├─ CMD(a)
│   └─ CMD(b)
└─ CMD(c)
```

`t_simple.args` is a `t_array` of `char *` (NULL-terminated).  
`t_simple.redirs` is a `t_array` of `t_parser_redir`:

```c
typedef struct s_parser_redir {
    t_parser_redir_type  type;    // REDIR_IN / REDIR_OUT / REDIR_APPEND / REDIR_HEREDOC
    char                *file;   // raw filename or heredoc delimiter (owned)
    t_bool               expand; // true when target should be variable-expanded
    t_bool               quoted; // true if original token had quotes
} t_parser_redir;
```

`redir.expand` is set to `!token->quoted`. A quoted heredoc delimiter (`<< 'EOF'`) disables expansion inside the heredoc body.

## Parser state (`t_parser`)

```c
typedef struct s_parser {
    t_node       *current;      // current node in the token list
    t_list_token *token;        // convenience pointer to current->content
    t_token_type  last_type;    // type of previous token
    unsigned int  state;        // TRUE = error encountered
    t_simple     *current_cmd;  // command being built
    t_ast        *ast;          // output AST (to store error_type)
} t_parser;
```

`parser.state` is a sticky flag: once set to `TRUE` it is never cleared. All parser functions check it at entry.

## Token classification

`parser_get_class(t_token_type)` maps tokens to three classes:

| Class | Tokens |
|---|---|
| `C_WORD` | `TOKEN_WORD` |
| `C_REDIR` | `TOKEN_REDIR_IN`, `TOKEN_REDIR_OUT`, `TOKEN_APPEND`, `TOKEN_HEREDOC` |
| `C_PIPE` | `TOKEN_PIPE` |
| `C_NONE` | anything else |

## Parsing flow

```
parser_controller()
  ├─ empty token list  → return NULL (empty input)
  ├─ first token is PIPE → SNTX_PIPE_START error
  └─ build_pipeline()
       ├─ parse_simple_command()   ← left side
       └─ while TOKEN_PIPE:
            ├─ parser_next()       ← consume pipe
            ├─ end-of-tokens       → SNTX_PIPE_END error
            ├─ parse_simple_command()   ← right side
            └─ build_pipe_node(left, right) → new left
```

### `parse_simple_command`

```
parser_start_command()   ← allocate t_simple, init args/redirs arrays
while token != PIPE:
    C_WORD  → consume_word()   add to args array, advance
    C_REDIR → consume_redir()  read next token as filename, add to redirs array
    other   → SNTX_EMPTY_CMD  (e.g. two pipes in a row)
parser_finish_command()  ← validate non-empty, detach t_simple, wrap in t_command
```

`consume_redir` reads one more token and checks it is a `C_WORD`; if not → `SNTX_REDIR_NO_TARGET`.

`parser_finish_command` also catches the case where both `args` and `redirs` are empty (`SNTX_EMPTY_CMD`). A command with only redirections and no words is valid — e.g., `< in > out` — so empty args alone is not an error.

## Syntax errors

| Code | Trigger | Example |
|---|---|---|
| `SNTX_UNCLOSED_QUOTE` | lexer returned NULL | `echo "hello` |
| `SNTX_PIPE_START` | first token is `\|` | `\| echo` |
| `SNTX_PIPE_END` | `\|` is last token | `echo \|` |
| `SNTX_EMPTY_CMD` | non-word/redir token after pipe | `echo \|\| echo` |
| `SNTX_REDIR_NO_TARGET` | redir not followed by a word | `echo >` or `> \|` |

All errors are stored in `ast->error_type` and `ast->error = 1`. The caller (core) prints the message via `sh_stxerr()` and sets exit status 2.

## Memory management

- `parser_start_command` allocates `t_simple` + two `t_array` items.
- Each call to `parser_add_arg` duplicates the string with `ft_strdup`.
- Each call to `parser_add_redir` duplicates `token->value`.
- On error, `parser_finish_command` calls `parser_free_simple()`.
- `parser_free_ast()` walks the tree recursively: pipe nodes recurse, CMD nodes free their `t_simple` arrays.
- `parser_free_cmd()` is the recursive free for individual nodes.
