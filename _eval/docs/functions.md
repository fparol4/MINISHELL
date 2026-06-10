# Function Reference

All project functions, grouped by module. Each entry describes what the function does, its signature, and any non-obvious behaviour.

---

## core

### `core_run(char **envp) → int`
Top-level entry. Calls `core_init`, `core_loop`, `core_destroy` in sequence. Returns the final shell exit status.

### `core_init(t_shell *shell, char **envp) → int`
Initialises the shell: calls `env_init(envp)`, sets `$? = 0` via `env_set`, sets `running = TRUE`, installs interactive signal handlers. Returns 1 on failure.

### `core_destroy(t_shell *shell)`
Frees the env list, sets `running = FALSE`.

### `core_loop(t_shell *shell) → int`
REPL. Calls `core_iteration` until it returns −1 (EOF/exit). Returns `rn_status_get` at the end.

### `core_iteration(t_shell *shell) → int`
One prompt-read-execute cycle. Returns 0 (continue), 1 (Ctrl-C received), −1 (exit/EOF).

### `core_process_line(t_shell *shell, char *line) → int` *(static)*
Calls `core_parse` then `rn_execute`. Sets `$? = 2` on parse failure.

### `core_parse(char *line) → t_ast *` *(static)*
Chains `lexer → parser_controller → lexer_free`. Returns the AST or NULL.

---

## shared / error

### `sh_err(char *scope, char *msg)`
Prints `"minishell: [scope: ] msg\n"` to stderr. `scope` and `msg` may be NULL.

### `sh_err2(char *scope, char *arg, char *msg)`
Like `sh_err` but includes an argument name between scope and message: `"minishell: [scope: ] arg: msg\n"`.

### `sh_stxerr(t_syntax_error type)`
Prints a fixed syntax-error message for the given `t_syntax_error` enum to stderr.

---

## shared / free

### `sh_freeargs(char **args)`
Calls `sh_freesec((void **)args)`. Frees a `NULL`-terminated array of heap strings plus the array itself.

### `sh_freesec(void **items)`
Iterates `items[i]` calling `free` while non-NULL, then frees `items`.

---

## shared / signal

### `sh_sig_mode(t_sig_mode mode)`
Sets up SIGINT and SIGQUIT handlers for the given mode:
- `SIG_INTERACTIVE`: SIGINT → `sh_sigint_interactive` (redisplay prompt), SIGQUIT → ignored.
- `SIG_EXEC`: both ignored (children restore defaults before exec).
- `SIG_HEREDOC`: SIGINT → `sh_sigint_heredoc` (print newline, set `g_signal`), SIGQUIT → ignored.

---

## shared / string

### `sh_isspace(char c) → int`
Returns 1 for space, tab, or newline. Used by the expander for word splitting.

### `sh_varstart(char c) → int`
Returns 1 if `c` is a valid first character of a variable name: alpha or `_`.

### `sh_varchar(char c) → int`
Returns 1 if `c` is a valid non-first character of a variable name: alphanumeric or `_`.

### `sh_isidentifier(char *s) → int`
Returns 1 if `s` is a valid shell identifier: starts with `sh_varstart`, rest satisfy `sh_varchar`. Used by `export` and `unset` to validate arguments.

---

## shared / quote

### `sh_has_quotes(char *s) → int`
Returns 1 if `s` contains any `'` or `"`. Used by the lexer to set `token->quoted`.

### `sh_quote_remove(char *s) → char *`
Allocates a new string with all quote characters (and their matching close) removed. Used for heredoc delimiter cleaning. Tracks open-quote state with a `char quote` variable.

---

## env

### `env_init(char **envp) → t_env *`
Parse `envp` array → linked list. If `envp` is NULL, builds minimal env via `minenv()`. Increments `SHLVL` after init.

### `env_get(t_env **env, const char *key) → char *`
Linear search. Returns the value pointer (not a copy), or NULL if not found.

