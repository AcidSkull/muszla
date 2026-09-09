CC = gcc
CFLAGS = -Wall -Wextra -O2 -Iinclude
LDFLAGS =

SRC_DIR = src
BUILD_DIR = build
TARGET = muszla

SRC = $(wildcard $(SRC_DIR)/*.c)
SRC_OBJ = $(patsubst $(SRC_DIR)/%.c,$(BUILD_DIR)/%.o,$(SRC))

.PHONY: all clean

all: $(TARGET)

$(TARGET): $(SRC_OBJ)
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf $(BUILD_DIR) $(TARGET)