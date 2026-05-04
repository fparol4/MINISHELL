# Execution Manager (`execm`) Guidelines

## Overview
The `execm` module is responsible for path resolution and the actual execution of commands. It sits squarely between the parser's Abstract Syntax Tree (`t_cmd`) and the operating system's process management system (`fork`, `execve`, `waitpid`). 

## Core Responsibilities
1. **Command Classification:** Determine if a given command is a shell built-in or an external system binary.
2. **Path Resolution:** Locate the absolute path of an external binary using the `PATH` environment variable.
3. **Execution:** Use `fork()` to create child processes and `execve()` to replace the child process with the target program.
4. **Status Management:** Wait for the child process to complete and capture its exit status to update the shell's `$?` variable.

## Required Core Functions

### 1. `char *execm_get_path(t_env *env_list, const char *cmd)`
- Fetches the `PATH` string from the environment list via `envm_get`.
- Uses `ft_split` to divide `PATH` by colons (`:`).
- Appends `/<cmd>` to each directory and uses `access(path, F_OK | X_OK)` to check if the binary exists and is executable.
- **Edge cases:**
  - If `cmd` contains a slash (e.g., `./minishell`, `../src`, or `/bin/ls`), **DO NOT** search the `PATH`. Directly check `access(cmd, F_OK | X_OK)` and return a duplicate of `cmd` if valid.
  - If the command is not found in any path, return `NULL` (this will trigger a "command not found" error).

### 2. `int execm_is_builtin(const char *cmd)`
- Returns `1` if `cmd` matches one of the mandatory built-ins (`echo`, `cd`, `pwd`, `export`, `unset`, `env`, `exit`), otherwise `0`.

### 3. `int execm_run_builtin(t_cmd *cmd_node, t_env **env_list)`
- Dispatches the execution to the correct `builtin_<name>` function inside `src/builtin/`.

### 4. `int execm_exec_simple(t_cmd *cmd_node, t_env **env_list)`
- The core executor for a single, non-piped command.
- **Flow:**
  1. Check if `cmd_node->args[0]` is a builtin using `execm_is_builtin`. If yes, run `execm_run_builtin` in the current process (do not fork unless it's inside a pipe, which we will handle later).
  2. If it's an external command, resolve its path with `execm_get_path`.
  3. Convert `env_list` to a `char **` array using `envm_toarr`.
  4. Call `fork()`.
     - **In the child (`pid == 0`):** Call `execve(path, args, env_array)`. If it fails, print an error, free all duplicated memory, and `exit()`.
     - **In the parent:** Call `waitpid(pid, &status, 0)` and extract the return code using the `WIFEXITED(status)` and `WEXITSTATUS(status)` macros.

## Important Edge Cases & Behaviors
- **Memory Leaks in the Child:** When `fork()` duplicates the process, the child inherits a copy of all allocated memory (the AST, the env list, etc.). If `execve` succeeds, this memory is automatically replaced by the new program. However, if `execve` fails, the child MUST explicitly free all memory before calling `exit()`, otherwise Valgrind will report leaks.
- **Standard Exit Codes:** 
  - `127`: Command not found.
  - `126`: Command found but not executable (Permission denied).
- **Directory Execution:** Attempting to execute a directory directly (e.g., `./src/`) should result in an "Is a directory" error and exit with code `126`.

## Testing Strategy (Using `snow.h`)
The next agent should create `tests/test_execm.c` to test path resolution and classification independently.

**Must-have Unit Tests:**
1. **Path Resolution (System):** Test `execm_get_path` with `"ls"`. It should return a valid absolute path like `/bin/ls` or `/usr/bin/ls`.
2. **Path Resolution (Local):** Test `execm_get_path` with a local executable or script (e.g., `"./minishell"` if it exists, or create a temporary script) and ensure it returns the local path.
3. **Command Not Found:** Test `execm_get_path` with a fake command `"does_not_exist_xyz"` and ensure it returns `NULL`.
4. **Classification:** Test `execm_is_builtin` with `"cd"` (returns `1`) and `"grep"` (returns `0`).

*(Note: Mocking `fork` and `execve` in unit tests is complex and error-prone. Focus your unit tests strictly on path resolution and classification. Full execution will be tested via bash script integration tests later).*