### `env_set(t_env **env, const char *key, const char *value) → int`
Update existing node or append new one. `value = NULL` is valid (declare without assign). Returns 0 / −1.

### `env_unset(t_env **env, const char *key) → int`
Remove and free node. Silent no-op if not found. Returns 0.

### `env_toarr(t_env **env) → char **`
Returns `NULL`-terminated `char **` of `"KEY=VALUE"` strings. Skips null-value nodes and `ENV_ERRCODE`. Caller frees with `sh_freeargs`.

### `env_size(t_env **env) → int`
Count all nodes.

### `env_free(t_env **env)`
Free entire list.

### `env_node_new(const char *key, const char *value) → t_env *` *(private)*
Allocate and populate a single node with `ft_strdup` copies.

### `env_node_free(t_env *node)` *(private)*
Free key, value, and node struct.

---

## lexer

### `lexer(const char *input) → t_manager *`
Main entry. Runs the scanner + state machine. Returns NULL and prints error on unclosed quote.

### `lexer_free(t_manager *manager)`
Free all `t_list_token` nodes and the manager struct.

### `state_machine(t_manager *manager, t_scanner *input, t_rules *rules) → int`
Drives the scanner: dispatches `extract_operator` or `extract_word` based on current character. Returns 0 if unclosed quote detected.

### `rules_init(t_rules *rules)`
Builds both lookup tables and sets `start_word` / `start_operator` masks.

### `extract_word(t_manager *manager, t_scanner *input, t_rules *rules) → t_list_token *`
Read characters until whitespace or operator (respecting open quotes). Returns NULL on unclosed quote.

### `extract_operator(t_manager *manager, t_scanner *input, t_rules *rules) → t_list_token *`
Read at most 2 consecutive operator characters. Maps string to `t_token_type`.

### `token_add(t_manager *manager, const char *value, t_token_type type) → t_list_token *`
Allocate token, set `expand` and `quoted` flags, append to list.

### `token_free(void *content)`
Free `t_list_token` and its value string.

### Scanner functions (all in `source/lexer/scanner/`)

| Function | Description |
|---|---|
| `scanner_init(sc, input)` | Set cursor to 0, record input pointer and length |
| `scanner_advance(sc)` | cursor++ if not at end |
| `scanner_is_end(sc) → int` | cursor >= len |
| `scanner_mark_start(sc) → size_t` | start = cursor |
| `scanner_current(sc) → size_t` | byte value at cursor (for table lookup) |
| `scanner_peek(sc, offset) → char` | byte at cursor+offset, or '\0' |
| `scanner_extract(sc) → char *` | `ft_substr(input, start, cursor-start)` |
| `scanner_until(sc, f)` | advance while `f(current)` is true |
| `scanner_table_char(table)` | fill char-class flags (P_*) for all 256 bytes |
| `scanner_table_token(table)` | fill token-class flags (L_*) for all 256 bytes |

---

## parser

### `parser_controller(t_manager *manager) → t_ast *`
Entry. Detects pipe-at-start, calls `build_pipeline`, returns AST (possibly with error).

### `parser_free_ast(t_ast *ast)`
Free tree + ast struct.

### `parser_free_cmd(t_command *cmd)`
Recursive: frees pipe nodes left+right, or simple node args+redirs.

### `parser_free_simple(t_simple *simple)`
Free args array (and all strings), redirs array (and all `file` strings), and the struct itself.

### `parse_simple_command(t_parser *parser) → t_command *`
Consume WORD and redir tokens until PIPE or end. Returns CMD node.

### `parser_init(t_parser *parser, t_manager *manager)`
Set first token, zero state.

### `parser_next(t_parser *parser)`
Advance to next node; record `last_type`.

### `parser_is_end(t_parser *parser) → t_bool`
True if no current node.

### `parser_get_class(t_token_type type) → unsigned int`
Map token type to `C_WORD`, `C_REDIR`, `C_PIPE`, or `C_NONE`.

