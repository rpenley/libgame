GAMES_DIR = games
BUILD_DIR = build
BIN_DIR   = $(BUILD_DIR)/bin

CC     = gcc
CFLAGS = -Wall -Wextra -std=c99 -I.

$(shell mkdir -p $(BIN_DIR))

GAME_SOURCES = $(wildcard $(GAMES_DIR)/*.c)

all: $(GAME_SOURCES:$(GAMES_DIR)/%.c=$(BIN_DIR)/%)

$(BIN_DIR)/%: $(GAMES_DIR)/%.c libgame.h
	$(CC) $(CFLAGS) -o $@ $<
	@echo "Built $@"

clean:
	rm -rf $(BUILD_DIR)
	@echo "Cleaned build files"

.PHONY: all clean
