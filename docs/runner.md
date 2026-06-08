# Runner

**Entry point:** `rn_execute(t_command *cmd, t_env **env)` → `int` (exit status)  
**Files:** `source/runner/`

## Dispatch (`control.c`)

```
rn_execute(cmd, env)
  ├─ PNODE_CMD   → rn_exec_cmd()
  ├─ PNODE_PIPE  → rn_exec_pipe() → rn_pipe()
  └─ rn_status_set(env, status)   ← always updates $?
```

### `rn_exec_cmd` — single command

```
1. rn_expand(simple->args, env)   ← variable/quote expansion → char **args
2. rn_redir_push(simple->redirs)  ← save stdio, open + dup2 each redir
   (on failure: restore stdio, return error status)
3. rn_cmd_run(args, env)
   ├─ rn_exec_bin()  if first arg matches a builtin name
   └─ rn_exec_ext()  otherwise
4. sh_freeargs(args)
5. rn_redir_restore()             ← restore saved stdio fds
```

Redirections are applied **after** argument expansion but **before** running the command. The saved stdin/stdout fds are always restored in the parent shell process; this is what allows `cd`, `export`, etc. to redirect without permanently altering the shell's file descriptors.

---

## Expander (`source/runner/expander/`)

**Entry:** `rn_expand(char **args, t_env **env)` → `char **`

The expander processes every string in the args array and may produce more or fewer strings due to word splitting.

### Data structures

```c
t_arglist  { char **items; int size; int cap; }   // growing output array
t_word     { char *buf; int len; int cap; int active; }  // current word being assembled
t_exp_ctx  { t_arglist *list; t_word *word; char *arg; int *i; t_env **env; }
```

### Per-argument processing (`rn_argexpand`)

Walks the raw arg character by character via `exp_arg_char(ctx)`:

| Character | Action |
|---|---|
| `'` | `exp_single()` — copy everything literally until matching `'`, set `word.active = 1` |
| `"` | `exp_double()` — same but `$` triggers `exp_var_quoted()` |
| `$` | `exp_var()` — expand variable, pass result to `exp_add_unquoted()` |
| space/tab/newline | `exp_flush_word()` — word splitting: emit current word to list |
| other | `exp_wordchar()` — append literal byte to word buffer |

At end of arg: `exp_flush_word()` emits any remaining word.

### Variable expansion (`variable.c`)

`exp_varvalue(arg, i, env)`:
1. Advance past `$`.
2. If next char is `?` → return `env_get(env, ENV_ERRCODE)` (the `$?` key is the literal string `"$?"`).
3. If next char is not a valid identifier start (`sh_varstart`) → return `NULL` (dollar is kept literal).
4. Read identifier chars with `sh_varchar`, extract key with `ft_substr`.
5. `env_get(env, key)` — if not found, return `""`.

**Inside double quotes** (`exp_var_quoted`): the value is appended to the current word buffer directly with `exp_wordstr()` — no word splitting.  
**Outside quotes** (`exp_var`): the value goes through `exp_add_unquoted()` — spaces in the value cause word splits.

### Word splitting (`list.c — exp_add_unquoted`)

Walks the expanded value. Each run of whitespace flushes the current word to the list and skips the spaces; non-space characters are appended to the word buffer. This is how `"$X"` stays one argument even when `X = "a b"` (because the double-quote path uses `exp_wordstr` not `exp_add_unquoted`).

### Empty-expansion removal

`exp_flush_word` only adds to the list when `word.active == 1`. The `active` flag is only set when at least one character has been written or a quote was opened. This means an unquoted variable that expands to empty is dropped entirely. A `''` or `""` sets `active = 1` immediately, so it survives as an empty string argument.

### Output

After all args are processed, `exp_listgrow` ensures one extra slot, then `NULL` is written to terminate the array. The caller receives a `NULL`-terminated `char **` and owns it (must call `sh_freeargs`).

---