### `parser_get_token(t_node *node) → t_list_token *`
Cast `node->content` to `t_list_token *`.

### `parser_start_command(t_parser *parser)`
Allocate `t_simple` and init both `t_array` members.

### `parser_finish_command(t_parser *parser)`
On error: free current_cmd. On empty (no args and no redirs): `SNTX_EMPTY_CMD`. Otherwise: detach (set `current_cmd = NULL`).

### `parser_add_arg(t_parser *parser, char *arg, t_bool expand)`
`ft_array_append` the arg string. Also sets `current_cmd->expand = TRUE` if expand flag. Keeps array NULL-terminated at `items[length]`.

### `parser_add_redir(t_parser *parser, t_parser_redir_type type, t_list_token *token)`
Builds a `t_parser_redir` struct and appends to `current_cmd->redirs`. `expand = !token->quoted`.

### `parser_set_syntax_error(t_parser *parser, t_syntax_error type)`
Set `parser->state = TRUE` and `ast->error_type = type`.

---

## runner / control

### `rn_execute(t_command *cmd, t_env **env) → int`
Dispatch to `rn_exec_cmd` or `rn_exec_pipe`. Always calls `rn_status_set` after.

### `rn_exec_cmd(t_command *cmd, t_env **env) → int`
Expand args, push redirs, run command (builtin or external), restore redirs.

### `rn_exec_pipe(t_command *cmd, t_env **env) → int`
Thin wrapper → `rn_pipe`.

---

## runner / expander

### `rn_expand(char **args, t_env **env) → char **`
Process every string in `args`, return new `NULL`-terminated array. May produce more or fewer strings (word splitting).

### `rn_argexpand(char *arg, t_arglist *out, t_env **env) → int` *(static)*
Per-argument expansion loop. Calls `exp_arg_char` repeatedly.

### `exp_arg_char(t_exp_ctx *ctx) → int`
Dispatch on current character: single quote → `exp_single`, double quote → `exp_double`, `$` → `exp_var`, whitespace → `exp_flush_word` + skip, other → `exp_wordchar`.

### `exp_single(t_word *word, char *arg, int *i) → int`
Copy everything between single quotes literally. Sets `word->active = 1`.

### `exp_double(t_exp_ctx *ctx) → int`
Copy between double quotes; expand `$` via `exp_var_quoted`. Sets `word->active = 1`.

### `exp_var(t_exp_ctx *ctx) → int`
Outside quotes: expand variable, pass result through `exp_add_unquoted` (word splitting enabled).

### `exp_var_quoted(t_exp_ctx *ctx) → int`
Inside double quotes: expand variable, append to word buffer via `exp_wordstr` (no word splitting).

### `exp_varvalue(char *arg, int *i, t_env **env) → char *`
Read variable name starting at `*i` (after `$`). Handles `$?` specially. Returns value pointer (not owned) or empty string. Returns NULL if `$` should be literal.

### `exp_wordchar(t_word *word, char c) → int`
Append one byte to the word buffer. Grows buffer as needed (doubles from 32).

### `exp_wordstr(t_word *word, const char *str) → int`
Append a string to the word buffer.

### `exp_flush_word(t_arglist *list, t_word *word) → int`
If `word->active`, `ft_strdup(word->buf)` → append to list. Reset word. No-op if not active.

### `exp_add_unquoted(t_arglist *list, t_word *word, const char *value) → int`
Walk value: whitespace triggers `exp_flush_word`; non-whitespace calls `exp_wordchar`. This is word splitting for unquoted variable expansions.

### `exp_listadd(t_arglist *list, char *item) → int`
Append one string to the output list after growing if needed.

### `exp_listgrow(t_arglist *list) → int`
Double capacity of `list->items` (start at 8). Copy existing pointers. Free old array.

---

## runner / pipe

### `rn_pipe(t_command *node, t_env **env) → int`
Flatten tree, create OS pipes, fork all children, wait for all, return last status.

