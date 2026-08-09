CC = gcc
CFLAGS = -Wall -Wextra -O2 -Iinclude
LDFLAGS =

SRC_DIR = src
TEST_DIR = tests
BUILD_DIR = build
TARGET = muszla

SRC = $(wildcard $(SRC_DIR)/*.c)
TEST = $(wildcard $(TEST_DIR)/test_*.c)

SRC_OBJ = $(patsubst $(SRC_DIR)/%.c,$(BUILD_DIR)/%.o,$(SRC))
TEST_OBJ = $(patsubst $(TEST_DIR)/%.c,$(BUILD_DIR)/%.o,$(TEST))
TEST_BINS = $(patsubst $(TEST_DIR)/%.c,$(BUILD_DIR)/%,$(TEST))

.PHONY: all clean test

all: $(TARGET)

$(TARGET): $(SRC_OBJ)
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)

$(BUILD_DIR)/%: $(BUILD_DIR)/%.o $(SRC_OBJ)
	@mkdir -p $(dir $@)
	$(CC) $^ -o $@ $(LDFLAGS)

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR)/%.o: $(TEST_DIR)/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

test: $(TEST_BINS)
	@if [ -z "$(TEST_BINS)" ]; then \
		exit 0; \
	fi
	@set -e; \
	for b in $(TEST_BINS); do \
		echo "Running $$b"; \
		$$b; \
	done


clean:
	rm -rf $(BUILD_DIR)
