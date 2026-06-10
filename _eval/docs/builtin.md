# Builtins

**Files:** `source/bin/`  
**Dispatch:** `source/runner/builtin.c — rn_exec_bin()`

All builtins share the signature `int bin_X(char **args, t_env **env)` where `args` is the expanded argument list starting after the command name (i.e., `args[0]` is the first argument, not the command).

---

## echo (`bin/echo.c`)

```
echo [-n] [args...]
```

Prints arguments separated by spaces. Adds a newline at the end unless `-n` is given.

**`-n` detection** (`echo_has_flag`): a flag argument is `-` followed by one or more `n` characters only. `-nn`, `-nnn` are all valid. `-na` is not. Multiple leading `-n` flags are consumed in sequence before printing starts.

**Return:** always 0.

---

## cd (`bin/cd.c`)

```
cd [path]
```

**No argument:** reads `$HOME` (`PWD_HOME` constant). If `HOME` is not set → error message, return 1.

**`cd -`:** reads `$OPWD` (or falls back to `$OPWD` via the literal key). Prints the target path to stdout before changing. If `OPWD` is not set → error message, return 1.

**With path:** use it directly.

**After `chdir(path)`:**
1. Save old `$PWD` into `$OPWD`.
2. `getcwd()` → set `$PWD` to the real resolved path.

**Return:** 0 on success, 1 on error (too many args / HOME unset / OPWD unset / chdir failed).

Note: `cd` is always run in the parent process. When run in a pipe, the directory change only affects that child — it is not visible to the parent shell.

---

## pwd (`bin/pwd.c`)

```
pwd
```

Calls `getcwd()` into a 4096-byte stack buffer. If `getcwd` fails (e.g., directory was deleted), falls back to `env_get(env, ENV_PWD)`. Prints result with a newline.

**Return:** 0 on success, 1 if both `getcwd` and `$PWD` fail.

---

## export (`bin/export/`)

```
export [name[=value]...]
```

**No arguments:** `export_print()` — prints all env entries sorted alphabetically in `declare -x KEY="VALUE"` format. Variables declared without a value print as `declare -x KEY` (no `=`). `ENV_ERRCODE` (`$?`) is excluded.

**With arguments:** for each arg:
1. Extract key: everything before `=` (or whole string if no `=`).
2. Validate key with `sh_isidentifier()` — must start with alpha or `_`, rest alphanumeric or `_`. Invalid → error message, continue (code = 1).
3. If `=` present: `env_set(env, key, value_after_eq)`.
4. If no `=`: `env_set(env, key, NULL)` — declare without assigning.

**Sorting** (`keys.c — export_getkeys + export_sortkeys`): collects all keys into a `char **`, sorts with bubble sort using `ft_strcmp`, then iterates to print. `ENV_ERRCODE` is excluded from the count.

**Return:** 0 if all args valid, 1 if any arg was an invalid identifier.

---

## unset (`bin/unset.c`)

```
unset [name...]
```

For each argument: validate with `sh_isidentifier()`. Invalid → error message, status = 1. Valid → `env_unset(env, arg)`.

Silently does nothing if the key does not exist.

**Return:** 0 if all args valid, 1 if any was an invalid identifier.

---

## env (`bin/env.c`)

```
env
```

Prints all env entries that have a value and are not `ENV_ERRCODE`, in insertion order, as `KEY=VALUE\n`.

If any argument is given → `"env: <arg>: No such file or directory"`, return 127 (mimics trying to run env as a command runner).

**Return:** 0 (success), 127 (if args given).

---

## exit (`bin/exit.c`)

```
exit [n]
```

If interactive (both stdin and stdout are ttys): prints `"exit"` to stdout.

**No argument:** `exit(rn_status_get(env))` — exit with current `$?`.

**With one numeric argument:** parse the number with overflow detection. The parser (`exit_parse_prefix` + `exit_parse_digits`) handles optional leading whitespace and `+`/`-` sign, then reads digits checking for `LLONG_MAX`/`LLONG_MIN` overflow. `exit((unsigned char)code)` — only the low 8 bits matter, matching bash.

**Two or more arguments:** `"too many arguments"` to stderr, return 1. Does **not** exit.

**Non-numeric argument:** `"numeric argument required"` to stderr, `exit(2)`.

**Return:** 1 (too many args — only case that returns rather than exiting).
