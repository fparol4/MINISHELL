# Next 002

## Scope

Continue **executor-only** work.

Do **not** work on lexer/parser here.

The current executor already supports:

- `rn_execute` as the entrypoint
- `CMD` and `PIPE` node execution
- builtin dispatch
- external command execution
- argument expansion
- redirections
- pipe + redirection interaction
- `ENV_ERRCODE` update through `rn_execute`

## Current Runner Layout

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
- `source/modules/runner/redir.c`
  - `rn_redir_push`
  - `rn_redir_restore`
  - internal redirection helpers
- `source/modules/runner/status.c`
  - `rn_status_set`
  - `rn_status_get`
- `source/modules/runner/expander.c`
  - `rn_expand`

## Current Test Layout

- `tests/envm.c`
- `tests/bultin.c`
- `tests/runner.c`
- `tests/runner/common.c`
- `tests/runner/expand.c`
- `tests/runner/command.c`
- `tests/runner/pipe.c`
- `tests/runner/redir.c`
- `tests/runner/status.c`
- `tests/tester.h`

## Current Verified State

- `make 't:runner'` passes **23/23**
- `make test` passes

Runner coverage already includes:

- expansion
- builtin dispatch
- external execution
- status propagation
- plain pipes
- plain redirections
- pipe + input redirection
- pipe + output redirection
- pipe + heredoc

## Current Execution Model

- `rn_execute` is still the only place that finalizes `ENV_ERRCODE`
- `rn_exec_cmd` expands argv, applies redirections, runs builtin/external logic, restores stdio
- `rn_pipe` forks left/right children, connects pipe ends first, then each child re-enters `rn_execute`
- command-level redirections override pipe stdio when they target the same stream

## Main Remaining Executor Work

The next work should focus on **short, high-value improvements** that increase shell correctness and shell UX.

---

## 1. Map the improvements needed in `rn_exec_ext`

Current problem:

- `rn_exec_ext` is still too generic in its error handling
- it mostly collapses failures into `127`, `126`, or generic `"execution failed"`

What needs to be improved:

1. **Slash-path handling**
   - `./foo`
   - `/bin/ls`
   - `dir/script`
   - these must not be treated exactly like PATH lookup misses

2. **Missing command vs missing path**
   - command not found through `PATH` -> `127`
   - direct path that does not exist -> shell-style missing file / `127`

3. **Permission denied**
   - existing file without execute permission -> `126`

4. **Directory execution**
   - trying to execute a directory should become `126`

5. **Exec format / non-executable runtime failures**
   - `execve` failing after path resolution should map to the proper shell-style status
   - likely `126` for executable resolution failures that are not “not found”

6. **Error messages**
   - messages should be less generic than `"execution failed"`
   - map message text to the actual failure class

Suggested implementation direction:

- keep `rn_exec_ext` small
- move status/message mapping into small helpers in `external.c`
- likely inspect `access`, maybe `stat`, and `errno`
- distinguish:
  - PATH miss
  - path exists but is not executable
  - path is directory
  - `execve` runtime failure

### Tasks

1. Add a helper to detect whether argv[0] is a slash-path.
2. Add a helper that classifies a resolved target before `execve`.
3. Add `errno`-based handling after `execve` failure in the child.
4. Return shell-style codes:
   - `127` for not found
   - `126` for permission / directory / invalid executable class
5. Replace generic messages with clearer shell-style messages.
6. Add dedicated external-exec tests for these cases.

---

## 2. Map and write the tasks for shell-experience improvements

This is the higher-level list to improve UX without changing parser/lexer.

### Priority bucket

1. external command error quality
2. heredoc multi-entry behavior + warning
3. fd cleanup / close discipline
4. path resolution correctness for slash-paths
5. confidence tests for all of the above

### Suggested tasks

1. Improve external command classification and status mapping.
2. Implement multiple heredoc handling in-order.
3. Add heredoc EOF-before-delimiter warning behavior.
4. Close inherited saved stdio fds before `execve`.
5. Improve direct-path execution classification.
6. Add focused runner tests for failure semantics and edge cases.

---

## 3. Multiple heredocs + warning: map and tasks

Current state:

- one heredoc works
- expansion on/off works
- heredoc can participate in a pipe
- there is no explicit work yet for **multiple heredocs on the same command**
- there is no warning when EOF arrives before the delimiter