### `rn_pipe_count(t_command *node) → size_t`
Recursive leaf count: pipe node = left + right, cmd node = 1.

### `rn_pipe_flatten(t_command *node, t_command **items, size_t *idx)`
In-order traversal: fills `items` with CMD nodes left-to-right.

### `rn_pipe_create(int *fds, size_t pipe_count) → int`
Initialise all fds to -1, then call `pipe` for each. Closes all on failure.

### `rn_pipe_close_all(int *fds, size_t pipe_count)`
Close all valid fds in the flat array (2 * pipe_count entries).

### `rn_pipe_child(t_pipe_ctx *ctx, size_t pos)`
Child setup: `dup2` stdin from previous pipe if not first; `dup2` stdout to next pipe if not last; close all pipe fds; restore signals to default; `_exit(rn_execute(cmds[pos], env))`.

### `rn_pipe_fork_wait(t_command **cmds, t_env **env, int *fds, size_t count) → int`
Switch to `SIG_EXEC`, fork all children, close parent's pipe fds, wait all, switch back to `SIG_INTERACTIVE`.

### `rn_pipe_wait(pid_t *pids, size_t count) → int`
Wait for all pids in order; return status of the last one. Handles `WIFEXITED` and `WIFSIGNALED`.

---

## runner / redir

### `rn_redir_push(t_parser_redir *redirs, size_t count, t_env **env, int saved[2]) → int`
Save stdin+stdout with `dup`, then call `rn_redir_apply`. On apply failure, restore before returning.

### `rn_redir_restore(int saved[2]) → int`
`dup2` saved fds back to stdin/stdout; close saved fds. Returns 1 on `dup2` failure.

### `rn_redir_apply(t_parser_redir *redirs, size_t count, t_env **env) → int`
Open all redir fds, then `dup2` each to the appropriate stdio. Closes any already-opened fds on failure.

### `rn_redir_fd(t_parser_redir *redir, t_env **env) → int`
Resolve target, then open fd. Returns the fd, or -1 (error), or -2 (interrupted by Ctrl-C).

### `rn_redir_target(t_parser_redir *redir, t_env **env) → char *` *(static)*
For heredoc: `sh_quote_remove(redir->file)`. For others: `rn_expand`, assert exactly one word.

### `rn_redir_open(char *target, t_parser_redir_type type) → int` *(static)*
`open` with the appropriate flags.

### `rn_redir_heredoc(char *target, t_env **env, int expand) → int`
Create pipe, fill it with heredoc content, close write end, return read end fd.

### `heredoc_read_loop(int fd, char *target, t_env **env, int expand) → t_heredoc_state`
Read lines from stdin until delimiter matched, EOF, signal, or error. Write processed lines to `fd`.

### `rn_redir_readline() → char *`
Print `"> "` prompt, read one line from stdin one byte at a time into a growing buffer. Returns NULL on EOF or read error.

### `rn_redir_line(char *line, t_env **env, int expand) → char *`
If `expand`: walk line expanding `$VAR` and `$?`. Always appends `"\n"`. Returns new string.

### `rn_redir_warn(char *target)`
Print `"minishell: warning: here-document delimited by end-of-file (wanted 'target')"` to stderr.

### `rn_redir_append(char **buf, char *part) → int` *(buffer helper)*
`ft_strjoin(*buf, part)` → free old → store new. Returns 1 on alloc failure.

### `rn_redir_char(char **buf, char c) → int` *(buffer helper)*
Append single character via `rn_redir_append`.

### `rn_redir_linegrow(char **line, size_t *cap, size_t len) → int`
Grow `*line` buffer to at least `len+1`. Doubles from 32.

### `read_heredoc_loop(char **line, size_t *len, size_t *cap) → ssize_t`
Read one byte at a time until newline or EOF/error. Fills `*line` (growing). Returns 1 on newline, ≤0 on end/error.

---

## runner / external

