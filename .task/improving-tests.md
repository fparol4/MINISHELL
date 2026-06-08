# Improving Tests

The current suite passes and covers the main module surfaces, but these cases
would make regressions easier to catch after the parser/runner refactors.

## Expander

- `$?` expands from `ENV_ERRCODE`.
- Unknown variables expand to an empty string.
- Empty quoted arguments are preserved as empty args.
- A lone `$` stays literal.
- `$` followed by an invalid variable start stays literal.
- Adjacent quoted and unquoted empty parts keep the expected word count.
- Multiple variables in the same word expand in order.
- Unquoted empty expansion removes the argument when appropriate.

## Lexer And Parser

- Mixed single and double quotes in one word.
- Quoted redirection targets preserve `quoted` metadata.
- Heredoc delimiters with quotes disable heredoc body expansion.
- Pipes with surrounding whitespace and quoted pipe characters.
- Consecutive operators such as `||`, `>>>`, `<< <`, and `> |`.
- Empty quoted command args, for example `echo ""`.
- Redirections without command words, for example `> file`.
- Multiple redirections before and after args in the same command.

## Redirections

- Ambiguous redirect for empty expansion.
- Redirect target with quoted spaces should resolve as one target.
- Redirect target with unquoted spaces should fail as ambiguous.
- Permission-denied output redirect.
- Input redirect missing file updates status and prevents command execution.
- Heredoc interrupted by `SIGINT` returns the expected status.

## Builtins

- `export KEY` creates an env node without a value.
- `export KEY=` creates an env node with an empty value.
- `export _A1=value` accepts underscores and digits after the first char.
- `export A+=value` behavior is explicit, whether supported or rejected.
- `unset` ignores missing keys and rejects invalid identifiers if required.
- `echo` handles repeated `-n` flags.
- `cd` updates `OLDPWD` as well as `PWD`.
- `exit` handles numeric overflow.

## Runner Integration

- Full lexer -> parser -> runner flow for simple commands.
- Full flow for pipelines with redirections.
- Full flow for heredoc plus variable expansion.
- Status after failed command, failed redirection, and failed pipeline branch.
- Builtin with redirection restores stdio afterward.
- Command with only redirections creates/truncates files and returns success.

