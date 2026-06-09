# Architecture

## VISAO GERAL

Minishell follows a classic pipeline: read → lex → parse → execute. Each stage is a separate module with its own types and headers. The shell state is one stack-allocated `t_shell` struct; everything else is passed down by pointer.

```
main()
  └─ core_run(envp)
       ├─ core_init()         allocate env, set $? = 0, set signals
       ├─ core_loop()         REPL
       │    ├─ readline()     read line, add_history()
       │    ├─ lexer()        char stream → token list
       │    ├─ parser_controller()  token list → AST
       │    └─ rn_execute()   walk AST, run commands
       └─ core_destroy()      free env
```

## Module map

| Module | Directory | Public header | Responsibility |
|---|---|---|---|
| core | `source/core/` | `headers/core.h` | REPL loop, init/destroy |
| shared | `source/shared/` | `headers/shared.h` | Errors, free helpers, signals, string utils, quote utils |
| env | `source/env/` | `headers/env.h` | Environment variable store (linked list) |
| lexer | `source/lexer/` | `headers/lexer.h` | Tokenisation: char stream → `t_manager` (doubly-linked token list) |
| parser | `source/parser/` | `headers/parser.h` | AST construction + syntax error detection |
| runner | `source/runner/` | `headers/runner.h` | Execution: expansion, redirections, pipes, builtins, external commands |
| bin | `source/bin/` | `headers/bin.h` | Builtin implementations |
| libft | `libraries/libft/` | `libft.h` | Custom libc: strings, memory, lists, printf, gnl |

## Key types

```
t_shell          { t_env *env; t_bool running; }
t_env            { char *key; char *value; t_env *next; }  ← singly-linked list
t_list_token     { char *value; t_token_type type; t_bool expand; t_bool quoted; }
t_manager        doubly-linked list of t_list_token nodes (libft t_dlist)
t_ast            { t_command *root; size_t size; int error; t_syntax_error error_type; }
t_command        { t_pnode_type type; union { t_simple simple; t_pipe pipe; }; }
t_simple         { t_array args; t_bool expand; t_array redirs; }
t_pipe           { t_command *left; t_command *right; }
t_parser_redir   { t_parser_redir_type type; char *file; t_bool expand; t_bool quoted; }
```

## Data flow for a single command

```
"echo hello | cat -n"
      │
      ▼  lexer()
[WORD:"echo"][WORD:"hello"][PIPE:"|"][WORD:"cat"][WORD:"-n"]
      │
      ▼  parser_controller()
      PIPE
     /    \
CMD(echo hello)  CMD(cat -n)
      │
      ▼  rn_execute()   →  rn_exec_pipe()  →  rn_pipe()
fork children, dup2 pipe fds, each child calls rn_execute() on its CMD node
```

## Global state

One global variable: `volatile sig_atomic_t g_signal` (defined in `source/shared/signal.c`). It only stores the signal number. Signal handlers write to it; the main loop reads and clears it. No other data lives in global scope.

## Header layering

```
headers/minishell.h   ← includes all public module headers + readline + signal
  └─ headers/bin.h
  └─ headers/env.h
  └─ headers/lexer.h
  └─ headers/parser.h
  └─ headers/runner.h

source/*/_.h          ← private to each module, includes its own public header
```

## Memory ownership

- `lexer()` allocates a `t_manager`; caller must call `lexer_free()`.
- `parser_controller()` allocates a `t_ast`; caller must call `parser_free_ast()`.
- `rn_expand()` returns a new `char **`; caller must call `sh_freeargs()`.
- `env_toarr()` returns a new `char **`; caller must call `sh_freeargs()`.
- `t_env` list lives for the entire shell session; freed in `core_destroy()`.
