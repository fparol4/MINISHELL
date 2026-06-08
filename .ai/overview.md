# Minishell — Project Overview

## Subject (version 10.0, mandatory only)

Build a small Bash-like shell in C. Key mandatory requirements:

- **Prompt** displayed when waiting for input (readline).
- **History** via `add_history`.
- **Executable search**: PATH, relative, absolute paths.
- **One global variable** — only `volatile sig_atomic_t g_signal` to store signal number. Norm-type structs in global scope are forbidden.
- **Quotes**: single quote prevents all metachar interpretation; double quote prevents all except `$`.
- **Redirections**: `<` `>` `<<` (heredoc, no history update) `>>`.
- **Pipes** `|`.
- **Environment variable expansion** `$VAR` and `$?`.
- **Signals** (interactive mode): Ctrl-C → new prompt, Ctrl-D → exit, Ctrl-\ → nothing.
- **Builtins**: `echo` (with `-n`), `cd` (relative/absolute only), `pwd`, `export`, `unset`, `env`, `exit`.
- Memory leaks from readline are tolerated; project's own code must not leak.
- Makefile must have: `$(NAME)`, `all`, `clean`, `fclean`, `re`. Flags: `-Wall -Wextra -Werror`.
- README.md required (must start with italic 42 curriculum line, include Description, Instructions, Resources, and AI usage).

Not required: `\` (backslash), `;` (semicolon), `&&`, `||`, wildcards.

---

## Architecture

```
main.c
  └─ core_run()          [source/core/core.c + reader.c]
       ├─ env_init()     [source/env/]
       ├─ sh_sig_mode()  [source/shared/signal.c]
       └─ core_loop()
            ├─ readline() + add_history()
            ├─ lexer()           [source/lexer/]   → t_manager (doubly-linked token list)
            ├─ parser_controller() [source/parser/] → t_ast (binary tree: PNODE_CMD | PNODE_PIPE)
            └─ rn_execute()      [source/runner/]
                 ├─ expander/    (variable & quote expansion)
                 ├─ redir/       (apply redirections, heredoc)
                 ├─ pipe/        (fork + pipe tree execution)
                 ├─ external/    (PATH resolution + execve)
                 └─ builtin.c    (dispatch to source/bin/)
```

### Key data structures

| Type | File | Purpose |
|---|---|---|
| `t_shell` | minishell.h | Top-level: env list + running flag |
| `t_list_token` | lexer.h | Token: value, type (PIPE/REDIR_*/WORD), expand, quoted flags |
| `t_ast` | parser.h | Root command node + size + syntax error info |
| `t_command` | parser.h | Union: `t_simple` (args array + redirs array) or `t_pipe` (left/right) |
| `t_env` | env.h | Linked-list env store |
| `g_signal` | shared/signal.c | **Only global** — `volatile sig_atomic_t` |

### Signal handling

Three modes via `sh_sig_mode(t_sig_mode)`:
- `SIG_INTERACTIVE`: SIGINT → write newline + rl_redisplay, SIGQUIT → SIG_IGN.
- `SIG_EXEC`: SIGINT → SIG_IGN (children inherit default).
- `SIG_HEREDOC`: SIGINT → write newline + set g_signal.

### Lexer

State-machine scanner (`source/lexer/scanner/`) produces a doubly-linked list of `t_list_token`. Token types: `PIPE`, `REDIR_IN`, `REDIR_OUT`, `APPEND`, `HEREDOC`, `WORD`. Tracks `expand` and `quoted` flags per token.

### Parser

Converts token list into a binary AST. Simple commands (`PNODE_CMD`) hold an args `t_array` and redirs `t_array`. Pipe nodes (`PNODE_PIPE`) hold left/right `t_command*`. Syntax errors detected: unclosed quote, pipe at start/end, empty command, redir with no target.

### Runner

`rn_execute()` dispatches on node type:
- **Pipe node**: forks children, sets up fd plumbing, waits.
- **Simple node**: expands args + heredocs, applies redirections, then runs builtin or external.

Expander handles `$VAR`, `$?`, quote stripping, and word splitting.

---

## Source modules

| Module | Path | Responsibility |
|---|---|---|
| core | source/core/ | REPL loop, init/destroy shell |
| shared | source/shared/ | Errors, free helpers, signal, quote utils, strings |
| env | source/env/ | Env linked-list: init, get, set, unset, toarr, free |
| lexer | source/lexer/ | Tokenization (state machine + scanner) |
| parser | source/parser/ | AST construction + syntax error detection |
| runner | source/runner/ | Execution: pipe, redir, heredoc, expander, external, builtin dispatch |
| bin | source/bin/ | Builtins: cd, echo, env, exit, export, pwd, unset |
| libft | libraries/libft/ | Custom libc: strings, memory, lists, printf, gnl |

---

## Builtins implemented

`echo` (with `-n`), `cd`, `pwd`, `export` (no options), `unset` (no options), `env` (no options/args), `exit` (no options).

---

## Build

```sh
make          # build minishell
make re       # clean rebuild
make test     # run all test suites (envm, builtin, runner, core, lexer, parser)
```

Tests use the `snow.h` framework (`libraries/snow.h`). Test binaries go to `tests/build/`.

---

## README compliance (subject §V)

- [x] First line is italic 42 curriculum credit
- [x] Description section
- [x] Instructions section
- [x] Features section
- [x] Resources section (man pages)
- [x] AI usage described

---

## Evaluation scope (mandatory, no bonus)

Focus areas for peer review:
1. Correct behavior of all 7 builtins.
2. Pipe chains (multi-command pipelines).
3. All four redirections including heredoc.
4. Variable expansion (`$VAR`, `$?`).
5. Quote handling (single vs double).
6. Signal behavior (Ctrl-C/D/\).
7. Exit status propagation (`$?`).
8. No memory leaks (project code; readline leaks are tolerated).
9. Only one global variable (`g_signal`).
10. Makefile rules and compilation flags.