### `rn_exec_ext(char **args, t_env **env) → int`
Resolve path, convert env to array, switch to `SIG_EXEC`, fork, exec in child, wait.

### `rn_ext_resolve(char **args, t_env **env, char **path) → int`
Determine executable path: direct (has `/`) or via `rn_path`. Returns 0 if ready, error status otherwise.

### `rn_ext_classify(char *path) → t_ext_kind`
`stat` the path. Returns `EXT_READY`, `EXT_NOT_FOUND`, `EXT_DIR`, or `EXT_DENIED`.

### `rn_ext_report(char *path, t_ext_kind kind) → int`
Print appropriate error, return 126 or 127.

### `rn_ext_child(char *path, char **args, char **envp)`
Restore default signals, `execve`. On failure: `_exit(rn_ext_execfail(path))`.

### `rn_ext_execfail(char *path) → int` 
Map `errno` to message + return code (127 for ENOENT, 126 for others).

### `rn_ext_wait(pid_t pid) → int`
`waitpid`, switch to `SIG_INTERACTIVE`, decode status.

---

## runner / path

### `rn_path(char **args, t_env **env) → char *`
Search PATH env var for an executable matching `args[0]`. Returns heap-allocated path or NULL.

### `rn_path_search(char **args, t_env **env) → char *` *(static)*
Split PATH on `:`, call `path_search_dirs`.

### `path_search_dirs(char **dirs, char *cmd) → char *` *(static)*
Iterate dirs, build full path, call `rn_path_candidate`. Returns first ready match, or fallback (exists but not executable).

### `rn_path_join(char *dir, char *cmd) → char *`
`ft_strjoin(dir, "/")` + `ft_strjoin(tmp, cmd)`.

### `rn_path_match(char *full) → int`
`stat` + not-directory check + `access(X_OK)`.

### `rn_path_candidate(char *full, char **fallback) → int`
If match: return 1 (caller keeps path). If not match but exists: store as fallback. Else: free and return 0.

### `rn_path_free(char **dirs)`
`sh_freeargs(dirs)`.

---

## runner / status

### `rn_status_set(t_env **env, int status) → int`
`ft_itoa(status)` → `env_set(env, ENV_ERRCODE, value)`.

### `rn_status_get(t_env **env) → int`
`env_get(env, ENV_ERRCODE)` → `ft_atoi`.

---

## runner / builtin dispatch

### `rn_exec_bin(char **args, t_env **env, int *status) → int`
Try each builtin name with `ft_strcmp(args[0], name)`. On match: call `bin_X(args + 1, env)`, set `*status`, return 1. Return 0 if no match.

---

## builtins

### `bin_echo(char **args, t_env **env) → int`
Print args with spaces. Consume leading `-n`/`-nn` flags to suppress newline.

### `bin_cd(char **args, t_env **env) → int`
Change directory. Updates `$OPWD` and `$PWD`. Handles no-arg (HOME) and `-` (OLDPWD).

### `bin_pwd(char **args, t_env **env) → int`
Print cwd via `getcwd`, fallback to `$PWD`.

### `bin_export(char **args, t_env **env) → int`
No args: sorted `declare -x` listing. With args: validate and set env entries.

### `bin_unset(char **args, t_env **env) → int`
Remove env entries. Validate identifiers.

### `bin_env(char **args, t_env **env) → int`
Print all env entries with values.

### `bin_exit(char **args, t_env **env) → int`
Parse optional numeric exit code, call `exit()`. Too-many-args returns 1 without exiting.

### `export_print(t_env **env) → int` *(export/print.c)*
Print sorted `declare -x` listing for all entries except `ENV_ERRCODE`.

### `export_getkeys(t_env **env, int *size_out) → char **` *(export/keys.c)*
Collect all keys (except `ENV_ERRCODE`) into array, bubble-sort alphabetically, return.

### `export_sortkeys(char **keys, int size)` *(static)*
In-place bubble sort of key pointer array using `ft_strcmp`.
