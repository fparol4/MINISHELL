# Shell Behaviour

This document covers behaviours that are not implementation details of a single module but reflect the shell's overall contract with the user. The reference for all open questions is bash.

---

## Prompt

```
GA&Fabricio:minishell$
```

Defined as the `PROMPT` macro in `headers/minishell.h`. Displayed by `readline()` on every iteration of the REPL loop.

---

## History

`add_history(line)` is called on every non-empty line before processing. History is stored in readline's internal buffer and navigable with arrow keys. Heredoc input is **not** added to history (it never reaches `add_history`; the heredoc reader uses raw `read()` instead of `readline()`).

---

## Quotes

### Single quotes `'`
Prevent all interpretation of metacharacters inside. The content is passed through verbatim. Dollar signs, spaces, pipes, and redirections inside single quotes are literal.

```
echo 'hello $NAME | cat'   →   hello $NAME | cat
```

### Double quotes `"`
Prevent interpretation of all metacharacters except `$`. Variables expand inside double quotes; the expanded value is **not** word-split.

```
NAME="a b"
echo "$NAME"   →   a b   (one argument)
echo $NAME     →   a b   (may be two arguments after splitting)
```

### Quote removal
Quotes are stripped during expansion (the expander, not the lexer). The lexer keeps raw values including quote characters. The expander processes them and never emits quotes in the final argument strings.

---

## Variable expansion

`$VAR` expands to the value of `VAR` in the environment. If `VAR` is not set, expands to empty string `""`.

Valid variable name: starts with alpha or `_`, followed by alphanumeric or `_` characters (`sh_varstart`, `sh_varchar`).

A lone `$` not followed by a valid name start, or `$` at end of string, is kept as a literal `$`.

`$?` expands to the exit status of the most recently completed foreground pipeline. Stored internally as `env["$?"]`.

Expansion happens in the runner (after parsing), not in the lexer. The lexer sets `token->expand = true` when `$` is present as a hint, but the actual expansion is always done by `rn_expand`.

---

## Word splitting

After variable expansion, unquoted values are split on whitespace (`' '`, `'\t'`, `'\n'`). Each whitespace-separated segment becomes a separate argument.

```
FILES="a b c"
echo $FILES      →  3 arguments: a  b  c
echo "$FILES"    →  1 argument:  a b c
```

Empty expansions in unquoted context are silently dropped (no empty argument is produced). Quoted empty strings (`""` or `''`) produce one empty-string argument.

---

## Redirections

All redirections are processed left-to-right. Multiple redirections of the same type: the last one wins (each `dup2` overwrites the previous fd).

| Syntax | Effect |
|---|---|
| `< file` | Open `file` for reading, redirect stdin |
| `> file` | Open `file` for writing (truncate), redirect stdout |
| `>> file` | Open `file` for writing (append), redirect stdout |
| `<< DELIM` | Read lines until `DELIM`, redirect stdin from in-memory pipe |

Redirection targets are variable-expanded. If the expansion produces zero or more than one word → `"ambiguous redirect"` error, status 1.

A quoted heredoc delimiter disables `$` expansion inside the heredoc body:
```
<< 'EOF'     →  no expansion inside
<< EOF       →  $VAR expands inside
```

---

## Pipelines

`cmd1 | cmd2 | cmd3` forks N children. Each child's stdout is connected to the next child's stdin via an OS pipe. All children run concurrently. The parent waits for all of them. The pipeline's exit status is the exit status of the rightmost command.

Builtins inside a pipeline run in child processes. Their environment changes (cd, export) do not propagate to the parent.

---

## Exit status

`$?` after a command:
- Builtin: the integer returned by `bin_X`.
- External: `WEXITSTATUS(status)` if the process exited normally.
- Killed by signal: `128 + signal_number`.
- Command not found: 127.
- Not executable / is a directory / wrong format: 126.
- Syntax error: 2.
- Empty line / no command: 0 (status unchanged? — `rn_status_set` is called with the return of `rn_execute` which returns 0 for no-op).

---

## Signals

Three signal modes, switched by `sh_sig_mode(t_sig_mode)`:

### `SIG_INTERACTIVE` (prompt)

| Signal | Handler |
|---|---|
| SIGINT (Ctrl-C) | Print newline, call `rl_on_new_line()` + `rl_replace_line("", 0)` + `rl_redisplay()`. Set `g_signal = SIGINT`. |
| SIGQUIT (Ctrl-\\) | Ignored (`SIG_IGN`). |

After readline returns, the main loop checks `g_signal == SIGINT` and sets `$?` to 130.

### `SIG_EXEC` (running an external command or pipe)

| Signal | Handler |
|---|---|
| SIGINT | Ignored by parent (`SIG_IGN`). Children restore default before `execve`. |
| SIGQUIT | Ignored by parent. Children restore default before `execve`. |

This means Ctrl-C kills the running foreground child but does not affect the parent shell process. The parent learns of the signal through `waitpid` status (`WIFSIGNALED`) and sets `$? = 128 + SIGINT = 130`.

### `SIG_HEREDOC` (reading heredoc input)

| Signal | Handler |
|---|---|
| SIGINT | Print newline. Set `g_signal = SIGINT`. |
| SIGQUIT | Ignored. |

After the heredoc read loop ends, `g_signal == SIGINT` causes the heredoc to return `HEREDOC_INTR`, which causes the redir to return status 130 and the command is not executed.

### `g_signal`

```c
volatile sig_atomic_t g_signal = 0;
```

Defined once in `source/shared/signal.c`. Used across the REPL loop to communicate between signal handlers and the main loop. Always reset to 0 after being consumed. This is the **only** global variable in the project.

---

## Ctrl-D (EOF)

When `readline()` returns `NULL` without `g_signal == SIGINT`, it is EOF (Ctrl-D). The shell prints `"exit\n"` and terminates by returning from `core_loop` with the current `$?` value.

---

## Not implemented

The following are intentionally absent (outside the mandatory subject scope):

- `\` (backslash escape)
- `;` (command separator)
- `&&` and `||` (logical operators)
- `*` wildcards
- Subshells `( )`
- Background execution `&`
- `~` tilde expansion