What is required:

1. **Apply multiple heredocs in list order**
   - same as other redirections: left to right
   - later input redirections should override earlier stdin
   - for multiple heredocs, every heredoc should still be processed in order

2. **EOF-before-delimiter warning**
   - when stdin ends before delimiter match
   - emit a warning similar to shell behavior

3. **Keep heredoc expansion behavior**
   - `redir.expand == 1` expands variables
   - `redir.expand == 0` keeps content literal

### Suggested implementation tasks

1. Review `rn_redir_apply` to confirm all heredocs are processed left-to-right even when later ones override stdin.
2. Add an explicit helper for heredoc read completion status:
   - delimiter found
   - EOF before delimiter
   - read/write failure
3. Emit a warning on EOF-before-delimiter.
4. Add tests for:
   - two heredocs on one command
   - heredoc followed by `< file`
   - `< file` followed by heredoc
   - EOF before delimiter warning

### Expected semantic result

- all heredocs are consumed in order
- final stdin still follows the last input redirection
- warning appears when delimiter was not seen before EOF

---

## 4. FD cleanup: map if not much code

This should be implemented if it stays small.

Current issue:

- `rn_redir_push` saves stdin/stdout
- external execution forks
- those saved fds can leak into the child if not closed before `execve`

### Suggested implementation

- in the child path of `rn_exec_ext`, close non-needed inherited fds before `execve`
- or mark saved descriptors close-on-exec when they are created

### Tasks

1. Map where saved stdio fds can survive into children.
2. Choose one small fix:
   - close-on-exec
   - explicit child close
3. Add a short regression test if practical, otherwise document in code comments minimally.

---

## 5. We must close

This refers to **descriptor discipline** in the executor.

Review and confirm all these areas:

1. redirection open fd is always closed after `dup2`
2. heredoc pipe write/read ends are closed on every path
3. pipe child setup closes both inherited pipe ends after `dup2`
4. saved stdio fds are closed after restore
5. error paths do not leak fds

This item is partly implemented already, but the next agent should audit it while touching external/heredoc paths.

---

## 6. Path resolution correctness: map if not much code

This should also be implemented if it stays small.

Current issue:

- `rn_path` currently checks `access(..., F_OK | X_OK)`
- that is not enough to distinguish shell-relevant cases

What to map:

1. slash-path command
2. PATH lookup command
3. existing directory
4. existing non-executable file
5. missing path
6. PATH unset / empty

### Suggested tasks

1. Review whether this belongs fully in `path.c` or partially in `external.c`.
2. Add classification helpers for:
   - resolved executable path
   - non-executable existing path
   - directory path
   - not found
3. Add tests for:
   - direct missing path
   - direct non-executable file
   - direct directory path

---

## 7. Confidence tests still missing

Write these tests after or while implementing the above.

### External execution semantics

1. direct missing file path
2. direct directory path
3. direct file without exec permission
4. PATH miss still returns `127`
5. execution failure path returns expected code/message class

### Heredoc behavior

1. multiple heredocs on one command
2. heredoc then file input redirection
3. file input redirection then heredoc
4. EOF before delimiter warning case

### Pipe + redirection confidence

1. multiple input redirections inside piped command
2. multiple output redirections inside piped command
3. heredoc + later input override inside pipe

### Structure / edge cases

1. nested `PIPE` execution if AST already supplies nested nodes
2. command with no argv and only redirections
3. redirection failure inside a pipeline branch

---

## Recommended Order

1. improve `rn_exec_ext` classification and messages
2. fix fd close discipline while touching external execution
3. implement multiple heredoc warning/completion behavior
4. add confidence tests for external/heredoc/path cases
5. add remaining pipeline/redirection edge tests

---

## Important Constraints For The Next Agent

1. Keep the current execution model:
   - `rn_execute` is the top entrypoint
   - `CMD` and `PIPE` are the only execution-driving node types
   - `ENV_ERRCODE` must still be finalized through `rn_execute`

2. Do not undo the current test split:
   - keep `tests/runner.c` as the aggregate entrypoint
   - keep focused suites under `tests/runner/`

3. Keep `make test` stable.

4. Prefer small helpers over growing existing functions beyond the current style.

5. Stay executor-only in this step.
