# Lexer Enhancement Plan

This document defines the naming and file-structure direction for the lexer
module only. It is intended as a working agreement before refactoring.

## Goal

Make the lexer easier to scan, easier to split into small files, and easier to
navigate without changing behavior.

The target shape is:

- one responsibility per file
- one clear public entry point for the lexer
- one internal header that centralizes the lexer implementation API
- no ownership changes unless explicitly required

## Header Boundary

### Public header

`headers/lexer.h`

This should expose only the public lexer-facing types that other modules need.
It should stay small and stable.

### Internal lexer header

`headers/lexer_internal.h`

This should expose the complete lexer implementation surface to files inside the
lexer module. It should centralize the internal API instead of scattering
declarations across multiple headers.

The internal header is the place for:

- scanner functions
- rules functions
- extraction functions
- token functions
- state functions
- lexer entry point
- lexer-internal types and helpers

## File Structure

The lexer module should be split by responsibility:

```text
source/modules/lexer/
├── scanner/
│   ├── load.c
│   ├── move.c
│   ├── get.c
│   └── table.c
├── rules.c
├── extract.c
├── token.c
├── state.c
└── lexer.c
```

## Naming Convention

The preferred convention is:

- the file name describes the responsibility
- the function name describes the operation
- the folder describes the subsystem

This is the convention to keep:

- `load.c` -> load/setup functions
- `move.c` -> cursor movement and scan progress
- `get.c` -> read/access helpers
- `table.c` -> character/token table initialization
- `rules.c` -> rule definition and lexer rule setup
- `extract.c` -> word/operator extraction
- `token.c` -> token allocation and release
- `state.c` -> scan state machine
- `lexer.c` -> lexer entry point

This avoids overloading generic names like `scanner.c` while keeping the code
short and discoverable.

## Function Naming Pattern

The function names should follow the file responsibility, but stay descriptive.

Recommended shape:

- `scanner_init`
- `scanner_advance`
- `scanner_until`
- `scanner_markstart`
- `scanner_isend`
- `scanner_current`
- `scanner_peek`
- `scanner_extract`
- `scanner_tablechar`
- `scanner_tabletoken`
- `rules`
- `extract_getword`
- `extract_getoperator`
- `token`
- `state`
- `lexer`

The important rule is consistency. A caller should be able to guess the file
from the function name and the function role from the file name.

## What To Keep Simple

The refactor should not change behavior.

Do not:

- change lexer token semantics
- change quote handling behavior
- change error behavior
- change ownership rules unless needed for the split
- rename public types unless required by the module boundary

Do:

- reduce file size
- reduce generic helper naming noise
- group related helpers together
- keep the internal header authoritative for the module

## Practical Rules

1. `scanner_free` is not required unless the scanner starts owning memory.
2. `take_while` should become `scanner_until` if the behavior stays the same.
3. The lexer entry point should be `lexer`.
4. Files should prefer a single responsibility over a broad utility bucket.
5. If a file grows beyond one responsibility, split it before adding more helpers.

## Review Checklist

Before committing the lexer refactor, verify:

- file names match their responsibility
- function names are consistent with the naming pattern
- root `headers/lexer.h` stays public-only
- internal lexer declarations are centralized
- the build still compiles the lexer module cleanly
- lexer tests still pass unchanged

## Outcome We Want

After this refactor, the lexer should read like a small subsystem with a clear
boundary:

- `lexer.h` for public surface
- `lexer_internal.h` for internal module surface
- one file per job
- one name style across all lexer helpers

