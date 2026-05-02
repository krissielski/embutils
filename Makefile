# embutils Makefile
#
# Usage:
#   make              — build all test binaries
#   make build        — build library object files
#   make test         — build and run all tests
#   make clean        — remove build artifacts
#   make test_ring_buffer   — build/run one test suite
#   make test_crc           — build/run one test suite
#   make test_bit_utils     — build/run one test suite
#
# Override the compiler:
#   make CC=arm-none-eabi-gcc   (cross-compile check)
#   make CC=clang

CC      ?= gcc
CFLAGS  ?= -Wall -Wextra -Wpedantic -std=c99 -g
INCLUDES = -Isrc -Itests/unity

# Source files shared by every test binary
UNITY_SRC  = tests/unity/unity.c
LIB_SRCS   = src/ring_buffer.c src/crc.c src/bit_utils.c

BUILD_DIR  = build
BUILD_STAMP = $(BUILD_DIR)/.dir

# Derive object paths
UNITY_OBJ  = $(BUILD_DIR)/unity.o
LIB_OBJS   = $(BUILD_DIR)/ring_buffer.o \
             $(BUILD_DIR)/crc.o \
             $(BUILD_DIR)/bit_utils.o

# Test binaries
TEST_RING_BUFFER = $(BUILD_DIR)/test_ring_buffer
TEST_CRC         = $(BUILD_DIR)/test_crc
TEST_BIT_UTILS   = $(BUILD_DIR)/test_bit_utils

ALL_TESTS = $(TEST_RING_BUFFER) $(TEST_CRC) $(TEST_BIT_UTILS)

# ---- Default target ----

.PHONY: all
all: $(ALL_TESTS)

.PHONY: build
build: $(LIB_OBJS)

# ---- Run all tests ----

.PHONY: test
test: $(ALL_TESTS)
	@echo ""
	@echo "=========================================="
	@echo " Running test_ring_buffer"
	@echo "=========================================="
	./$(TEST_RING_BUFFER)
	@echo ""
	@echo "=========================================="
	@echo " Running test_crc"
	@echo "=========================================="
	./$(TEST_CRC)
	@echo ""
	@echo "=========================================="
	@echo " Running test_bit_utils"
	@echo "=========================================="
	./$(TEST_BIT_UTILS)

# ---- Individual test run targets ----

.PHONY: test_ring_buffer
test_ring_buffer: $(TEST_RING_BUFFER)
	./$(TEST_RING_BUFFER)

.PHONY: test_crc
test_crc: $(TEST_CRC)
	./$(TEST_CRC)

.PHONY: test_bit_utils
test_bit_utils: $(TEST_BIT_UTILS)
	./$(TEST_BIT_UTILS)

# ---- Build rules ----


$(BUILD_STAMP):
	mkdir -p $(BUILD_DIR)
	touch $@

$(UNITY_OBJ): $(UNITY_SRC) tests/unity/unity.h tests/unity/unity_internals.h | $(BUILD_STAMP)
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

$(BUILD_DIR)/ring_buffer.o: src/ring_buffer.c src/ring_buffer.h | $(BUILD_STAMP)
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

$(BUILD_DIR)/crc.o: src/crc.c src/crc.h | $(BUILD_STAMP)
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

$(BUILD_DIR)/bit_utils.o: src/bit_utils.c src/bit_utils.h | $(BUILD_STAMP)
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

$(TEST_RING_BUFFER): tests/test_ring_buffer.c $(UNITY_OBJ) $(BUILD_DIR)/ring_buffer.o | $(BUILD_STAMP)
	$(CC) $(CFLAGS) $(INCLUDES) $^ -o $@

$(TEST_CRC): tests/test_crc.c $(UNITY_OBJ) $(BUILD_DIR)/crc.o | $(BUILD_STAMP)
	$(CC) $(CFLAGS) $(INCLUDES) $^ -o $@

$(TEST_BIT_UTILS): tests/test_bit_utils.c $(UNITY_OBJ) $(BUILD_DIR)/bit_utils.o | $(BUILD_STAMP)
	$(CC) $(CFLAGS) $(INCLUDES) $^ -o $@

# ---- Clean ----

.PHONY: clean
clean:
	rm -rf $(BUILD_DIR)
