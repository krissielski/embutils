# embutils

A small embedded C utility library used as the sample project for the
GitHub Actions CI/CD tutorial. It contains three self-contained modules
that are common in real embedded firmware:

| Module | What it does |
|---|---|
| `ring_buffer` | Fixed-size, FIFO byte ring buffer |
| `crc` | CRC-8 (Dallas/Maxim) and CRC-16 (CCITT-FALSE) |
| `bit_utils` | Bit macros and byte-packing helpers |

Tests are written with the [Unity](https://github.com/ThrowTheSwitch/Unity)
framework (vendored — no install required).

---

## Building and running tests

### Prerequisites

- GCC (or any C99-compatible compiler)
- GNU Make
- A POSIX-compatible shell (Linux, macOS, or WSL on Windows)

### Commands

```bash
# Build all test binaries
make

# Build and run all tests
make test

# Run a single test suite
make test_ring_buffer
make test_crc
make test_bit_utils

# Remove build artifacts
make clean
```

### Cross-compiler check

You can verify the code compiles cleanly for a different target by
overriding `CC`:

```bash
make CC=arm-none-eabi-gcc
```

---

## Project structure

```
embutils/
├── src/
│   ├── ring_buffer.h / .c
│   ├── crc.h / .c
│   └── bit_utils.h / .c
├── tests/
│   ├── unity/              ← Unity framework (vendored)
│   │   ├── unity.h
│   │   ├── unity_internals.h
│   │   └── unity.c
│   ├── test_ring_buffer.c
│   ├── test_crc.c
│   └── test_bit_utils.c
├── .github/
│   └── workflows/
│       └── build.yml       ← GitHub Actions CI workflow
├── Makefile
└── README.md
```