## Redirections (`source/runner/redir/`)

### Overview

```
rn_redir_push(redirs, count, env, saved)
  ├─ redir_save_stdio(saved)       ← dup(STDIN), dup(STDOUT) into saved[0]/saved[1]
  └─ rn_redir_apply(redirs, count, env)
       ├─ redir_open_all()          ← open/create each redir's fd
       └─ redir_dup_all()           ← dup2 each fd to stdin or stdout, close original

rn_redir_restore(saved)
  ├─ dup2(saved[0], STDIN_FILENO)
  └─ dup2(saved[1], STDOUT_FILENO)
```

### Target resolution (`target.c — rn_redir_target`)

- **Heredoc**: strip quotes from delimiter with `sh_quote_remove()`. No expansion.
- **Other types**: run `rn_expand([redir->file, NULL], env)`. The result must be exactly one word; zero or multiple words → `"ambiguous redirect"` error.

### File opening (`target.c — rn_redir_open`)

| Type | Flags |
|---|---|
| `REDIR_IN` | `O_RDONLY` |
| `REDIR_OUT` | `O_WRONLY \| O_CREAT \| O_TRUNC`, mode 0644 |
| `REDIR_APPEND` | `O_WRONLY \| O_CREAT \| O_APPEND`, mode 0644 |

### Heredoc (`heredoc.c`, `heredoc_read.c`, `buffer.c`)

```
rn_redir_heredoc(target, env, expand)
  ├─ pipe(pfd)                    ← create in-memory pipe
  ├─ rn_redir_heredoc_fill()
  │    ├─ sh_sig_mode(SIG_HEREDOC) ← Ctrl-C sets g_signal, writes newline
  │    └─ heredoc_read_loop(pfd[1], target, env, expand)
  │         while readline() != target:
  │           rn_redir_line(line, env, expand) → write to pfd[1]
  ├─ close(pfd[1])
  └─ return pfd[0]                ← read-end becomes the redir fd
```

`rn_redir_readline()` reads one line from stdin one byte at a time (using a growing buffer), printing `"> "` prompt to stderr.

`rn_redir_line(line, env, expand)`:
- `expand = true`: walk the line, expand `$VAR` and `$?` inline → `rn_redir_value()`.
- `expand = false` (quoted delimiter): `ft_strdup(line)`.
- Always appends `"\n"` to the result.

Heredoc states: `HEREDOC_DONE` (delimiter matched), `HEREDOC_EOF` (stdin ended without delimiter — prints warning), `HEREDOC_INTR` (Ctrl-C), `HEREDOC_FAIL` (alloc/write error).

### stdio direction

`redir_stdio(type)` returns `STDIN_FILENO` for `REDIR_IN` and `REDIR_HEREDOC`, `STDOUT_FILENO` for `REDIR_OUT` and `REDIR_APPEND`. This determines which fd `dup2` targets.

Multiple redirections of the same type (e.g., `> a > b`) are all opened in order; each `dup2` replaces the previous one, so the last one wins — matching bash behaviour.

---

## Pipes (`source/runner/pipe/`)

### Overview

```
rn_pipe(node, env)
  ├─ rn_pipe_count(node)    ← count leaf CMD nodes in the pipe tree
  ├─ rn_pipe_flatten(node, cmds, idx)  ← left-to-right array of CMD nodes
  ├─ rn_pipe_create(fds, count-1)      ← create N-1 OS pipes
  └─ rn_pipe_fork_wait(cmds, env, fds, count)
       ├─ sh_sig_mode(SIG_EXEC)         ← parent ignores SIGINT
       ├─ rn_pipe_fork_all()
       │    for each cmd[i]:
       │      fork()
       │      child → rn_pipe_child(ctx, i)
       │        ├─ dup2(fds[(i-1)*2], STDIN)   if not first
       │        ├─ dup2(fds[i*2+1], STDOUT)    if not last
       │        ├─ rn_pipe_close_all()
       │        ├─ signal(SIGINT/SIGQUIT, SIG_DFL)
       │        └─ _exit(rn_execute(cmds[i], env))
       ├─ rn_pipe_close_all()  ← parent closes all pipe fds
       ├─ rn_pipe_wait()       ← waitpid all children, return last status
       └─ sh_sig_mode(SIG_INTERACTIVE)
```

