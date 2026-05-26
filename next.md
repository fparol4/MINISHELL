# Next

## Immediate Goal

Finish runner execution in the same model already established:

- `rn_execute` is the entrypoint.
- `CMD` and `PIPE` are the only node types that should drive execution.
- `ENV_ERRCODE` must be updated from the final status of every execution path.
- Redirections still need to be added later, but not in this step.

## Required Next Steps

1. Split `source/modules/runner/exec.c` further.
   - Move `rn_exec_bin` into a builtin-focused file.
   - Move `rn_exec_ext` into an external-command file.
   - Keep `rn_execute` and `rn_exec_cmd` in a small control-flow file.

2. Implement `rn_pipe`.
   - Create a pipe execution file for `PIPE` nodes.
   - Left side should write to the pipe.
   - Right side should read from the pipe.
   - Return the right-side status.
   - Update `ENV_ERRCODE` only once through `rn_execute`.

3. Add redirection support.
   - Create `rn_redir_*` helpers in a dedicated file.
   - Apply redirections in order.
   - Handle `IN_F`, `IN_H`, `OUT_T`, `OUT_A`.
   - Redirection targets must expand to exactly one argument.
   - Heredoc expansion must respect `redir.expand`.

4. Add runner tests for pipeline behavior.
   - Simple `echo | wc`.
   - Pipe with a failing left side and successful right side.
   - Nested pipes once supported.

5. Add runner tests for redirections.
   - Input redirection.
   - Output truncation.
   - Output append.
   - Heredoc with expansion on and off.

6. Keep the existing test layout stable.
   - `tests/envm.c`
   - `tests/bultin.c`
   - `tests/runner.c`
   - `tests/tester.h`
   - `make test` should remain the aggregate entrypoint.

## Current Context For Other Work

- The repo now uses:
  - `headers/shared.h`
  - `source/shared/error.c`
  - `source/shared/free.c`
  - `source/shared/string.c`
- Env helpers live under `source/modules/envm/`.
- Runner helpers currently live under:
  - `source/modules/runner/status.c`
  - `source/modules/runner/path.c`
  - `source/modules/runner/pipe.c`
  - `source/modules/runner/exec.c`
  - `source/modules/runner/expander.c`
- The runner tests already validate:
  - expansion
  - builtin dispatch
  - external execution
  - status propagation

## Suggested File Split Direction

- `runner/control.c`
  - `rn_execute`
  - `rn_exec_cmd`

- `runner/builtin.c`
  - `rn_exec_bin`

- `runner/external.c`
  - `rn_exec_ext`

- `runner/path.c`
  - `rn_path`

- `runner/status.c`
  - `rn_status_set`
  - `rn_status_get`

- `runner/pipe.c`
  - `rn_pipe`

- `runner/redir.c`
  - `rn_redir_*`

This keeps each file near the “five functions per file” target.
