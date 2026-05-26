# Progress

## Parser / AST

- The parser output is now treated as `CMD` or `PIPE` only.
- Redirections live on `t_redir` attached to a command node.
- Current AST shape in `headers/minishell.h`:
  - `t_node_type`: `CMD`, `PIPE`
  - `t_redir_type`: `IN_F`, `IN_H`, `OUT_T`, `OUT_A`
  - `t_redir`: `type`, `target`, `expand`, `next`
  - `t_exnode`: `type`, `args`, `redir`, `left`, `right`

## Shared Helpers

- Added `headers/shared.h`.
- Added shared functions in `source/shared/`:
  - `sh_freesec(void **items)`
  - `sh_freeargs(char **args)`
  - `sh_err(char *scope, char *msg)`
  - `sh_err2(char *scope, char *arg, char *msg)`
  - `sh_isspace(char c)`
  - `sh_varstart(char c)`
  - `sh_varchar(char c)`
- `sh_freeargs` is used by runner expander, env `toarr`, and tests.

## Env Module

- Added `env_size(t_env **envm)` in `source/modules/envm/size.c`.
- Fixed `env_toarr` in `source/modules/envm/toarr.c`.
- Replaced the old `common.c` inclusion pattern with:
  - `source/modules/envm/common.c`
  - `source/modules/envm/common.h`
- `envm` tests are in `tests/envm.c`.

## Builtins

- Builtin tests are in `tests/bultin.c`.
- Current builtin API is:
  - `bin_echo`
  - `bin_cd`
  - `bin_pwd`
  - `bin_exit`
  - `bin_env`
  - `bin_export`
  - `bin_unset`
- Builtins are already covered by tests and `make 't:bultin'`.

## Runner Expander

- Added `headers/runner.h`.
- Implemented `rn_expand(char **args, t_env **env)` in `source/modules/runner/expander.c`.
- Expansion behavior currently includes:
  - single quotes keep literal content
  - double quotes expand variables without splitting
  - unquoted variables split on spaces
  - mixed quoted/unquoted text concatenates correctly
- Runner tests cover the expander in `tests/runner.c`.

## Runner Executor

- Execution flow now includes:
  - `rn_execute(t_exnode *node, t_env **env)` in `source/modules/runner/control.c`
  - `rn_exec_cmd(t_exnode *node, t_env **env)` in `source/modules/runner/control.c`
  - `rn_exec_bin(char **args, t_env **env, int *status)` in `source/modules/runner/builtin.c`
  - `rn_exec_ext(char **args, t_env **env)` in `source/modules/runner/external.c`
- `rn_exec_bin` returns whether the builtin was handled and writes the status through the pointer.
- `rn_exec_ext` performs:
  - PATH lookup through `rn_path`
  - `env_toarr`
  - `fork`
  - `execve`
  - `waitpid`
- `rn_execute` updates `ENV_ERRCODE` via `rn_status_set`.

## Runner Split

- `source/modules/runner/status.c`
  - `rn_status_set`
  - `rn_status_get`
- `source/modules/runner/control.c`
  - `rn_execute`
  - `rn_exec_cmd`
- `source/modules/runner/builtin.c`
  - `rn_exec_bin`
- `source/modules/runner/external.c`
  - `rn_exec_ext`
- `source/modules/runner/path.c`
  - `rn_path`
- `source/modules/runner/pipe.c`
  - `rn_pipe`
  - forks left/right execution around a real pipe
  - left side writes to stdout of the pipe
  - right side reads from stdin of the pipe
  - returns the right-side status to `rn_execute`
- `source/modules/runner/redir.c`
  - applies `IN_F`, `IN_H`, `OUT_T`, `OUT_A`
  - expands redirect targets to exactly one argument
  - restores stdio after command execution

## Tests

- Test entry points are flat under `tests/`:
  - `tests/envm.c`
  - `tests/bultin.c`
  - `tests/runner.c`
  - `tests/tester.h`
- `snow.h` was copied locally into `libraries/snow.h`.
- Make targets exist:
  - `make 't:envm'`
  - `make 't:bultin'`
  - `make 't:runner'`
  - `make test`

## Current Verified State

- `make test` passes.
- `make 't:runner'` currently passes 10/10 tests.
- `make 't:envm'` currently passes 8/8 tests.
- `make 't:bultin'` currently passes 8/8 tests.

## Remaining Known Work

- `rn_pipe` is still a stub.
- Redirection handling is not implemented yet.
- The codebase still needs further file splits to keep each runner file small and focused.
