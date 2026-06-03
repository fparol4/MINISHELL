# Minishell Agent Overview

This is a 42 `minishell` project in C. Stay inside this repo and `$HOME`; do not use `sudo` or inspect system-wide paths. Treat existing dirty work as user-owned.

## Current Repo State

- Main repo status currently includes a deleted `.ai/corrections.md` and a modified `libraries/libft` submodule.
- `libraries/libft` is a Git submodule used to build `libraries/libft/libft.a`; do not reset or clean it unless explicitly asked.
- The active umbrella header is `headers/minishell.h`. The root `minishell.h` is effectively legacy/minimal.

## Build And Test

- Build: `make`
- Run: `./minishell`
- Clean objects: `make clean`
- Full clean: `make fclean`
- Rebuild: `make re`
- All tests: `make test`
- Focused tests: `make t:envm`, `make t:builtin`, `make t:runner`, `make t:core`, `make t:lexer`, `make t:parser`

The Makefile compiles with `cc -g3 -Wall -Wextra -Werror`, includes `-I. -Iheaders -Ilibraries -Ilibraries/libft`, links `libraries/libft/libft.a` and `-lreadline`, and puts objects under `build/`.

## Runtime Flow

`source/main.c` calls `core_run(envp)`.

`source/core/` owns shell lifecycle:

- `core_init`: builds `t_shell.env`, initializes `$?` as `ENV_ERRCODE`, enables interactive signal mode.
- `core_loop`: readline prompt, history, lexer, parser, runner, final status.
- `core_destroy`: frees env.

Per command line:

1. `scanner_init`
2. `lexer_control` returns `t_manager` token list
3. `parser_controller` returns `t_ast`
4. Syntax errors print through `sh_stxerr` and set status `2`
5. `rn_execute(ast->root, &shell->env)`
6. Free lexer and parser allocations

## Main Data Types

- `t_shell` in `headers/core.h`: `{ t_env *env; t_bool running; }`
- `t_env` in `headers/env.h`: linked list of `key`, `value`, `next`
- `t_scanner` in `headers/scanner.h`: input cursor state
- `t_manager` / `t_node` in `headers/lexer.h`: aliases over `libft`'s generic `t_dlist` / `t_dlist_node`
- `t_list_token`: `{ value, type, expand, quoted }`
- `t_ast` in `headers/parser.h`: root command plus syntax error metadata
- `t_command` in `headers/parser_internal.h`: either `PNODE_CMD` with `t_simple`, or `PNODE_PIPE` with left/right commands
- `t_simple`: argv array, aggregate expand flag, redirection array, redirection count
- `t_parser_redir`: redir type, file/delimiter, expand flag, quoted flag

## Module Map

- `source/modules/lexer/`: tokenizes words, quotes, pipes, `<`, `>`, `<<`, `>>`; stores tokens in the `libft` doubly linked list; unclosed quote returns `NULL`.
- `source/modules/parser/`: builds simple command and left-associative pipeline AST; parses redirections in command order; owns AST cleanup.
- `source/modules/runner/`: expands args, applies redirections/heredocs, dispatches builtins, forks externals, executes pipelines, stores `$?`.
- `source/modules/builtin/`: `echo`, `cd`, `pwd`, `export`, `unset`, `env`, `exit`.
- `source/modules/envm/`: environment linked-list init/get/set/unset/size/free/to-array.
- `source/shared/`: errors, frees, signal globals/helpers, string/identifier helpers, quote removal.
- `tests/`: Snow-based unit tests; runner tests are split under `tests/runner/*.c` and included by `tests/runner.c`.

## Behavior Notes

- Builtins execute directly in the parent for a simple command, so env-changing builtins can mutate `t_env`.
- Pipelines are flattened before execution; each stage runs in a child.
- External resolution uses `PATH` unless command contains `/`; exit codes follow shell conventions: not found `127`, permission/dir/exec format `126`, signal status `128 + signal`.
- Expansion is in `runner/expander.c`: single quotes are literal, double quotes expand without word splitting, unquoted expansion can split on shell whitespace, `$?` reads `ENV_ERRCODE`.
- Redirection targets are expanded; ambiguous redirect is an error. Heredoc delimiters have quotes removed, and heredoc body expansion depends on the delimiter token expand flag.
- Signal modes are `SIG_INTERACTIVE`, `SIG_EXEC`, and `SIG_HEREDOC`; global `g_signal` tracks interrupts.

## Edit Guidance

- Prefer existing module boundaries and headers.
- Add new source files to the relevant `*_SRCS` list in `Makefile`.
- Keep memory ownership explicit: lexer owns token strings, parser owns AST/simple/redir strings, runner owns expanded argv arrays.
- After behavior changes, run the narrow `make t:*` target first, then `make test` when practical.
