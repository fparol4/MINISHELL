*This project has been created as part of the 42 curriculum by fcardozo and g-alves-*

# Minishell

## Description

Minishell is a small Unix shell written in C. It displays an interactive prompt,
keeps command history, expands environment variables, runs builtins and external
programs, handles redirections, pipelines, heredocs, and shell-like exit status
tracking through `$?`.

The implementation follows the mandatory 42 minishell subject and uses bash as
the reference for behavior when the subject leaves details open.

## Instructions

Build the project:

```sh
make
```

Run the shell:

```sh
./minishell
```

Remove generated objects:

```sh
make clean
```

Remove generated objects and the executable:

```sh
make fclean
```

Rebuild from scratch:

```sh
make re
```

## Features

- Interactive prompt with history.
- External command execution through `PATH`, relative paths, and absolute paths.
- Builtins: `echo`, `cd`, `pwd`, `export`, `unset`, `env`, and `exit`.
- Single and double quote handling.
- Environment expansion with `$VAR` and `$?`.
- Redirections: `<`, `>`, `<<`, and `>>`.
- Pipelines with `|`.
- Signal behavior for Ctrl-C, Ctrl-D, and Ctrl-\ in interactive mode.

## Resources

- `man bash`
- `man 2 fork`
- `man 2 execve`
- `man 2 pipe`
- `man 2 dup2`
- `man 2 waitpid`
- `man 3 readline`
- `man 7 signal`

AI was used as a review and debugging assistant: to inspect the implementation,
compare behavior against the subject, identify likely bugs, organize correction
lists, and help apply focused fixes. All generated suggestions were reviewed
against the project source and tested locally before being kept.
