# AGENTS.md — embutils

---

## Project overview

**embutils** is a small embedded C utility library.
It has no external dependencies and no hardware requirements — everything
compiles and runs on a standard Linux/macOS/WSL host.

Modules:

| Path | Purpose |
|---|---|
| `src/ring_buffer.h/.c` | Fixed-size FIFO byte ring buffer |
| `src/crc.h/.c` | CRC-8 (Dallas) and CRC-16 (CCITT-FALSE) |
| `src/bit_utils.h/.c` | Bit macros, byte-packing, bitfield extract |
| `tests/unity/` | Unity test framework (vendored, do not modify) |
| `tests/test_*.c` | One test file per module |

---

## How to build and test

```bash
# Build all test binaries
make

# Build and run all tests (required before every commit)
make test

# Run a single suite during development
make test_ring_buffer
make test_crc
make test_bit_utils

# Clean
make clean
```

**Always run `make test` and confirm all tests pass before finishing a task.**
If tests fail, fix the code — do not modify the tests to make them pass
unless the test itself is wrong (and explain why in the commit message).

---

## Code style

- C99, no C11 features (keeps it portable to older embedded toolchains).
- Tabs for indentation (follow existing files).
- All public functions must have a doc-comment in the `.h` file.
- No dynamic memory allocation (`malloc`/`free`) — this is embedded code.
- No global mutable state in library code.
- Compiler flags include `-Wall -Wextra -Wpedantic` — code must compile
  cleanly with zero warnings.

---

## Adding a new module

1. Create `src/<name>.h` and `src/<name>.c`.
2. Add a doc-comment to every public function in the header.
3. Create `tests/test_<name>.c` using the Unity pattern (see existing tests).
4. Add the module to `Makefile`:
   - Add `$(BUILD_DIR)/<name>.o` build rule.
   - Add a `$(BUILD_DIR)/test_<name>` link rule.
   - Add `test_<name>` to `ALL_TESTS`.
5. Run `make test` to verify everything builds and passes.

---

## Adding tests

- One test file per module.
- Test function names follow `test_<function>_<condition>` (e.g.,
  `test_rb_pop_fails_when_empty`).
- Use `TEST_ASSERT_EQUAL_HEX8/16/32` for byte/register values — hex
  output makes failures easier to read.
- Include both happy-path and edge-case tests (empty, full, wrap-around,
  zero-length input, etc.).

---

## Workflow

- Branch from `main` for every change.
- Branch name convention: `<type>/<short-description>` — e.g.,
  `feat/add-crc32`, `fix/ring-buffer-wrap`, `test/crc-edge-cases`.
- Commit messages: imperative mood, ≤72 chars subject line — e.g.,
  `fix: handle zero-length input in crc8`.
- All CI checks must pass before merging to `main`.

---

## What NOT to do

- Do not modify files under `tests/unity/` — it is vendored upstream code.
- Do not use `malloc`, `calloc`, or `free` anywhere in `src/`.
- Do not add compiler warnings suppressions (`#pragma GCC diagnostic`)
  to hide real issues — fix the root cause instead.
- Do not commit binaries or build artifacts — see `.gitignore`.
