# Minishell — Documentation Index

| File | Contents |
|---|---|
| [architecture.md](architecture.md) | Module map, data types, data flow diagram, memory ownership, header layering |
| [flow.md](flow.md) | End-to-end execution traces for simple commands, redirections, pipelines, and heredocs |
| [lexer.md](lexer.md) | Tokeniser: scanner, lookup tables, state machine, word/operator extraction, quote tracking |
| [parser.md](parser.md) | AST construction: grammar, node types, token classes, syntax errors |
| [runner.md](runner.md) | Execution: expander, redirections, pipes, external commands, builtin dispatch, exit status |
| [env.md](env.md) | Environment variable store: linked list API, special keys, SHLVL, toarr |
| [builtin.md](builtin.md) | All seven builtins: echo, cd, pwd, export, unset, env, exit |
| [shell.md](shell.md) | Shell behaviour: prompt, history, quotes, variable expansion, word splitting, signals, Ctrl-D |
| [functions.md](functions.md) | Every function in the project with signature and description |
