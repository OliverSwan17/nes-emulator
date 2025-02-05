CC = gcc
CFLAGS = -Wall -Wextra -Iinclude -I/mingw64/include -g
LDFLAGS = -L/mingw64/lib -lpdcurses

SRC_DIR = src
INC_DIR = include
BIN_DIR = bin
OBJ_DIR = obj

SRCS = $(wildcard $(SRC_DIR)/*.c)
OBJS = $(SRCS:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)
TARGET = $(BIN_DIR)/emulator

$(TARGET): $(OBJS) | $(BIN_DIR)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c | $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJ_DIR) $(BIN_DIR):
	mkdir -p $@

clean:
	rm -rf $(OBJ_DIR)/* $(BIN_DIR)/*

run: $(TARGET)
	./$(TARGET)

.PHONY: clean run
