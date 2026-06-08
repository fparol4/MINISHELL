# Execution Flow

End-to-end walkthrough of what happens from the moment the user presses Enter.

---

## 1. REPL iteration (`core/reader.c — core_iteration`)

```
readline(PROMPT)
  ├─ returns NULL (Ctrl-D)  →  print "exit\n", return -1 → shell exits
  ├─ g_signal == SIGINT     →  set $? = 130, clear g_signal, return 1 (loop again)
  └─ non-NULL line
       ├─ add_history(line)   if non-empty
       ├─ core_process_line(shell, line)
       └─ free(line)
```

---

## 2. Parse phase (`core_process_line`)

```
core_parse(line)
  ├─ lexer(line)               → t_manager * (or NULL on unclosed quote)
  └─ parser_controller(manager) → t_ast *
       lexer_free(manager)
```

If `lexer()` returns `NULL` → already printed error → set `$? = 2`, done.  
If `ast->error` → print syntax error, set `$? = 2`, free ast, done.

---

## 3. Execution (`rn_execute`)

```
rn_execute(ast->root, &shell->env)
```

### Case A: Simple command `echo hello`

```
rn_exec_cmd(cmd, env)
  │
  ├─ rn_expand(["echo", "hello", NULL], env)
  │    → ["echo", "hello", NULL]   (no $ in args, trivially copies)
  │
  ├─ no redirections → skip redir_push
  │
  ├─ rn_cmd_run(["echo", "hello"], env)
  │    → rn_exec_bin() matches "echo"
  │    → bin_echo(["hello"], env)
  │         ft_putstr_fd("hello", 1)
  │         ft_putchar_fd('\n', 1)
  │         return 0
  │
  └─ rn_status_set(env, 0)   →  $? = 0
```

### Case B: Command with redirection `echo hi > out.txt`

```
rn_exec_cmd(cmd, env)
  │
  ├─ rn_expand(["echo", "hi"], env)  →  ["echo", "hi"]
  │
  ├─ rn_redir_push([{REDIR_OUT, "out.txt"}], 1, env, saved)
  │    ├─ saved[0] = dup(STDIN)    ← save stdin (even if unused)
  │    ├─ saved[1] = dup(STDOUT)   ← save stdout
  │    └─ rn_redir_apply()
  │         rn_redir_fd({REDIR_OUT, "out.txt"})
  │           rn_redir_target() → expand("out.txt") → "out.txt"
  │           open("out.txt", O_WRONLY|O_CREAT|O_TRUNC, 0644)  → fd=5
  │         dup2(5, STDOUT_FILENO)
  │         close(5)
  │
  ├─ rn_cmd_run(["echo", "hi"], env)
  │    bin_echo → writes "hi\n" to fd 1 (now out.txt)
  │
  ├─ sh_freeargs(args)
  │
  └─ rn_redir_restore(saved)
       dup2(saved[1], STDOUT_FILENO)   ← restore stdout
       close(saved[0]), close(saved[1])
```

### Case C: Pipeline `ls | grep foo`

```
rn_exec_pipe(cmd, env)  →  rn_pipe(cmd, env)
  │
  ├─ rn_pipe_count(node) = 2
  ├─ rn_pipe_flatten → cmds = [CMD(ls), CMD(grep foo)]
  ├─ rn_pipe_create(fds, 1)  →  pipe(&fds[0])  →  fds=[r,w]
  │
  └─ rn_pipe_fork_wait(cmds, env, fds, 2)
       sh_sig_mode(SIG_EXEC)
       │
       ├─ fork() → child 0 (ls):
       │    dup2(fds[1], STDOUT)   ← write end of pipe
       │    rn_pipe_close_all()
       │    signal(SIGINT/SIGQUIT, SIG_DFL)
       │    rn_execute(CMD(ls), env)
       │      rn_exec_cmd → rn_exec_ext → execve("/bin/ls", ...)
       │
       ├─ fork() → child 1 (grep foo):
       │    dup2(fds[0], STDIN)    ← read end of pipe
       │    rn_pipe_close_all()
       │    signal(SIGINT/SIGQUIT, SIG_DFL)
       │    rn_execute(CMD(grep foo), env)
       │      rn_exec_cmd → rn_exec_ext → execve("/bin/grep", ...)
       │
       ├─ rn_pipe_close_all(fds, 1)   ← parent closes both ends
       ├─ rn_pipe_wait(pids, 2)       ← waitpid both, return last status
       └─ sh_sig_mode(SIG_INTERACTIVE)
```

### Case D: Heredoc `cat << EOF`

```
rn_exec_cmd(cmd, env)
  │
  ├─ rn_expand(["cat"], env) → ["cat"]
  │
  ├─ rn_redir_push([{REDIR_HEREDOC, "EOF", expand=true}], 1, env, saved)
  │    saved[0] = dup(STDIN), saved[1] = dup(STDOUT)
  │    rn_redir_apply()
  │      rn_redir_fd({REDIR_HEREDOC, "EOF"})
  │        rn_redir_target() → sh_quote_remove("EOF") → "EOF"
  │        rn_redir_heredoc("EOF", env, expand=true)
  │          pipe(pfd)                ← pfd[0]=read, pfd[1]=write
  │          sh_sig_mode(SIG_HEREDOC)
  │          heredoc_read_loop(pfd[1], "EOF", env, true)
  │            "> " → read line "hello $NAME\n"
  │            rn_redir_line → expand → "hello World\n" → write(pfd[1])
  │            "> " → read line "EOF\n"
  │            rn_redir_delim("EOF", "EOF") = true → HEREDOC_DONE
  │          sh_sig_mode(SIG_INTERACTIVE)
  │          close(pfd[1])
  │          return pfd[0]           ← fd for read end of pipe
  │        dup2(pfd[0], STDIN_FILENO)
  │        close(pfd[0])
  │
  ├─ rn_cmd_run(["cat"], env)
  │    rn_exec_ext → fork → execve("/bin/cat", ...)
  │      cat reads from stdin (which is now the pipe's read end)
  │      outputs "hello World\n"
  │
  └─ rn_redir_restore(saved) → restore stdin
```

---

## 4. Status update

After `rn_execute` returns, `rn_status_set(env, status)` writes the result into `env["$?"]`. The next iteration of the REPL can then read it via `$?` expansion.

---

## 5. Memory cleanup per iteration

```
core_process_line returns
  parser_free_ast(ast)
    parser_free_cmd(root)     ← recursive tree walk
      ft_array_free(args)
      ft_array_free(redirs)   ← frees t_parser_redir.file strings too
      free(cmd)
    free(ast)
free(line)                    ← readline-allocated string
```

The `t_env` list persists across iterations. `g_signal` is reset to 0 after each use.
