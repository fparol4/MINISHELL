# Migration Conclusion

## Status: Complete

All lexer/parser modules are ported from `main` into `dev/merge`. The full pipeline
`readline → scanner → lexer → parser → runner` is wired with no intermediate
conversion layer — the runner consumes `t_ast` / `t_command` directly.

---

## What was ported / implemented

| Module | Files | Notes |
|--------|-------|-------|
| scanner | `lexer/scanner/utils.c` | Unchanged from main |
| lexer | `dlist.c`, `extract.c`, `lexer.c`, `lexer_init.c`, `lexer_rules.c`, `quote_state.c`, `token.c`, `token_table.c`, `token_type.c` | Split from main's 3-file layout into 9 focused files |
| parser | `parser.c`, `parser_command.c`, `parser_cleanup.c`, `parser_errors.c`, `parser_state.c`, `parser_utils.c`, `dynarray.c` | Adapted from main's `parser_controller.c` |
| shared | `error.c` (updated), `signal.c` (updated) | `print_syntax_error` moved here; readline reset on SIGINT |
| core | `loop.c` (updated) | Calls lexer → parser → runner directly via `ast->root`; handles SIGINT/EOF |
| runner | `control.c`, `redir.c`, `pipe.c` (updated) | Accept `t_command *` / `t_parser_redir *` directly |

---

## Type renames (conflict resolution)

Main branch types clashed with executor types that were in `minishell.h`:

| Old name | dev/merge name | Reason |
|----------|---------------|--------|
| `t_redir` (executor linked list) | removed | Runner uses `t_parser_redir[]` array directly |
| `t_exnode` | removed | Runner uses `t_command *` directly |
| `t_node_type` (CMD/PIPE) | removed | Runner uses `t_pnode_type` (PNODE_CMD/PNODE_PIPE) |
| `t_redir_type` (IN_F/IN_H/OUT_T/OUT_A) | removed | Runner uses `t_parser_redir_type` (REDIR_IN/OUT/APPEND/HEREDOC) |

All four executor-side types removed from `minishell.h`. The runner includes
`parser_internal.h` and works with parser types end-to-end.

---

## Direct binding: loop → runner

```
readline → scanner_init → lexer_control → parser_controller → rn_execute(ast->root) → parser_free_ast
```

No conversion step. `rn_execute(t_command *cmd, t_env **env)` is the runner entry.

---

## Signal handling

| Signal | Interactive | Exec (pipe) | Heredoc |
|--------|-------------|-------------|---------|
| SIGINT | handler: `\n` + rl_on_new_line + rl_replace_line → core_loop continues | ignored by parent, SIG_DFL in child | handler: `\n` + g_signal → heredoc loop exits |
| SIGQUIT | ignored | ignored | ignored |

`core_loop` differentiates SIGINT (g_signal set, continue loop) from EOF/Ctrl-D (g_signal unset, print "exit" and break).

---

## Renamed

`source/modules/bultin/` → `source/modules/builtin/`

---

## Test results

```
make t:lexer   → 14/14 ✓
make t:parser  → 17/17 ✓
make all       → links clean ✓
```

---

## Remaining edge cases (not blocking)

- **Expansion with redir targets**: `rn_redir_target()` calls `rn_expand()` on the file field. Ambiguous redirects (word-split to multiple tokens) produce an error message. Single-quoted redir targets correctly suppress `$VAR` expansion via the expander's quote-aware scan.
- **Heredoc delimiter quoting**: `cat << 'EOF'` should suppress expansion inside the heredoc. The `expand` flag on `t_parser_redir` is set by the lexer (false when delimiter is quoted), and `rn_redir_heredoc_fill` reads it via the `expand` arg — already wired correctly.
