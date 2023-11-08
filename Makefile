CC = gcc
CFLAGS = -Iinclude
LDFLAGS = /usr/lib/libcs50.a /usr/lib/x86_64-linux-gnu/libz.a
SRC_DIR = src
BIN_DIR = bin

all: $(BIN_DIR)/main

$(BIN_DIR)/main: $(SRC_DIR)/main.c $(SRC_DIR)/unzip.c $(SRC_DIR)/spec.c $(SRC_DIR)/parse.c $(SRC_DIR)/utils.c
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

clean:
	rm -f $(BIN_DIR)/main
