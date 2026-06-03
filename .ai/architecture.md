# Source Architecture Review

WIP inventory for reviewing folder structure and function names module by module.
This file lists the current `source/` structure and the functions defined in each
file. `static` means file-local helper.

## Folder Structure

```text
source/
├── main.c
├── core/
│   ├── core.c
│   └── reader.c
├── shared/
│   ├── error.c
│   ├── free.c
│   ├── general.c
│   ├── signal.c
│   └── string.c
└── modules/
    ├── builtin/
    │   ├── cd.c
    │   ├── echo.c
    │   ├── env.c
    │   ├── exit.c
    │   ├── export.c
    │   ├── pwd.c
    │   └── unset.c
    ├── envm/
    │   ├── common.c
    │   ├── free.c
    │   ├── get.c
    │   ├── init.c
    │   ├── set.c
    │   ├── size.c
    │   ├── toarr.c
    │   └── unset.c
    ├── lexer/
    │   ├── extract.c
    │   ├── lexer.c
    │   ├── lexer_init.c
    │   ├── lexer_rules.c
    │   ├── quote_state.c
    │   ├── scanner/
    │   │   └── utils.c
    │   ├── token.c
    │   ├── token_table.c
    │   └── token_type.c
    ├── parser/
    │   ├── dynarray.c
    │   ├── parser.c
    │   ├── parser_cleanup.c
    │   ├── parser_command.c
    │   ├── parser_errors.c
    │   ├── parser_state.c
    │   └── parser_utils.c
    └── runner/
        ├── builtin.c
        ├── control.c
        ├── expander.c
        ├── external.c
        ├── path.c
        ├── pipe.c
        ├── redir.c
        └── status.c
```

## Entry Point

- `source/main.c:5`
  - `main`

## Core Module

- `source/core/core.c`
  - `core_init`
  - `core_destroy`
  - `core_run`

- `source/core/reader.c`
  - `core_process_line` static
  - `core_loop`

## Shared Module

- `source/shared/error.c`
  - `sh_err`
  - `sh_err2`
  - `sh_stxerr`

- `source/shared/free.c`
  - `sh_freesec`
  - `sh_freeargs`

- `source/shared/general.c`
  - No function definitions currently detected.

- `source/shared/signal.c`
  - `sh_sigint_interactive` static
  - `sh_sigint_heredoc` static
  - `sh_sig_mode`

- `source/shared/string.c`
  - `sh_isspace`
  - `sh_varstart`
  - `sh_varchar`
  - `sh_isidentifier`
  - `sh_has_quotes`
  - `sh_quote_remove`

## Builtin Module

- `source/modules/builtin/cd.c`
  - `getpath` static
  - `bin_cd`

- `source/modules/builtin/echo.c`
  - `newline` static
  - `bin_echo`

- `source/modules/builtin/env.c`
  - `bin_env`

- `source/modules/builtin/exit.c`
  - `exit_value` static
  - `bin_exit`

- `source/modules/builtin/export.c`
  - `vid` static
  - `sortkeys` static
  - `getkeys` static
  - `printsize` static
  - `print_export` static
  - `bin_export`

- `source/modules/builtin/pwd.c`
  - `bin_pwd`

- `source/modules/builtin/unset.c`
  - `bin_unset`

## Env Manager Module

- `source/modules/envm/common.c`
  - `f_newnode`
  - `f_freenode`

- `source/modules/envm/free.c`
  - `env_free`

- `source/modules/envm/get.c`
  - `env_get`

- `source/modules/envm/init.c`
  - `shlvl` static
  - `entryparse` static
  - `minenv` static
  - `env_init`

- `source/modules/envm/set.c`
  - `env_set`

- `source/modules/envm/size.c`
  - `env_size`

- `source/modules/envm/toarr.c`
  - `toentry` static
  - `envsize` static
  - `env_toarr`

- `source/modules/envm/unset.c`
  - `env_unset`

## Lexer Module

- `source/modules/lexer/extract.c`
  - `get_word`
  - `get_operator`

- `source/modules/lexer/lexer.c`
  - `ft_state_machine` static
  - `lexer_control`

