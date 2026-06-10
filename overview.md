# Minishell — A Complete, Human Overview

> A small Unix shell written in C for the 42 curriculum.
> This document is meant to be **read top to bottom** and leave you understanding
> the whole project: every file, every responsibility, the obscure corners
> (signals, redirections, heredocs, pipelines), and finally the complete flow
> from a keystroke at the prompt to a process exiting.
>
> Authors of the shell: *fcardozo* & *g-alves-*.

---

## Table of Contents

1. [The big picture](#1-the-big-picture)
2. [Directory structure](#2-directory-structure)
3. [The data that flows through the shell](#3-the-data-that-flows-through-the-shell)
4. [`shared/` — the common toolbox](#4-shared--the-common-toolbox)
5. [`core/` — the heart that beats (REPL)](#5-core--the-heart-that-beats-repl)
6. [`env/` — the environment as a linked list](#6-env--the-environment-as-a-linked-list)
7. [`lexer/` — turning text into tokens](#7-lexer--turning-text-into-tokens)
8. [`parser/` — turning tokens into a command tree](#8-parser--turning-tokens-into-a-command-tree)
9. [`runner/` — making things actually happen](#9-runner--making-things-actually-happen)
   - 9.1 [The expander](#91-the-expander--word-splitting-and-variables)
   - 9.2 [Redirections](#92-redirections--the-careful-art-of-moving-fds)
   - 9.3 [Heredocs](#93-heredocs--the-most-subtle-redirection)
   - 9.4 [External commands & PATH](#94-external-commands--path-resolution)
   - 9.5 [Pipelines](#95-pipelines--many-processes-one-data-stream)
10. [`bin/` — the builtins](#10-bin--the-builtins)
11. [Signals — the deep dive](#11-signals--the-deep-dive)
12. [The complete flow, end to end](#12-the-complete-flow-end-to-end)

---

## 1. The big picture

A shell is a **Read–Eval–Print Loop** (REPL). It does four things forever:

```
   read a line  ──▶  understand it  ──▶  execute it  ──▶  remember the result
        ▲                                                        │
        └────────────────────────────────────────────────────────┘
```

Minishell breaks "understand it" and "execute it" into a classic compiler-style
pipeline. A line of text passes through four stages, each one handing a richer,
more structured object to the next:

```
  "cat file | grep foo > out"
        │
        ▼
  ┌──────────┐   list of tokens   ┌──────────┐   command tree   ┌──────────┐
  │  LEXER   │ ─────────────────▶ │  PARSER  │ ───────────────▶ │  RUNNER  │
  └──────────┘                    └──────────┘                  └──────────┘
   splits text                     groups tokens                 expands vars,
   into words &                    into commands,                opens fds,
   operators                       redirs & pipes                forks, execs
```

- **Lexer** answers: *"where does each word/operator begin and end?"*
- **Parser** answers: *"which words belong to which command, and how are they wired together?"*
- **Runner** answers: *"what do I actually do to make this happen on the OS?"*

The `core/` module is the loop that drives all of this, and `shared/`, `env/`,
and `bin/` are supporting cast used by everyone.

---

## 2. Directory structure

```
minishell/
├── Makefile               Build rules. Compiles libft, then the shell.
├── README.md              Project description & feature list.
├── headers/               PUBLIC headers — the contract between modules.
│   ├── minishell.h          Umbrella header: includes everything + system libs.
│   ├── core.h               t_shell, signal modes, shell status codes.
│   ├── env.h                t_env linked list + its API.
│   ├── lexer.h              t_list_token, token types, lexer() entry point.
│   ├── parser.h             t_command tree, t_ast, syntax errors, redir types.
│   ├── runner.h             rn_execute() entry point + exit-code constants.
│   ├── bin.h                The 7 builtin prototypes.
│   └── shared.h             Cross-cutting helpers (errors, quotes, signals).
│
├── libraries/libft/       The student's own libc (strings, lists, arrays...).
│
└── source/
    ├── main.c             Entry point. Calls core_run(envp).
    │
    ├── core/              ── THE REPL ──
    │   ├── core.c           init / run / destroy the shell.
    │   ├── reader.c         the loop: readline → parse → execute.
    │   └── _core.h          private header for core.
    │
    ├── shared/            ── COMMON TOOLBOX (prefix sh_) ──
    │   ├── signal.c         the global signal handlers + mode switching.
    │   ├── error.c          uniform "minishell: scope: msg" error printing.
    │   ├── quote.c          quote detection + quote removal.
    │   ├── string.c         tiny char predicates (isspace, varchar...).
    │   └── free.c           free a NULL-terminated array of pointers.
    │
    ├── env/               ── ENVIRONMENT (prefix env_) ──
    │   ├── init.c           build the t_env list from envp (+ SHLVL bump).
    │   ├── list.c           node new/free.
    │   ├── get.c set.c unset.c   lookup / upsert / remove by key.
    │   ├── size.c           count nodes.
    │   ├── toarr.c          flatten list back to char** for execve.
    │   ├── free.c           free the whole list.
    │   └── _env.h           private header.
    │
    ├── lexer/             ── TOKENIZER ──
    │   ├── lexer.c          orchestrates a scan of the input string.
    │   ├── state.c          the main loop: word? operator? skip?
    │   ├── extract.c        pull out one word / one operator.
    │   ├── rules.c          which character classes start words/operators.
    │   ├── token.c          create / free a token, append to the list.
    │   ├── _lexer.h         private header (t_rules).
    │   └── scanner/         a cursor over the raw string:
    │       ├── load.c         init the cursor.
    │       ├── move.c         advance / mark / is-end.
    │       ├── get.c          peek / current char / extract substring.
    │       ├── table.c        build the 256-entry character class tables.
    │       └── _scanner.h     P_* (char classes) and L_* (token classes) bits.
    │
    ├── parser/            ── GRAMMAR ──
    │   ├── parser.c         top level: build the pipeline (left-assoc tree).
    │   ├── command.c        parse one simple command (words + redirs).
    │   ├── state.c          start/finish a command, append arg/redir.
    │   ├── utils.c          token cursor + token→class classification.
    │   ├── errors.c         record a syntax error on the parser/ast.
    │   ├── cleanup.c        free the command tree / ast.
    │   └── _parser.h        private header (t_parser, t_token_class).
    │
    ├── runner/            ── EXECUTION (prefix rn_) ──
    │   ├── control.c        the dispatcher: cmd vs pipe; the redir+run dance.
    │   ├── builtin.c        is args[0] a builtin? if so, call it.
    │   ├── status.c         store/read $? in the environment.
    │   ├── _runner.h        private header + exit-code constants.
    │   ├── expander/        $VAR expansion, quote removal, word splitting.
    │   ├── redir/           <, >, >>, << — opening fds and swapping stdio.
    │   ├── external/        resolve + fork + execve an external program.
    │   ├── path/            search $PATH for an executable.
    │   └── pipe/            build and run a pipeline of N processes.
    │
    └── bin/               ── BUILTINS ──
        ├── echo.c cd.c pwd.c exit.c env.c unset.c
        ├── export/          export.c keys.c print.c (sorted, bash-style output).
        └── _bin.h
```

A naming convention runs through the whole project, and it helps a lot:

| Prefix  | Module    | Meaning                                  |
|---------|-----------|------------------------------------------|
| `sh_`   | shared    | shared helper usable anywhere            |
| `env_`  | env       | environment list operation               |
| `rn_`   | runner    | something happening at execution time    |
| `exp_`  | expander  | a piece of the variable/word expander    |
| `bin_`  | bin       | a builtin command                        |
| `core_` | core      | the REPL machinery                       |
| `scanner_` | lexer  | the raw-string cursor                    |
| `parser_` | parser  | parser state machine                     |

---

## 3. The data that flows through the shell

Before reading the modules, hold these four shapes in your head. Everything is a
transformation between them.

**1. Raw input** — just a `char *` from readline.

**2. Token list** (`t_list_token` in a doubly linked list). Each token:
```c
typedef struct s_list_token {
    char         *value;   // the literal text, e.g. "cat" or ">"
    t_token_type  type;    // TOKEN_WORD, TOKEN_PIPE, TOKEN_REDIR_IN, ...
    t_bool        expand;  // does it contain a '$'? (a hint for later)
    t_bool        quoted;  // does it contain a quote character?
}   t_list_token;
```

**3. Command tree** (`t_command`). This is a small **binary tree**. A node is
either a *simple command* (a leaf) or a *pipe* (an internal node):
```c
typedef struct s_command {
    t_pnode_type type;              // PNODE_CMD or PNODE_PIPE
    union {
        t_simple simple;            // leaf: args[] + redirs[]
        t_pipe   pipe;              // node: left + right children
    } t_define;
}   t_command;
```
`a | b | c` becomes (left-associative):
```
            PIPE
           /    \
         PIPE    c
        /    \
       a      b
```
A `t_simple` holds two dynamic arrays: `args` (the words) and `redirs` (the
redirections attached to that command).

**4. The whole parse result** (`t_ast`) wraps the tree's `root`, plus an
`error` flag and an `error_type` so the caller knows *why* parsing failed.

---

## 4. `shared/` — the common toolbox

Small, dependency-light helpers used everywhere. Worth knowing because their
names appear constantly.

**`string.c`** — character predicates:
- `sh_isspace(c)` — space, tab, or newline.
- `sh_varstart(c)` — may a variable name *begin* with this char? (`[A-Za-z_]`)
- `sh_varchar(c)` — may a variable name *contain* this char? (`[A-Za-z0-9_]`)
- `sh_isidentifier(s)` — is the whole string a valid shell identifier? (used by
  `export`/`unset` to reject things like `1foo` or `a-b`).

**`quote.c`** — quoting logic:
- `sh_has_quotes(s)` — does the string contain `'` or `"`?
- `sh_quote_remove(s)` — walk the string with a tiny state machine
  (`quote_should_copy`) that toggles an "inside which quote" flag and copies only
  the characters that are *not* the active quote delimiters. Turns `"a'b'"` into
  `ab`. Used for heredoc delimiters (where `$` is not expanded but quotes still
  vanish).

**`error.c`** — uniform error reporting on stderr:
- `sh_err(scope, msg)`   → `minishell: scope: msg`
- `sh_err2(scope, arg, msg)` → `minishell: scope: arg: msg`
- `sh_stxerr(type)`      → prints the right "syntax error: ..." line.

**`free.c`** — `sh_freesec` / `sh_freeargs` free a `NULL`-terminated `char **`
(each element, then the array). The `args` and `envp` arrays are freed with this
everywhere.

**`signal.c`** — covered in depth in [§11](#11-signals--the-deep-dive). It owns
the one global, `g_signal`, and the function `sh_sig_mode()` that reconfigures
the handlers for the three situations the shell can be in.

---

## 5. `core/` — the heart that beats (REPL)

`core/` is the loop. It is intentionally thin: read a line, hand it to the
pipeline, repeat.

### `t_shell` (in `core.h`)
```c
typedef struct s_shell {
    t_env  *env;       // the environment, owned here for the whole session
    t_bool  running;   // loop condition
}   t_shell;
```

### `core.c` — lifecycle
- **`core_init`** — zero `g_signal`, build the environment from `envp`
  (`env_init`), seed `$?` to `"0"` (stored under the key `"$?"`), set
  `running = TRUE`, and put signals into **interactive** mode. If anything fails
  it tears down cleanly.
- **`core_destroy`** — free the environment and clear readline's history.
- **`core_run`** — the public entry from `main`: init → loop → destroy, returning
  the final exit status.

### `reader.c` — the loop itself
This is the most important file in `core/`. Read it as a funnel:

- **`core_loop`** — saves the terminal settings with `tcgetattr`, then loops
  `core_iteration` while `running`. After each command it restores the terminal
  with `tcsetattr` (defensive: a misbehaving child could leave the terminal in a
  weird state). Returns the last `$?` when the loop ends.

- **`core_iteration`** — one turn of the REPL:
  1. `readline(PROMPT)` prints the prompt and blocks for a line.
  2. If it returns `NULL`, that's **EOF** (Ctrl-D) → `core_eof`.
  3. If a `SIGINT` arrived while waiting, set `$?` to `130`.
  4. Reset `g_signal` to 0.
  5. If the line is non-empty: `add_history` then `core_process_line`.
  6. `free(line)`.
  7. If a builtin requested exit (`g_signal == SH_EXIT_REQUESTED`), return `-1`
     to stop the loop.

- **`core_eof`** — Ctrl-D handling. If we got here *because* of a SIGINT, just
  reset and continue; otherwise print `exit\n` and signal the loop to stop. This
  mirrors bash printing `exit` when you press Ctrl-D at an empty prompt.

- **`core_process_line`** → **`core_parse`** — the bridge into the pipeline:
  ```c
  manager = lexer(line);              // text   → token list
  ast     = parser_controller(manager); // tokens → command tree
  lexer_free(manager);                // tokens no longer needed
  ```
  Then:
  - lexer failure (e.g. unclosed quote) → set `$?` = 2, skip.
  - parser syntax error → print the specific error, set `$?` = 2, free, skip.
  - otherwise → `rn_execute(ast->root, &env, STDIN_FILENO, ast)` and free the ast.

  Notice `STDIN_FILENO` is passed as the `heredoc_fd`: at the top level, a
  heredoc reads from the real terminal. (Inside a pipeline this argument becomes
  a *saved copy* of the terminal — see [§9.5](#95-pipelines--many-processes-one-data-stream).)

---

## 6. `env/` — the environment as a linked list

The environment is a singly linked list of key/value pairs (`t_env`). A list
(not an array) makes insert/remove trivial, which `export`/`unset` need.

- **`init.c`**
  - `env_init(envp)` — turn the `char **envp` into the list. `entryparse`
    splits each `"KEY=VALUE"` at the first `=`. An entry with no `=` becomes a
    key with a `NULL` value (an *exported but unset* variable, exactly like bash).
  - `shlvl()` — find `SHLVL` and increment it (or set it to 1), so nested shells
    count up like the real thing.
  - `minenv()` — if the process was started with *no* environment at all, build a
    minimal one with `PWD` and `SHLVL=1` so the shell still works.
- **`list.c`** — `env_node_new` (dup key, dup value if present) and
  `env_node_free`.
- **`get.c`** — `env_get` returns a *pointer into* the node's value (not a copy);
  callers must not free it.
- **`set.c`** — `env_set` is an **upsert**: if the key exists, replace its value;
  otherwise append a new node. A `NULL` value means "keep key, no value".
- **`unset.c`** — `env_unset` unlinks and frees the matching node.
- **`size.c`** — counts nodes.
- **`toarr.c`** — `env_toarr` rebuilds a `char **` (`"KEY=VALUE"`) for `execve`.
  Crucially it **skips** the internal `$?` pseudo-variable and any key with a
  `NULL` value (unset-but-exported vars are not passed to children — bash agrees).

**A neat trick:** the shell stores the last exit status `$?` *inside the
environment* under the key `"$?"` (the constant `ENV_ERRCODE`). That's why
`rn_status_set`/`rn_status_get` (in the runner) are just thin wrappers over
`env_set`/`env_get`. Because `"$?"` is not a valid identifier, the user can never
collide with it, and `env`/`export`/`execve` all filter it out.

---

## 7. `lexer/` — turning text into tokens

**Goal:** read the raw line and produce a flat list of tokens (words and
operators), respecting quotes and rejecting unclosed quotes.

### The scanner — a cursor with a character-class table

The lexer never inspects characters with `if (c == ...)` scattered around.
Instead it precomputes, once, a **256-entry table** that classifies every byte.
This is the cleanest idea in the lexer, so let's unpack it.

`scanner/_scanner.h` defines two sets of bit flags packed into one `unsigned int`
per character:

```
 P_* (char nature, low bits)          L_* (token role, high bits, OFFSET=8)
 ──────────────────────────           ──────────────────────────────────
 P_ALPHA   letter                     L_WORD     starts/continues a word
 P_DIGIT   digit                      L_PIPE     '|'
 P_SPACE   whitespace                 L_REDIR_IN '<'
 P_SQUOTE  '                          L_REDIR_OUT'>'
 P_DQUOTE  "                          L_DOLLAR   '$'
 P_ESCAPE  backslash                  L_NONE     anything else
 P_SYMBOL  any other symbol
```

`scanner_table_char` fills the `P_*` bits; `scanner_table_token` fills the `L_*`
bits. Because the two families live in different bit ranges, a single `props[c]`
value carries both "what kind of character is this" and "what token role does it
play". Classification is then a single array lookup — fast and branch-free.

The cursor (`t_scanner`) is just `{ input, cursor, len, start }`:
- `scanner_current` — the byte at the cursor (as `unsigned char`, 0 at end).
- `scanner_advance` — move forward one.
- `scanner_peek(offset)` — look ahead without moving.
- `scanner_mark_start` — remember where the current token began.
- `scanner_extract` — `ft_substr` from `start` to `cursor` (the token's text).
- `scanner_is_end` — cursor past the end?

### `rules.c` — what starts a word vs an operator

`t_rules` bundles the table plus two masks:
- `start_word` = `L_WORD | P_DQUOTE | P_SQUOTE | P_SYMBOL`
  → letters, digits, quotes, and stray symbols all begin a *word*.
- `start_operator` = `L_PIPE | L_REDIR_IN | L_REDIR_OUT`
  → `|`, `<`, `>` begin an *operator*.

### `state.c` — the dispatch loop

`state_machine` walks the input. For each position it reads `props` and decides:

```
 props & start_operator ? ──▶ extract_operator
 props & start_word     ? ──▶ extract_word   (may fail → unclosed quote)
 otherwise                ──▶ skip it (whitespace, etc.)
```

It returns 0 only on an unclosed quote, which the caller turns into a syntax
error.

### `extract.c` — pulling out one token

- **`extract_word`** consumes characters until it hits, *while not inside a
  quote*, either whitespace or an operator char. The subtlety is the quote
  state machine `quote_update`: entering `'` or `"` sets the state; the matching
  closer clears it. While inside quotes, spaces and `|`/`<`/`>` are ordinary
  characters and do **not** end the word. So `"a | b"` is **one** word, but
  `a | b` is three tokens. If the word ends while still inside a quote
  (`state != P_NONE`), it returns `NULL` → **unclosed quote** error. Note the
  word keeps its quotes for now; they are stripped later by the expander.

- **`extract_operator`** consumes up to `OPERATOR_MAX_LEN` (2) operator chars, so
  `>>`, `<<` are read as one token, while `>`, `<`, `|` are single-char tokens.
  `operator_type` then maps the literal text to the precise `TOKEN_*`:
  `>>`→`TOKEN_APPEND`, `<<`→`TOKEN_HEREDOC`, `|`→`TOKEN_PIPE`, etc.

### `token.c` — building the list

`token_add` allocates a `t_list_token`, dups the value, records `type`, sets the
two hints (`expand` = "contains `$`", `quoted` = "contains a quote"), wraps it in
a doubly linked list node, and appends it to the manager (`ft_dlist_add_tail`).

### `lexer.c` — orchestration & ownership

`lexer()` ties it together: init the scanner, allocate the list (`t_manager`) and
the rules, build the tables, run the state machine. On unclosed quote it prints
the error and frees everything (`lexer_cleanup`). On success it frees only the
rules and returns the token list. `lexer_free` walks the list freeing each token
and node.

> **Note on `t_manager` / `t_node`:** these are typedefs (in `lexer.h`) for the
> libft doubly linked list `t_dlist` / `t_dlist_node`. The token list *is* a
> generic linked list whose `content` is a `t_list_token *`.

---

## 8. `parser/` — turning tokens into a command tree

**Goal:** consume the flat token list and produce a `t_ast` whose `root` is the
command tree — or a syntax error explaining what's wrong.

### The parser cursor (`utils.c`)

`t_parser` holds the position in the token list plus the command being built:
```c
typedef struct s_parser {
    t_node       *current;     // current list node
    t_list_token *token;       // its token (or NULL at end)
    t_token_type  last_type;   // previous token's type
    unsigned int  state;       // non-zero == "we hit an error, bail out"
    t_simple     *current_cmd; // the command currently being assembled
    t_ast        *ast;         // where to record error_type
}   t_parser;
```
- `parser_init` points the cursor at the list head.
- `parser_next` advances, remembering `last_type`.
- `parser_is_end` — are we out of tokens?
- `parser_get_class` collapses the seven token types into three **classes**:
  `C_WORD`, `C_REDIR` (any of `<`, `>`, `>>`, `<<`), or `C_PIPE`. The parser
  reasons in classes, which keeps the grammar code short.

### Grammar, top down (`parser.c`)

The grammar is tiny:
```
pipeline       := simple_command ( '|' simple_command )*
simple_command := ( WORD | redirection )+
redirection    := ( '<' | '>' | '>>' | '<<' ) WORD
```

- **`parser_controller`** — allocate the `t_ast`. If the very first token is a
  `|`, that's `SNTX_PIPE_START`. Otherwise call `build_pipeline` and mark
  `ast->error` if it comes back `NULL`.
- **`build_pipeline`** — parse one simple command as `left`. While the next token
  is a `|`: step over it, ensure something follows (else `SNTX_PIPE_END`), parse
  the right-hand command, and fold them into a `PNODE_PIPE` node via
  `build_pipe_node`. This builds the **left-associative** tree shown in §3.

### Parsing one command (`command.c`)

- **`parse_simple_command`** — `parser_start_command` allocates a fresh
  `t_simple` (two empty dynamic arrays). Then loop until a `|` or end, handling
  each token with `parse_simple_token`. Finally `parser_finish_command` and wrap
  the `t_simple` into a `t_command` (`simple_to_command`).
- **`parse_simple_token`** dispatches by class:
  - `C_WORD` → `consume_word`: dup the word into the command's `args` array.
  - `C_REDIR` → `consume_redir`.
  - anything else here → `SNTX_EMPTY_CMD`.
- **`consume_redir`** is where redirection *syntax* is checked. It computes the
  redir type by arithmetic on the enum:
  `type = token->type - TOKEN_REDIR_IN` — because `parser.h` orders
  `REDIR_IN, REDIR_OUT, REDIR_APPEND, REDIR_HEREDOC` to line up with the token
  enum `TOKEN_REDIR_IN, TOKEN_REDIR_OUT, TOKEN_APPEND, TOKEN_HEREDOC`. It then
  advances and requires the **next** token to be a `C_WORD` (the target);
  otherwise `SNTX_REDIR_NO_TARGET`. The target word is stored with the redir.

### Assembling state (`state.c`)

- **`parser_start_command`** lazily allocates the `t_simple` and initializes the
  `args` and `redirs` dynamic arrays.
- **`parser_finish_command`** closes a command. If an error was flagged, it frees
  the half-built command. If the command ended up **completely empty** (no args
  and no redirs — e.g. the input was just spaces), that's `SNTX_EMPTY_CMD`.
- **`parser_add_arg`** appends a `char *` to `args` and keeps the array
  `NULL`-terminated (so it can be handed straight to `execve` later).
- **`parser_add_redir`** builds a `t_parser_redir { type, file, expand, quoted }`
  and appends it to `redirs`. Note `redir.expand = !token->quoted` — a redirection
  target is expanded *unless* it was quoted. (Heredoc delimiters are special; see
  §9.3.)

### Errors & cleanup

- **`errors.c`** — `parser_set_syntax_error` sets `state = TRUE` and records the
  `error_type` on the ast, so the message and the bail-out happen together.
- **`cleanup.c`** — `parser_free_cmd` recursively frees the tree (both children
  of a pipe, or the arrays of a leaf). `parser_free_ast` frees the root and the
  ast wrapper. `parser_free_simple` frees a standalone `t_simple`.

The five possible syntax errors (`t_syntax_error`): unclosed quote, pipe at
start, pipe at end, empty command, redirection with no target.

---

## 9. `runner/` — making things actually happen

This is the largest and most interesting subsystem: it takes a command tree and
performs real operations on the operating system — expanding variables, opening
files, duplicating file descriptors, forking processes, and calling `execve`.

### The dispatcher (`control.c`)

`rn_execute(cmd, env, heredoc_fd, ast)` is the single entry point and it is
**recursive** through pipelines:

```
 rn_execute(cmd)
   ├─ cmd is PNODE_CMD  ──▶ rn_exec_cmd   (a single command)
   └─ cmd is PNODE_PIPE ──▶ rn_pipe       (a pipeline; will call rn_execute again
                                           inside each child)
 ...then store the result as $?.
```

**`rn_exec_cmd`** — the lifecycle of *one* simple command, and it reads like a
careful four-step dance:

1. **Expand** the args: `rn_expand(simple->args.items, env)`. This performs
   `$VAR` expansion, quote removal, and word splitting (see §9.1). Result is a
   fresh `char **`.
2. **Apply redirections** (`rn_exec_redirs` → `rn_redir_push`): open every
   target, save the current stdin/stdout, and swap the fds. The original
   stdin/stdout are remembered in `saved[2]` so they can be put back.
3. **Run** the command (`rn_cmd_run`): if `args[0]` is a builtin, call it; else
   run an external program.
4. **Restore** the original stdin/stdout (`rn_redir_restore`) and free the args.

> Why save & restore instead of just letting fds change? Because a **builtin runs
> in the *same* process as the shell** (no fork). If `export FOO=bar > file`
> redirected stdout and we never restored it, every later prompt would write into
> `file`. So for the parent process we always undo the redirection afterwards.
> (External commands run in a *child*, so their redirections die with the child —
> but the save/restore is uniform and harmless there.)

**`rn_cmd_run`** — `if rn_exec_bin(...) return its status; else rn_exec_ext(...)`.

### Status (`status.c`)

`rn_status_set` writes `$?` (stores `ft_itoa(status)` under `ENV_ERRCODE`);
`rn_status_get` reads and `ft_atoi`s it. Every command's result lands here, and
the expander reads it back when you type `$?`.

---

### 9.1 The expander — word splitting and variables

Files: `runner/expander/`. Entry: `rn_expand(char **args, t_env **env)`.

This is the stage that makes `echo $HOME` print your home directory and turns
`"$X"` and `$X` into subtly different things. It walks **each argument character
by character**, maintaining a growing output **word buffer** (`t_word`), and
emits finished words into a list. The output is a brand-new `char **`.

Key structures:
```c
typedef struct s_word {           // a single word being built
    char *buf; int len; int cap;
    int   active;                 // has this word received any char yet?
}   t_word;

typedef struct s_exp_ctx {        // everything the char-handlers need
    t_array *list;   // finished words
    t_word  *word;   // current word
    char    *arg;    // the source argument
    int     *i;      // cursor into arg
    t_env  **env;
}   t_exp_ctx;
```

**`exp_process_char`** (in `dispatch.c`) is the heart. For each character it
chooses a handler:

| Char        | Handler          | Behavior                                            |
|-------------|------------------|-----------------------------------------------------|
| `'`         | `exp_single`     | copy verbatim until the closing `'`. **No expansion.** |
| `"`         | `exp_double`     | copy until closing `"`, but **do** expand `$` inside. |
| `$`         | `exp_var`        | expand a variable (unquoted context).               |
| whitespace  | `exp_process_space` | flush the current word, skip the spaces (word split). |
| otherwise   | `exp_wordchar`   | append the literal character.                        |

The `active` flag is what makes empty-but-real words work. Opening a quote sets
`active = 1` even before any character is copied, so `""` produces an **empty
string argument**, while a `$UNSET` that expands to nothing produces **no word at
all**. This matches bash precisely.

**Variable expansion** (`variable.c`):
- `exp_varvalue` advances past `$`, then:
  - `$?` → the current exit status (via `exp_var_status`, reads `ENV_ERRCODE`).
  - a valid name (`sh_varstart` then `sh_varchar*`) → `env_get` that key.
  - anything else (e.g. `$` followed by a space or digit) → returns `NULL`,
    meaning "this isn't a variable", and the caller emits a literal `$`.
- The **quoted vs unquoted** difference is the whole point of having two callers:
  - `exp_var` (unquoted) → `exp_add_unquoted`: the variable's value is itself
    **word-split** on whitespace. So if `X="a b"`, then `echo $X` prints two
    arguments.
  - `exp_var_quoted` (inside `"`) → `exp_wordstr`: the value is appended as-is,
    **no splitting**. So `echo "$X"` prints one argument `a b`.

**The word buffer** (`word.c`): `exp_wordchar`/`exp_wordstr` append to `buf`,
growing it geometrically (`wordgrow` doubles capacity). `list.c`'s
`exp_flush_word` finalizes the current buffer into a heap string and pushes it to
the list (only if `active`), then resets the buffer for the next word.

`expand.c` drives all of this per-argument (`rn_argexpand`), collects results in
a `t_array`, and finally appends a `NULL` terminator (`rn_expand_done`) so the
result is a ready-to-exec `char **`.

> **In one sentence:** the expander replaces variables, removes quotes, and
> splits unquoted whitespace — producing the final argument vector the program
> will actually receive.

---

### 9.2 Redirections — the careful art of moving fds

Files: `runner/redir/`. This is one of the two "obscure" areas, so let's build
the mental model first.

#### The mental model

Every process has a table of **file descriptors** (small integers). By
convention `0` = stdin, `1` = stdout, `2` = stderr. A program like `cat` doesn't
know or care *what* fd 0 is connected to — a terminal, a file, a pipe. It just
reads fd 0.

A redirection like `cat < file` means: *before running cat, make fd 0 refer to
`file` instead of the terminal.* The tool for "make this fd refer to what that fd
refers to" is **`dup2(oldfd, newfd)`**: it closes `newfd` and makes it a copy of
`oldfd`. So `dup2(file_fd, 0)` is exactly "stdin now comes from the file".

The four redirections:

| Syntax | Meaning                | `open` flags                          | Target fd |
|--------|------------------------|---------------------------------------|-----------|
| `<`    | read from file         | `O_RDONLY`                            | stdin (0) |
| `>`    | write, truncate        | `O_WRONLY \| O_CREAT \| O_TRUNC`      | stdout(1) |
| `>>`   | write, append          | `O_WRONLY \| O_CREAT \| O_APPEND`     | stdout(1) |
| `<<`   | heredoc (inline input) | (a pipe, see §9.3)                     | stdin (0) |

#### The flow, file by file

**`push.c` — save, apply, (later) restore.**
- `redir_save_stdio(saved)` — `dup` the current stdin and stdout into
  `saved[0]`/`saved[1]`. These are *backups* so we can return to the terminal
  afterward.
- `rn_redir_push` — save, then `rn_redir_apply`. If applying fails, restore
  immediately and report the failure.
- `rn_redir_restore(saved)` — `dup2` the backups back onto 0 and 1, then close
  the backups. This is the "undo" used after a builtin (see §9 dispatcher).

**`target.c` — figure out *what* to open.**
- `rn_redir_target` — for a normal redirection, the target word is run through
  the **expander** (`rn_expand`). If it expands to zero or more-than-one word,
  that's bash's *"ambiguous redirect"* error (you can't redirect into two files).
  For a **heredoc**, the delimiter is *not* expanded; it's only quote-stripped
  with `sh_quote_remove`.
- `rn_redir_open` — the `open(2)` call with the right flags from the table above.
- `rn_redir_fd` — glue: resolve the target, then either open the file or run the
  heredoc; on a normal open failure, print `minishell: target: <errno message>`.

**`apply.c` — open all, then swap all.** This file is careful about a real
problem: if a command has several redirections, some may open successfully before
one fails, and those already-open fds must not leak.
- `redir_open_all` — open every redirection into a temporary `t_redir_fd[]`
  array, each remembering both the opened `fd` and which `stdio` (0 or 1) it
  targets. If any open fails, it closes everything opened so far and returns
  (propagating `EXIT_SIGINT` if a heredoc was interrupted with Ctrl-C).
- `redir_dup_all` — only after *all* opens succeed, `dup2` each onto its target
  stdio and close the now-duplicated original. Doing opens and dups in two phases
  is what makes the cleanup-on-failure correct.
- When a command has **multiple redirections to the same stream**
  (`> a > b`), they are applied in order, so the *last* one wins for the actual
  data — while both files still get created. That matches bash.

---

### 9.3 Heredocs — the most subtle redirection

Files: `runner/redir/heredoc.c`, `heredoc_read.c`, `expand.c`, `strbuilder.c`.

A heredoc (`<< EOF`) feeds inline text to a command's stdin until a delimiter
line is seen:
```
cat << EOF
hello $USER
EOF
```
Here `cat`'s stdin should be the two-or-so lines you typed, *not* the terminal.

#### How it works

The trick: **a pipe is used as a temporary buffer.** `rn_redir_heredoc`:
1. `pipe(pfd)` — creates a pipe: `pfd[0]` is the read end, `pfd[1]` the write end.
2. Reads heredoc lines and **writes them into `pfd[1]`**.
3. Closes the write end and returns `pfd[0]` — the read end — as the fd that will
   become the command's stdin. So the command reads exactly what we wrote.

#### Reading the lines (`heredoc_read.c`)

- `rn_redir_readline(input_fd)` prints the `> ` continuation prompt (to stderr)
  and reads **one line, one byte at a time** from `input_fd` (a tiny growable
  buffer via `rn_redir_linegrow`). Reading byte-by-byte — rather than using
  readline — is deliberate: it lets the heredoc read from an arbitrary fd, which
  matters inside pipelines (where stdin has been replaced by a pipe, so the
  terminal fd is passed in separately as `input_fd`).
- `rn_redir_heredoc_loop` repeats:
  - `NULL` line → **EOF** (Ctrl-D before the delimiter) → `HEREDOC_EOF`.
  - a `SIGINT` was caught → `HEREDOC_INTR`.
  - line **equals the delimiter** → `HEREDOC_DONE` (stop, don't write it).
  - otherwise → expand it if needed and write it to the pipe.

#### Expanding heredoc bodies (`expand.c`)

Whether the body expands `$VAR` depends on whether the **delimiter was quoted**.
Recall the parser set `redir.expand = !token->quoted`. If the delimiter was
written as `<< "EOF"` or `<< 'EOF'`, expansion is off and lines are copied
verbatim; otherwise `$VAR` and `$?` are expanded line by line by
`rn_redir_expand_line` (using the same `exp_varvalue` as the main expander, plus
the small string builder in `strbuilder.c`). Every line gets a trailing `\n`.

#### Signals during a heredoc (`heredoc.c`)

This is where signals and redirs intersect. `rn_redir_heredoc_fill`:
- switches signals to **heredoc mode** (`SIG_HEREDOC`) so Ctrl-C interrupts the
  *reading loop* instead of the shell,
- runs the loop,
- switches signals back to **interactive**,
- and translates the various end-states into the right return:
  - `HEREDOC_INTR` → `REDIR_FD_INTR` (a special `-2`): the command is aborted and
    `$?` becomes 130. The fd is closed.
  - `HEREDOC_FAIL` → `REDIR_FD_ERR` (`-1`): an allocation/write error.
  - `HEREDOC_EOF` → prints bash's *"here-document delimited by end-of-file"*
    warning but still uses what was collected.
  - otherwise → returns the read-end fd.

That `REDIR_FD_INTR` value is propagated all the way up through `apply.c` so the
whole command is cancelled cleanly with exit status 130 — exactly what bash does
when you press Ctrl-C while typing a heredoc.

---

### 9.4 External commands & PATH resolution

Files: `runner/external/`, `runner/path/`.

When `args[0]` is **not** a builtin, the shell must find and run a program.

**Resolving the path (`external/resolve.c`):**
- If `args[0]` contains a `/`, it is treated as a literal path (relative or
  absolute) — no `$PATH` search.
- Otherwise `rn_path` searches `$PATH`.
- `rn_ext_classify` then `stat`s the resolved path and decides:
  `EXT_NOT_FOUND` / `EXT_DIR` (it's a directory) / `EXT_DENIED` (no execute
  permission) / `EXT_READY`. `rn_ext_report` prints the matching message and
  returns the matching exit code (`127` not found, `126` not executable).

**Searching `$PATH` (`path/path.c`, `path/parts.c`):**
- `rn_path_search` splits `$PATH` on `:` and, for each directory,
  `rn_path_join`s `dir + "/" + cmd`.
- `rn_path_candidate` accepts the first entry that exists, is not a directory,
  and is executable (`access(X_OK)`). If it only finds a *non-executable* match,
  it remembers it as a `fallback` so the eventual error is "permission denied"
  rather than "command not found" — a small touch of fidelity to bash.

**Forking and executing (`external/process.c`):**
```
 rn_exec_ext
   ├─ rn_ext_resolve            find the program (or fail with 126/127)
   ├─ env_toarr                 build char** envp for execve
   └─ ext_fork
        ├─ sh_sig_mode(SIG_EXEC)   parent ignores Ctrl-C/Ctrl-\ while child runs
        ├─ fork()
        │    └─ child: rn_ext_child
        │         ├─ signal(SIGINT/SIGQUIT, SIG_DFL)   child uses default signals
        │         ├─ execve(path, args, envp)
        │         └─ _exit(rn_ext_execfail(path))      only if execve failed
        └─ parent: rn_ext_wait
             ├─ waitpid for the child
             ├─ sh_sig_mode(SIG_INTERACTIVE)           restore prompt signals
             └─ translate wait status → exit code
```

The status translation is standard and important:
- `WIFEXITED` → the program's own exit code (`WEXITSTATUS`).
- `WIFSIGNALED` → `128 + signal_number`. So a program killed by `SIGINT` (2)
  yields 130, by `SIGQUIT` (3) yields 131, etc. This is the universal shell
  convention and is why `$?` is 130 after Ctrl-C.

`rn_ext_execfail` (run only in the child, after a failed `execve`) maps `errno`
to the right message/exit code (`ENOENT`→127, `EISDIR`/`EACCES`/`ENOEXEC`→126).

---

### 9.5 Pipelines — many processes, one data stream

Files: `runner/pipe/`. This is the second "obscure" area.

A pipeline `a | b | c` runs three programs **concurrently**, with `a`'s stdout
connected to `b`'s stdin, and `b`'s stdout to `c`'s stdin. The connector is again
`pipe(2)`: it returns two fds, and bytes written to the write end come out of the
read end.

For N commands you need **N−1 pipes**.

#### Step 1 — flatten the tree (`tree.c`)

The parser built a *tree* of pipes, but execution wants a flat **array** of
commands. `rn_pipe_count` counts the leaves; `rn_pipe_flatten` does an in-order
walk filling `cmds[]` left to right. So the tree for `a | b | c` flattens to
`[a, b, c]`.

#### Step 2 — set up the context (`pipe.c`)

`rn_pipe_ctx` (a `t_pipe_ctx`) gathers everything one place:
```c
typedef struct s_pipe_ctx {
    t_command **cmds;        // flattened commands, NULL-terminated
    t_command  *root;        // the pipe tree root (for child cleanup)
    t_ast      *ast;
    t_env     **env;
    int        *fds;         // all pipe fds: 2 ints per pipe, laid end to end
    pid_t      *pids;        // one per command
    size_t      pipe_count;  // number of pipes == commands - 1
}   t_pipe_ctx;
```
`pipe_alloc` allocates `cmds` and the `fds` array (size `(N-1) * 2`).
`rn_pipe_create` (in `fd.c`) initializes every fd to `-1`, then calls `pipe()`
for each pipe, storing the pair at `fds[i*2]` (read) and `fds[i*2+1]` (write).

#### Step 3 — fork every command (`process.c`)

`rn_pipe_fork_wait`:
1. allocate `pids[]`,
2. set signals to **exec mode** (the parent shell ignores Ctrl-C while the
   pipeline runs),
3. `rn_pipe_fork_all` — `fork` once per command; each child runs
   `rn_pipe_child`,
4. **the parent closes all pipe fds** (`rn_pipe_close_all`) — this is essential:
   if the parent kept a write end open, the reader at the end of the pipe would
   never see EOF and would hang forever,
5. `rn_pipe_wait` — `waitpid` every child; the pipeline's exit status is the
   **last** command's status (bash semantics),
6. restore **interactive** signals, free `pids`.

#### Step 4 — wire up each child (`fd.c`, `rn_pipe_child`)

This is the crux. For the command at position `pos`:
```
 if pos > 0:    dup2(fds[(pos-1)*2],     STDIN)   // read from the previous pipe
 if has next:   dup2(fds[pos*2 + 1],     STDOUT)  // write into our own pipe
 close ALL pipe fds in this child   (we only need the two we just dup'd)
 reset SIGINT/SIGQUIT to default    (children are normal foreground processes)
 rn_execute(cmds[pos], env, heredoc_fd, NULL)     // recurse! run the command
 _exit(status)
```
The first command keeps the real stdin; the last keeps the real stdout. Every
child closes *all* the pipe fds after dup-ing, because leaving extra copies open
would, again, prevent EOF from propagating.

**The heredoc-in-a-pipeline subtlety** (`rn_heredoc_fd`): a command inside a
pipeline can contain a heredoc (`a | cat << EOF`). But that child's stdin is
about to be replaced by a pipe via `dup2`. So *before* the `dup2`, the child
saves a private copy of the current terminal fd with `dup(STDIN_FILENO)` and
passes that to `rn_execute` as the `heredoc_fd`. That's the very argument that
threads all the way down to `rn_redir_readline`, so the heredoc can still read
your typing from the terminal even though the command's real stdin is a pipe.
(The first command, `pos == 0`, still has the real terminal on stdin, so it just
passes `STDIN_FILENO`.)

**Child cleanup** (`cleanup.c`): each forked child is a full copy of the shell,
so before `_exit` it frees the environment, history, the command tree, and the
pipe bookkeeping — keeping the children leak-free under tools like valgrind.

> **Why does a builtin like `export` not change the parent in a pipeline?**
> Because in a pipeline every command — builtin or not — runs in a forked child
> (`rn_pipe_child` calls `rn_execute`, which for a leaf calls the builtin *in the
> child*). The child's environment change dies with it. That's exactly bash's
> behavior: `export X=1 | cat` does not actually set `X` in your shell.

---

## 10. `bin/` — the builtins

Builtins are commands the shell runs **itself**, in its own process (when not in
a pipeline), because they need to affect the shell's own state (current
directory, environment, exit). `runner/builtin.c`'s `rn_exec_bin` is just a
dispatch table matching `args[0]` to one of seven functions; each returns an exit
code and writes it through `*status`.

- **`echo.c`** — prints its arguments separated by spaces. Supports `-n`
  (suppress trailing newline) and even runs of it like `-nnn`; `echo_has_flag`
  validates that a `-...` argument is *only* `n`s before treating it as a flag.
- **`pwd.c`** — prints `getcwd`; falls back to the `PWD` variable if `getcwd`
  fails.
- **`cd.c`** — changes directory. `getpath` resolves the destination: no arg →
  `$HOME`; `-` → `$OLDPWD` (and echoes it, like bash); otherwise the literal arg.
  Rejects more than one argument. After a successful `chdir`, it updates `OLDPWD`
  (to the previous `PWD`) and `PWD` (to the new `getcwd`). *(Note: the codebase
  uses the key `OPWD` via the `ENV_OPWD` constant for "old pwd".)*
- **`env.c`** — prints `KEY=VALUE` for every variable that has a value, skipping
  the internal `$?`. With any argument it errors (this minishell's `env` takes no
  args/options).
- **`unset.c`** — removes each named variable, validating identifiers first
  (invalid names produce an error and exit code 1, but processing continues).
- **`export/`** — three files:
  - `export.c` — with no args, print the sorted environment (`export_print`).
    With args, for each `KEY=VALUE` set the variable; for a bare `KEY` mark it
    exported with a `NULL` value. Validates identifiers (`export_invalid`).
  - `keys.c` — collect the keys (skipping `$?`) and **bubble-sort** them so the
    listing is alphabetical, like bash.
  - `print.c` — print each as `declare -x KEY="VALUE"` (bash's `export` format),
    omitting the `="..."` for value-less exported keys.
- **`exit.c`** — the trickiest builtin. It parses the optional numeric argument
  with full overflow checking against `LLONG_MAX`/`LLONG_MIN`
  (`exit_parse_prefix` / `exit_parse_digits` / `exit_apply_sign`). A
  non-numeric argument → `"numeric argument required"` and exit code 2. Too many
  arguments → error, code 1, and it does **not** exit. The final exit code is
  `(unsigned char)code` (so it wraps mod 256, like every shell). It signals the
  REPL to stop by setting `g_signal = SH_EXIT_REQUESTED` — a clean way to ask the
  loop in `reader.c` to break out. It also prints `exit` to stderr when
  interactive.

---

## 11. Signals — the deep dive

Signals are the other genuinely obscure topic, so here is the whole story in one
place. Everything lives in `shared/signal.c`, plus per-child resets in the
runner.

### The one global

The subject allows exactly one global variable, and this is it:
```c
volatile sig_atomic_t g_signal = 0;
```
- `volatile` — tells the compiler the value can change at any moment (inside a
  signal handler), so it must always re-read it from memory.
- `sig_atomic_t` — a type guaranteed to be read/written in one uninterruptible
  step, so a handler can never observe a half-written value.

It is used as a tiny mailbox: a handler writes into it, and the main loop reads it
to learn "did a SIGINT happen?". It is also overloaded to carry the
`SH_EXIT_REQUESTED` sentinel that `exit` uses to ask the loop to stop.

### The three modes (`sh_sig_mode`)

A shell needs *different* signal behavior depending on what it's doing. The enum
`t_sig_mode` names the three situations, and `sh_sig_mode` installs the right
handlers via `sigaction`:

| Mode             | When                         | SIGINT (Ctrl-C)                 | SIGQUIT (Ctrl-\\) |
|------------------|------------------------------|---------------------------------|-------------------|
| `SIG_INTERACTIVE`| sitting at the prompt        | `sh_sigint_interactive`         | ignored           |
| `SIG_EXEC`       | a child/pipeline is running  | ignored in the **parent**       | ignored           |
| `SIG_HEREDOC`    | reading a heredoc body       | `sh_sigint_heredoc`             | ignored           |

`SIGQUIT` is **always ignored** in the shell itself — bash never quits its prompt
on Ctrl-\\. Children, however, reset it to default so a runaway program can still
be core-dumped with Ctrl-\\.

### The handlers

- **`sh_sigint_interactive`** (at the prompt): print a newline, then use the
  readline helpers `rl_on_new_line` + `rl_replace_line("", 0)` + `rl_redisplay`
  to wipe the current input and draw a fresh empty prompt. This is the classic
  "Ctrl-C clears the line and gives you a new prompt" behavior. (It sets and then
  clears `g_signal`; the loop separately bumps `$?` to 130.)
- **`sh_sigint_heredoc`** (reading a heredoc): set `g_signal = SIGINT` and print a
  newline. The heredoc read loop notices `g_signal == SIGINT` and aborts the
  heredoc, which becomes exit status 130.

### `ECHOCTL` — a small terminal nicety

`sh_echoctl` toggles the terminal's `ECHOCTL` flag with `termios`. When enabled,
control keys echo as `^C`; the shell turns it **on** in interactive mode and
**off** during a heredoc so a stray `^C` doesn't litter the heredoc transcript.

### Who switches modes, and when

- Startup and after every command → `SIG_INTERACTIVE` (the prompt is safe).
- About to fork an external command or a pipeline → `SIG_EXEC` (so Ctrl-C goes to
  the *child*, not the shell — the shell just waits).
- Inside `execve`'d children and pipeline children → `signal(SIGINT, SIG_DFL)` and
  `signal(SIGQUIT, SIG_DFL)`, so the program behaves like any normal foreground
  process.
- Entering a heredoc → `SIG_HEREDOC`; leaving it → `SIG_INTERACTIVE`.

This separation is what makes the shell feel right: Ctrl-C at the prompt clears
the line, Ctrl-C during `sleep 100` kills the sleep (and sets `$? = 130`) but
leaves your shell alive, and Ctrl-C during a heredoc cancels just that heredoc.

---

## 12. The complete flow, end to end

Let's trace one realistic line all the way through:

```
GA&Fabricio:minishell$ grep "$NEEDLE" file.txt | wc -l > count.txt
```

**0. Startup** (`main` → `core_run` → `core_init`)
The environment is parsed into a `t_env` list, `SHLVL` is bumped, `$?` is seeded
to `0`, and signals enter `SIG_INTERACTIVE`. The terminal settings are saved.

**1. Read** (`core_iteration`)
`readline` prints the prompt and returns the line. It's non-empty, so it's added
to history and passed to `core_process_line`.

**2. Lex** (`core_parse` → `lexer`)
The scanner walks the string using the class table. It produces tokens:
```
WORD("grep")  WORD("\"$NEEDLE\"")  WORD("file.txt")  PIPE("|")
WORD("wc")    WORD("-l")           REDIR_OUT(">")     WORD("count.txt")
```
Note the second word keeps its quotes for now, and is flagged `expand=true`
(it contains `$`) and `quoted=true`.

**3. Parse** (`parser_controller` → `build_pipeline`)
First token isn't a `|`, good. `parse_simple_command` builds the left command:
- `args = ["grep", "\"$NEEDLE\"", "file.txt"]`, `redirs = []`.
Then a `|` is seen, so the right command is parsed:
- `args = ["wc", "-l"]`, and `consume_redir` sees `>` then the word
  `count.txt`, recording `redir { REDIR_OUT, "count.txt", expand=true }`.
The two leaves are folded into a `PNODE_PIPE`. The `t_ast` wraps it, `error = 0`.

**4. Execute** (`rn_execute(root, ...)`)
Root is a `PNODE_PIPE` → `rn_pipe`.
- Flatten the tree to `cmds = [grep-cmd, wc-cmd]`. One pipe needed; create it.
- Signals → `SIG_EXEC`. Fork twice.

**Child 0 (`grep`)** in `rn_pipe_child(pos=0)`:
- It's the first command, so stdin stays the terminal; it has a next command, so
  `dup2(pipe_write, STDOUT)`. Close all pipe fds. Reset signals to default.
- `rn_execute(grep-cmd, ..., PNODE_CMD)` → `rn_exec_cmd`:
  - **Expand** the args: `"$NEEDLE"` is inside double quotes, so `$NEEDLE` is
    looked up and substituted **without** word-splitting; quotes are removed.
    Result: `["grep", "<value of NEEDLE>", "file.txt"]`.
  - No redirs on this command.
  - `grep` isn't a builtin → `rn_exec_ext`: resolve `grep` via `$PATH`, `execve`
    it. Its stdout is the pipe.
- `_exit` with grep's status.

**Child 1 (`wc -l`)** in `rn_pipe_child(pos=1)`:
- `pos > 0`, so `dup2(pipe_read, STDIN)` — its input is grep's output. It's the
  last command, so stdout stays... but wait, it has a redirection.
- `rn_execute(wc-cmd, ...)` → `rn_exec_cmd`:
  - Expand args → `["wc", "-l"]`.
  - **Apply redirs**: save stdio, open `count.txt` with
    `O_WRONLY|O_CREAT|O_TRUNC`, `dup2` it onto STDOUT. So `wc`'s output goes to
    the file, not the pipe-or-terminal.
  - `execve` `wc`. It reads grep's lines from stdin (the pipe), counts them,
    writes the number into `count.txt`.

**Parent** (`rn_pipe_fork_wait`):
- Closes all pipe fds (so the reader can see EOF).
- `waitpid`s both children; the pipeline's status is **`wc`'s** status.
- Restores `SIG_INTERACTIVE`.

**5. Remember** (`rn_status_set`)
The pipeline's status is written to `$?`. Back in `core_iteration`, `g_signal`
is checked (no exit requested), the line is freed, and the loop goes around for
the next prompt.

And that is the whole journey: **text → tokens → tree → processes → a number in
`$?`**, with signals keeping the terminal civilized and redirections quietly
rewiring file descriptors the whole way down.

---

*Read the modules in this order if you're exploring the source for the first
time:* `core/reader.c` → `lexer/` → `parser/` → `runner/control.c` →
`runner/expander/` → `runner/redir/` → `runner/pipe/`. Everything else is a
helper called from those.