### Pipe fd layout

For N commands there are N-1 OS pipes. Each pipe produces two fds: `fds[i*2]` (read end) and `fds[i*2+1]` (write end).

Command `i` reads from `fds[(i-1)*2]` (previous pipe's read end) and writes to `fds[i*2+1]` (current pipe's write end). First command has no read-pipe; last command has no write-pipe.

### Tree flattening (`tree.c`)

The parser builds a left-leaning binary tree. `rn_pipe_count` counts leaves recursively; `rn_pipe_flatten` does an in-order (left, right) traversal to fill a `t_command *[]` array in left-to-right pipeline order.

### Status

`rn_pipe_wait` waits for all children in order and returns the status of the **last** child. `WIFEXITED` → `WEXITSTATUS`; `WIFSIGNALED` → `128 + signal_number`.

---

## External commands (`source/runner/external/`)

```
rn_exec_ext(args, env)
  ├─ rn_ext_resolve(args, env, &path)
  │    ├─ if '/' in args[0] → use as-is (absolute/relative)
  │    └─ else rn_path(args, env) → search PATH dirs
  ├─ env_toarr(env) → envp for execve
  ├─ sh_sig_mode(SIG_EXEC)
  ├─ fork()
  │    child → rn_ext_child(path, args, envp)
  │      ├─ signal(SIGINT/SIGQUIT, SIG_DFL)
  │      └─ execve(path, args, envp)
  │         on fail: rn_ext_execfail() → _exit()
  └─ rn_ext_wait(pid)
       ├─ waitpid
       └─ sh_sig_mode(SIG_INTERACTIVE)
```

### Path resolution (`source/runner/path/`)

`rn_path(args, env)`:
1. If `args[0]` contains `/` → `ft_strdup(args[0])` (treat as direct path).
2. Otherwise: `env_get(env, "PATH")` → `ft_split(path, ':')` → iterate dirs.
3. For each dir: `rn_path_join(dir, cmd)` → check with `rn_path_candidate()`.
4. `rn_path_candidate` tries `rn_path_match` (stat + `X_OK`). If not executable but exists, it stores it as `fallback` (used later by `rn_ext_classify` to distinguish "not found" from "found but not executable").

### Error exit codes

| Situation | Code |
|---|---|
| Command not found (PATH search failed) | 127 |
| Direct path not found | 127 |
| Is a directory | 126 |
| Permission denied | 126 |
| Exec format error (e.g. plain text file) | 126 |

---

## Builtin dispatch (`builtin.c`)

`rn_exec_bin(args, env, *status)` checks `args[0]` with `ft_strcmp` against each builtin name in order:

```
echo → bin_echo
cd   → bin_cd
pwd  → bin_pwd
export → bin_export
unset  → bin_unset
env    → bin_env
exit   → bin_exit
```

Returns 1 if a builtin was matched (sets `*status`), 0 otherwise.

Builtins run in the **current process**, not a child. This is necessary for `cd`, `export`, `unset`, and `exit` to affect the shell's own state. When a builtin is in a pipeline, it runs in a child process (via `rn_pipe_child` → `rn_execute`), but environment changes in that child are not visible to the parent.

---

## Exit status (`status.c`)

`rn_status_set(env, status)`: converts `status` to string, calls `env_set(env, "$?", value)`.  
`rn_status_get(env)`: `env_get(env, "$?")` → `ft_atoi`.

The key `"$?"` is the literal string stored as an env entry — it is never emitted by `env_toarr` or `env` because `toarr` skips entries whose key equals `ENV_ERRCODE`.