- `source/modules/lexer/lexer_init.c`
  - `init_lexer_variable`

- `source/modules/lexer/lexer_rules.c`
  - `define_rules`

- `source/modules/lexer/quote_state.c`
  - `update_quote_state`

- `source/modules/lexer/scanner/utils.c`
  - `scanner_init`
  - `scanner_advance`
  - `scanner_current`
  - `scanner_peek`
  - `scanner_is_end`
  - `scanner_mark_start`
  - `scanner_extract`
  - `scanner_take_while`
  - `init_char_table`

- `source/modules/lexer/token.c`
  - `free_token` static
  - `lexer_free`
  - `add_token_to_list`

- `source/modules/lexer/token_table.c`
  - `init_token_table`

- `source/modules/lexer/token_type.c`
  - `define_type`

## Parser Module

- `source/modules/parser/dynarray.c`
  - `dynarray_append`

- `source/modules/parser/parser.c`
  - `parser_controller`
  - `build_pipeline` static
  - `build_pipe_node` static

- `source/modules/parser/parser_cleanup.c`
  - `parser_free_ast`
  - `parser_free_command`
  - `parser_free_simple`
  - `free_simple_data` static

- `source/modules/parser/parser_command.c`
  - `parse_simple_command`
  - `simple_to_command` static
  - `consume_word` static
  - `consume_redir` static

- `source/modules/parser/parser_errors.c`
  - `parser_set_syntax_error`

- `source/modules/parser/parser_state.c`
  - `str_array_len` static
  - `parser_start_command`
  - `parser_finish_command`
  - `parser_add_arg`
  - `parser_add_redir`

- `source/modules/parser/parser_utils.c`
  - `parser_init`
  - `parser_next`
  - `parser_is_end`
  - `token_class`
  - `get_token`

## Runner Module

- `source/modules/runner/builtin.c`
  - `rn_exec_bin`

- `source/modules/runner/control.c`
  - `rn_exec_cmd`
  - `rn_exec_pipe`
  - `rn_execute`

- `source/modules/runner/expander.c`
  - `listgrow` static
  - `listadd` static
  - `wordgrow` static
  - `wordchar` static
  - `wordstr` static
  - `flush_word` static
  - `add_unquoted` static
  - `varvalue` static
  - `expand_var` static
  - `expand_quoted` static
  - `single` static
  - `double_quote` static
  - `rn_argexpand` static
  - `rn_expand`

- `source/modules/runner/external.c`
  - `rn_ext_slash` static
  - `rn_ext_classify` static
  - `rn_ext_report` static
  - `rn_ext_execfail` static
  - `rn_ext_resolve` static
  - `rn_ext_child` static
  - `rn_ext_wait` static
  - `rn_exec_ext`

- `source/modules/runner/path.c`
  - `rn_path_free` static
  - `rn_path_join` static
  - `rn_path_match` static
  - `rn_path_search` static
  - `rn_path`

- `source/modules/runner/pipe.c`
  - `rn_pipe_status` static
  - `rn_pipe_count` static
  - `rn_pipe_flatten` static
  - `rn_pipe_close_all` static
  - `rn_pipe_create` static
  - `rn_pipe_child` static
  - `rn_pipe_wait` static
  - `rn_pipe_fork_all` static
  - `rn_pipe`

- `source/modules/runner/redir.c`
  - `rn_redir_append` static
  - `rn_redir_char` static
  - `rn_redir_value` static
  - `rn_redir_expand_line` static
  - `rn_redir_delim` static
  - `rn_redir_line` static
  - `rn_redir_warn` static
  - `rn_redir_linegrow` static
  - `rn_redir_readline` static
  - `rn_redir_heredoc_fill` static
  - `rn_redir_heredoc` static
  - `rn_redir_target` static
  - `rn_redir_open` static
  - `rn_redir_fd` static
  - `rn_redir_apply` static
  - `rn_redir_restore`
  - `rn_redir_push`

- `source/modules/runner/status.c`
  - `rn_status_set`
  - `rn_status_get`
