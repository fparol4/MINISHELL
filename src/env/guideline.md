# Environment Management Module Guidelines

## Overview
This module is responsible for managing the shell's environment variables. It serves as the foundation for variable expansion, built-in commands (`export`, `unset`, `env`, `cd`), and passing the environment to child processes during execution.

## 42 Project Constraints
- **Norminette:** All C code must strictly comply with the 42 Norm.
- **Allowed Functions:** `malloc`, `free`, `perror`, `strerror` (along with your custom `libft` equivalent functions).
- **Memory Management:** Zero memory leaks allowed. Every allocated string, array, or struct must be cleanly freed upon shell exit or when an environment variable is unset/replaced.

## Architecture & Data Structures
While the `execve` system call requires a `char **` array, managing insertions and deletions (via `export` and `unset`) in a contiguous double array is highly inefficient and error-prone. 

**Recommendation:** Use a Linked List for internal state management.

```c
typedef struct s_env
{
    char            *key;
    char            *value;
    struct s_env    *next;
}   t_env;
```

## Required Core Functions

### 1. `t_env *envm_init(char **envp)`
- Parses the original `envp` array provided to `main()` into the `t_env` linked list.
- **Edge case:** If `envp` is empty or missing, provide minimal defaults (e.g., `PWD`, increment `SHLVL`).

### 2. `char *envm_get(t_env *env_list, const char *key)`
- Searches the linked list for the given key and returns its value.
- **Edge case:** Should safely return `NULL` if the key is not found.

### 3. `int envm_set(t_env **env_list, const char *key, const char *value)`
- Updates an existing key with a new value, OR appends a new node if the key doesn't exist.
- **Memory Warning:** When updating an existing node, ensure you `free()` the old value before assigning the newly duplicated string.

### 4. `void envm_unset(t_env **env_list, const char *key)`
- Removes the node matching the given key.
- Ensure the `next` pointers of the previous node are correctly re-linked and the deleted node's memory is fully freed (`key`, `value`, and struct).

### 5. `char **envm_toarr(t_env *env_list)`
- Reconstructs a `NULL`-terminated `char **` array from the current linked list state.
- **Usage:** This will be called immediately before `execve` to pass the updated environment to external commands.

## Important Edge Cases & Behaviors
- **Empty Values:** A variable can exist with an empty string value (e.g., `export VAR=`). `value` should point to `\0`.
- **No Equals Sign:** `export VAR` creates the variable without a value. `value` should safely be `NULL`.
- **Invalid Identifiers:** `export` keys must start with a letter or underscore `_`, and only contain alphanumeric characters or underscores. 
- **`SHLVL`:** Should be incremented by 1 when the shell starts up.

## Testing Strategy (Using `snow.h`)
The next agent should create a dedicated test file: `tests/test_env.c`. Use the `snow.h` framework to build confidence in the module before integrating it into the parser/executor.

**Must-have Unit Tests:**
1. **Creation:** Initialize an empty list, add `"USER=test"`, and assert `envm_get` returns `"test"`.
2. **Update:** Call `envm_set` on `"USER"` again with `"new_user"`. Assert the change.
3. **Deletion:** Call `envm_unset` on `"USER"`. Assert `envm_get` now returns `NULL`.
4. **Conversion:** Create a list with two elements, call `envm_toarr`, and assert index `0` and `1` contain the properly formatted `KEY=VALUE` strings.
