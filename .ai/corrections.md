# Minishell Suggested Corrections

This is a second-pass correction list based on the current implementation.

## High priority

- Enhance signal handling for interactive mode, command execution, heredocs, Ctrl-C, and Ctrl-D.
  - Ctrl-C at the prompt should clear the current line, print a fresh prompt, and set the shell status to `130`.
  - Ctrl-C while a foreground command is running should not leave readline state confused, and `$?` should become `128 + SIGINT` when the child is interrupted.
  - Ctrl-C during heredoc must abort the heredoc and skip command execution, with status `130`; it should not be treated as heredoc EOF.
  - Ctrl-D at the prompt should exit cleanly, print `exit`, and return the current shell status.
  - Ctrl-D during heredoc should keep the EOF warning behavior, but it must be distinct from Ctrl-C cancellation.

- Update `$?` on lexer and parser errors.
  - `core_process_line()` currently returns without setting status when `lexer_control()` fails.
  - Parser syntax errors print an error but do not call `rn_status_set()`.
  - Syntax errors such as `|`, `echo >`, `echo >>> out`, and unclosed quotes should set `$?` to `2`.

- Fix heredoc expansion semantics.
  - Heredoc body expansion should depend on whether the delimiter was quoted, not whether the delimiter text contains `$`.
  - `cat << EOF` should expand variables in the heredoc body.
  - `cat << 'EOF'` and `cat << "EOF"` should keep the heredoc body literal.
  - The delimiter used for comparison should have quote characters removed.

- Track quote metadata separately from dollar-presence expansion metadata.
  - `token->expand = ft_strchr(value, '$') != NULL` is not enough for shell semantics.
  - The parser/runner needs to know whether a word was single-quoted, double-quoted, unquoted, or mixed.
  - This matters for heredoc delimiters, redirect target expansion, quote removal, and field splitting.

- Fix redirection target expansion to honor quoting rules.
  - `rn_redir_target()` currently sends every redirect target through `rn_expand()`.
  - Quoted redirect targets should not field-split.
  - Ambiguous redirect should happen only when an unquoted expansion produces zero or multiple fields.
  - Quote removal should happen before opening the target path.

## Runner and process behavior

- Avoid printing `exit` from pipeline children.
  - `exit | cat` should not print the interactive `exit` banner.
  - The builtin needs context about whether it is running in the main interactive shell or a forked pipeline child.

- Review pipeline execution for nested pipeline AST nodes.
  - The parser builds left-associative nested pipe nodes.
  - `rn_pipe()` forks only two sides and recursively calls `rn_execute()` inside children.
  - This can work for simple cases, but it is harder to reason about for signal handling, exit status, and file descriptor closure. A flattened pipeline executor would be simpler and safer.

- Check pipe/file descriptor cleanup paths.
  - Fork failures and nested pipeline execution should close every inherited pipe end that is no longer needed.
  - Children should not retain unrelated pipe descriptors.

- Ensure external-command signal output matches shell behavior.
  - When a child dies by `SIGINT` or `SIGQUIT`, status is already mapped to `128 + signal`; verify the visible newline/core-dump message behavior against bash expectations for the project.

## Builtins and environment

- Fix `exit` numeric parsing range.
  - Shell `exit` accepts signed `long long` range, then returns the value modulo 256.
  - The current `ft_vatoi()` validates only `INT_MIN..INT_MAX`, so values such as `2147483648` are incorrectly rejected as non-numeric.
  - Use a `long long` parser with overflow detection, then cast/modulo for the final exit code.

- Keep `exit` with no arguments returning the previous status.
  - The current source already uses `rn_status_get(env)` for this. Keep this behavior covered by tests.

- Revisit environment export behavior for variables without `=`.
  - `export NAME` should mark the name for export without necessarily creating `NAME=` in child environments, depending on the expected 42 minishell behavior.
  - `env` and `env_toarr()` should be consistent about whether null-valued variables are exported.
  - `ENV_ERRCODE` must remain internal and must not be exported to child processes.

- Improve `cd` edge cases.
  - `cd` should reject too many arguments.
  - `cd -` should use `OLDPWD`; the code currently defines `ENV_OPWD` as `"OPWD"`, which looks like a typo for `"OLDPWD"`.
  - If `getcwd()` fails after `chdir()`, status and `PWD` handling should be deliberate.

- Improve `pwd` failure behavior.
  - If `getcwd()` fails but `PWD` exists, the command prints the fallback and still returns `1`.
  - Decide whether fallback success should return `0` or whether an error should also be printed.

- Validate `unset` identifiers.
  - `unset` should reject invalid identifiers with an error and non-zero status.
  - It should not blindly call `env_unset()` for every argument.

## Lexer, parser, and expansion

- Add quote-removal as an explicit step.
  - The current expander removes quotes while expanding command arguments, but the lexer/parser do not preserve enough metadata for all contexts.
  - Redirection filenames and heredoc delimiters need quote removal too.

- Revisit operator tokenization for unsupported repeated operators.
  - `>>>` is tokenized as `>>` followed by `>`, then reported as "redirection target".
  - Consider reporting the unexpected token more precisely, especially for `>>>`, `|||`, `&&`, and `;;` if they appear.

- Make allocation failures explicit in parser helpers.
  - `parser_add_arg()` and `parser_add_redir()` silently return on allocation failure without setting parser error state in every path.
  - If `ft_strdup()` or `dynarray_append()` fails, parsing should fail cleanly and free partial state.

- Avoid casting `unsigned int *` to `size_t *`.
  - `parser_add_redir()` passes `(size_t *)&parser->current_cmd->redir_count` into `dynarray_append()`.
  - This is undefined behavior on platforms where `sizeof(size_t) != sizeof(unsigned int)`.
  - Use a local `size_t`, then assign back after successful append, or make `redir_count` a `size_t`.

- Review word splitting rules.
  - `rn_expand()` splits unquoted variable values only on shell whitespace, but it does not model all shell field-splitting details.
  - Empty quoted arguments should be preserved, while empty unquoted expansions should disappear.
  - Add tests for `""`, `''`, `a""b`, `$EMPTY`, `"$EMPTY"`, and mixed quoted/unquoted words.

## Tests to add

- Syntax status:
  - `|` then `echo $?` should print `2`.
  - `echo >` then `echo $?` should print `2`.
  - unclosed quote then `echo $?` should print `2`.

- Signals:
  - Ctrl-C at prompt sets status `130` and redraws cleanly.
  - Ctrl-C during `cat` returns to prompt with status `130`.
  - Ctrl-C during heredoc aborts execution and status becomes `130`.
  - Ctrl-D at prompt exits with the current status.
  - Ctrl-D during heredoc prints the EOF warning and continues according to shell rules.

- Heredoc:
  - `cat << EOF` expands `$VAR`.
  - `cat << 'EOF'` does not expand `$VAR`.
  - `cat << "EOF"` does not expand `$VAR`.
  - Quoted delimiters are matched after quote removal.

- Redirection:
  - `echo hi > "$A"` does not field-split the target.
  - `echo hi > $A` reports ambiguous redirect when `$A` expands to multiple fields.
  - `echo hi > "$EMPTY"` attempts to open an empty filename and reports the correct error.

- Builtins:
  - `exit 2147483648` exits with status `0`.
  - `exit 9223372036854775808` reports numeric argument required and exits `2`.
  - `exit | cat` does not print the interactive `exit` banner.
  - `cd a b` fails.
  - `cd -` uses `OLDPWD`.
  - invalid `unset` identifiers fail.
