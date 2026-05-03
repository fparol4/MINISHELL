# Minishell: Problem Overview & Development Strategy

## Our Intent
Our goal is to build the execution engine for Minishell, picking up exactly where the **Parser flow** leaves off. The parser generates an Abstract Syntax Tree (AST) representing commands, pipes, and logical operators. Our responsibility is to reliably, securely, and accurately execute this tree while strictly adhering to the 42 School Norminette rules and zero-memory-leak constraints.

## Current Problem Space
The parser provides us with structured AST nodes (`t_cmd`), but turning those nodes into actual system processes introduces several complex, interlocking challenges:

1. **Environment State Management:** We cannot simply rely on the default `envp` array passed to `main()`. Built-in commands like `export`, `unset`, and `cd` dynamically alter the shell's environment during runtime. We need a robust, easily mutable structure (a linked list) to track state.
2. **Execution & Path Resolution:** We must correctly locate binaries using the dynamically changing `$PATH` variable and execute them securely using `execve`, handling all access rights and "command not found" errors properly.
3. **Pipes and Redirections:** The execution engine must correctly wire file descriptors across multiple child processes (`fork()`, `pipe()`, `dup2()`). Failing to close FDs properly will lead to deadlocks or FD exhaustion.
4. **Built-ins Segregation:** Certain built-ins (like `cd`, `export`, `unset`, `exit`) must alter the parent shell's memory space. Therefore, they *cannot* be executed in a child process (unlike standard system commands or built-ins running inside a pipe).

## Our Development Strategy: "Components and Connect"
Attempting to build the execution flow "Start to End" (traversing the AST and attempting to execute it all at once) results in spaghetti code that is overwhelmingly difficult to debug once forks and pipes are introduced. 

Instead, we are utilizing a **bottom-up, modular approach**:

1. **Test-Driven Foundation:** We have integrated the lightweight, header-only `snow.h` testing framework into our `tests/` directory.
2. **Isolated Modules:** We are building distinct, highly-focused components (Environment Manager, Built-ins Manager, Executor) and testing them independently using mock AST structures (`tests/mocks/mock_commands.c`).
3. **Current Priority - The Environment:** Because everything from variable expansion to `execve` relies on the shell's environment, our first task is the **Environment Management** module. We have established guidelines in `src/env/guideline.md` to build the `envm_` API (`envm_init`, `envm_get`, `envm_set`, `envm_unset`, `envm_toarr`) before touching any forks or pipes.

By thoroughly testing these independent components first, the final step of "connecting" them via an AST traversal will be safe, predictable, and robust.
