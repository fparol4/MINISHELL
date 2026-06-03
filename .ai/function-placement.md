# Function Placement Map

This is a classification of functions in `source/` that could reasonably move
into `shared/` or into `libft`. It is not a rewrite plan yet, just a placement
map for review.

## Good `libft` Candidates

These are generic helpers with no minishell-specific policy.

- `source/shared/free.c`
  - `sh_freesec`
  - `sh_freeargs`
  - Reason: general array/free helpers.

- `source/shared/string.c`
  - `sh_isspace`
  - `sh_varstart`
  - `sh_varchar`
  - `sh_isidentifier`
  - `sh_has_quotes`
  - `sh_quote_remove`
  - Reason: mostly string predicates and transformations.

- `source/modules/parser/dynarray.c`
  - `dynarray_append`
  - Reason: generic grow-and-copy array helper.

- `source/modules/lexer/scanner/utils.c`
  - `scanner_advance`
  - `scanner_current`
  - `scanner_peek`
  - `scanner_is_end`
  - `scanner_mark_start`
  - `scanner_take_while`
  - `scanner_extract`
  - Reason: generic cursor/scanner primitives, if you want a reusable scanning helper layer.

- `source/modules/runner/path.c`
  - `rn_path_join`
  - `rn_path_match`
  - `rn_path_free`
  - Reason: `rn_path_join` and `rn_path_free` are generic string/array helpers; `rn_path_match` is filesystem-specific so it is less libft-like and more shell-specific.

- `source/modules/runner/expander.c`
  - `listgrow`
  - `listadd`
  - `wordgrow`
  - `wordchar`
  - `wordstr`
  - Reason: reusable dynamic buffer/list growth helpers.

## Good `shared` Candidates

These are cross-module minishell helpers, but not generic enough for `libft`.

- `source/shared/error.c`
  - `sh_err`
  - `sh_err2`
  - `sh_stxerr`
  - Reason: shell-specific error reporting.

- `source/shared/signal.c`
  - `sh_sig_mode`
  - Reason: process-wide shell signal policy.

- `source/modules/envm/common.c`
  - `f_newnode`
  - `f_freenode`
  - Reason: environment node management is used across env modules and can stay as a shared env helper, though not `libft`.

- `source/modules/envm/get.c`
  - `env_get`
  - Reason: shared env lookup primitive.

- `source/modules/envm/size.c`
  - `env_size`
  - Reason: shared env size helper.

- `source/modules/envm/free.c`
  - `env_free`
  - Reason: shared environment teardown.

- `source/modules/envm/set.c`
  - `env_set`
  - Reason: shared environment mutation.

- `source/modules/envm/unset.c`
  - `env_unset`
  - Reason: shared environment mutation.

- `source/modules/envm/toarr.c`
  - `env_toarr`
  - Reason: shared environment export conversion.

- `source/modules/envm/init.c`
  - `env_init`
  - Reason: shared environment bootstrap.

## Keep In Module

These are tied to shell behavior, parsing, execution, or the current module’s
policy, so they should stay where they are.

- `source/core/core.c`
  - `core_init`
  - `core_destroy`
  - `core_run`

- `source/core/reader.c`
  - `core_process_line`
  - `core_loop`

- `source/modules/builtin/*`
  - `bin_cd`
  - `bin_echo`
  - `bin_env`
  - `bin_exit`
  - `bin_export`
  - `bin_pwd`
  - `bin_unset`
  - Helpers inside these files are builtin-specific.

- `source/modules/lexer/*`
  - `lexer_control`
  - `init_lexer_variable`
  - `define_rules`
  - `update_quote_state`
  - `add_token_to_list`
  - `lexer_free`
  - `get_word`
  - `get_operator`
  - `define_type`
  - `init_token_table`
  - lexer file-local helpers stay in lexer.

- `source/modules/parser/*`
  - `parser_controller`
  - `parser_free_ast`
  - `parser_free_command`
  - `parser_free_simple`
  - `parse_simple_command`
  - `parser_set_syntax_error`
  - `parser_start_command`
  - `parser_finish_command`
  - `parser_add_arg`
  - `parser_add_redir`
  - `parser_init`
  - `parser_next`
  - `parser_is_end`
  - `token_class`
  - `get_token`
  - `dynarray_append` is the only parser helper worth extracting.

- `source/modules/runner/*`
  - `rn_exec_bin`
  - `rn_exec_cmd`
  - `rn_exec_pipe`
  - `rn_execute`
  - `rn_expand`
  - `rn_exec_ext`
  - `rn_path`
  - `rn_pipe`
  - `rn_redir_push`
  - `rn_redir_restore`
  - `rn_status_set`
  - `rn_status_get`
  - These are shell execution policy and should stay in runner.

## Notes

- The strongest `libft` wins are generic helpers with no dependency on shell
  policy: `sh_freeargs`, `sh_isspace`, `sh_quote_remove`, `dynarray_append`,
  and the scanner/buffer helpers.
- The strongest `shared` wins are environment lifecycle and shell error/signal
  helpers.
- Exported `envm` APIs are already shared by usage, but they are minishell
  domain functions, not `libft` functions.
