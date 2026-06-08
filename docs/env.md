# Environment

**Files:** `source/env/`, `headers/env.h`

## Data structure

```c
typedef struct s_env {
    char         *key;
    char         *value;
    struct s_env *next;
} t_env;
```

A singly-linked list. Each node owns its `key` and `value` strings. Nodes with `value = NULL` represent variables that have been declared but not assigned (e.g., `export X` with no `=`). These are tracked by the shell and shown by `export` but not emitted by `env` or passed to child processes via `execve`.

## Special keys

| Constant | Value | Meaning |
|---|---|---|
| `ENV_PWD` | `"PWD"` | Current working directory |
| `ENV_OPWD` | `"OPWD"` | Previous directory (used by `cd -`) |
| `ENV_ERRCODE` | `"$?"` | Last exit status (internal only) |

`ENV_ERRCODE` uses the literal key `"$?"` so it can never clash with a real environment variable. `env_toarr` and `bin_env` both skip it explicitly so it is never leaked to child processes.

## API

### `env_init(char **envp) → t_env *`

Parses the `envp` array received by `main`. Each string is split on the first `=` into key and value. Entries without `=` are stored with `value = NULL`.

If `envp` is `NULL`, falls back to `minenv()` which builds a minimal two-entry list: `PWD=<getcwd>` and `SHLVL=1`.

After parsing, `shlvl()` walks the list and increments the `SHLVL` value by 1 (starting at 1 if unset). This correctly tracks shell nesting depth.

### `env_get(t_env **env, const char *key) → char *`

Linear search. Returns `node->value` (not a copy — do not free). Returns `NULL` if not found.

### `env_set(t_env **env, const char *key, const char *value) → int`

- If key exists: `free` old value, `ft_strdup(value)` into node. If `value` is `NULL`, sets `node->value = NULL` (declare without assign).
- If key does not exist: allocate new node and append to tail.
- Returns `0` on success, `-1` on allocation failure.

### `env_unset(t_env **env, const char *key) → int`

Unlink and free the node matching `key`. Adjusts `prev->next` or `*env` as needed. Silently succeeds if key not found (matches bash behaviour).

### `env_toarr(t_env **env) → char **`

Allocates a `NULL`-terminated `char **` array of `"KEY=VALUE"` strings. Skips nodes where `value == NULL` (declared-only variables) and the `ENV_ERRCODE` internal key. Used by `execve` to pass environment to child processes. Caller must free with `sh_freeargs`.

### `env_size(t_env **env) → int`

Counts all nodes regardless of value. Used internally by `env_toarr` and `export_getkeys`.

### `env_free(t_env **env)`

Frees all nodes in the list. Called once at shell exit by `core_destroy`.

## Node allocation helpers (private, `_env.h`)

- `env_node_new(key, value)` — allocates node with `ft_strdup` copies of both strings.
- `env_node_free(node)` — frees key, value, and node.

## Thread safety / ownership

The env list is not thread-safe. It lives entirely in the parent process and is never shared across forks. Child processes receive a snapshot via `env_toarr` + `execve`. Builtins that modify env (`cd`, `export`, `unset`) run in the parent process and affect the live list directly. When a builtin runs inside a pipe child, its env modifications are invisible to the parent.
