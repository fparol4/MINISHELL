# Tasks de Refatoração

---

## Etapa 1 — Arquivos com mais de 5 funções

- [ ] `source/modules/runner/expander.c` (14 funções)
- [ ] `source/modules/runner/redir.c` (17 funções)
- [ ] `source/modules/runner/pipe.c` (9 funções)
- [ ] `source/modules/runner/external.c` (8 funções)
- [ ] `source/shared/string.c` (6 funções)

---

## Etapa 2 — Funções com mais de 25 linhas (projeto todo)

### a) `source/modules/runner/expander.c`

- [x] a.1 — `varvalue` (28 linhas)
- [x] a.2 — `rn_argexpand` (38 linhas)

### b) `source/modules/runner/redir.c`

- [x] b.1 — `rn_redir_value` (27 linhas)
- [x] b.2 — `rn_redir_readline` (29 linhas)
- [x] b.3 — `rn_redir_heredoc_fill` (36 linhas)
- [x] b.4 — `rn_redir_apply` (45 linhas)

### c) `source/modules/runner/pipe.c`

- [x] c.1 — `rn_pipe` (37 linhas)

### d) `source/modules/runner/external.c`

*(Nenhuma função com mais de 25 linhas)*

### e) `source/shared/string.c`

- [x] e.1 — `sh_quote_remove` (26 linhas)

### f) `source/core/reader.c`

- [ ] f.1 — `core_loop` (30 linhas)

### g) `source/shared/signal.c`

- [ ] g.1 — `sh_sig_mode` (26 linhas)

### h) `source/modules/env/init.c`

- [ ] h.1 — `env_init` (29 linhas)

### i) `source/modules/env/set.c`

- [ ] i.1 — `env_set` (33 linhas)

### j) `source/modules/env/toarr.c`

- [ ] j.1 — `env_toarr` (27 linhas)

### k) `source/modules/bin/echo.c`

- [ ] k.1 — `bin_echo` (28 linhas)

### l) `source/modules/bin/exit.c`

- [ ] l.1 — `exit_value` (37 linhas)

### m) `source/modules/bin/export.c`

- [ ] m.1 — `getkeys` (29 linhas)
- [ ] m.2 — `print_export` (29 linhas)
- [ ] m.3 — `bin_export` (36 linhas)

### n) `source/modules/lexer/scanner/table.c`

- [ ] n.1 — `scanner_table_char` (26 linhas)

### o) `source/modules/parser/parser.c`

- [ ] o.1 — `build_pipeline` (26 linhas)

### p) `source/modules/parser/parser_command.c`

- [ ] p.1 — `parse_simple_command` (26 linhas)

### q) `source/modules/parser/parser_state.c`

- [ ] q.1 — `parser_add_arg` (29 linhas)
- [ ] q.2 — `parser_add_redir` (33 linhas)

### r) `source/modules/runner/control.c`

- [ ] r.1 — `rn_exec_cmd` (29 linhas)

### s) `source/modules/runner/path.c`

- [ ] s.1 — `rn_path_search` (32 linhas)
