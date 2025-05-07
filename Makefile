CC = gcc
CFLAGS = -Wall -Wextra -g
SRC_DIR = src
INC_DIR = inc
BUILD_DIR = bin
TARGET = jp

SOURCES = $(SRC_DIR)/main.c $(SRC_DIR)/json.c
OBJECTS = $(SOURCES:$(SRC_DIR)/%.c=$(BUILD_DIR)/%.o)

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CC) $(CFLAGS) -o $@ $^ -I$(INC_DIR)

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@ -I$(INC_DIR)

clean:
	rm -rf $(BUILD_DIR) $(TARGET)

.PHONY: all clean