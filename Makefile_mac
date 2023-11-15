CC = clang
CFLAGS = -Iinclude -DLOGGING
LDFLAGS = /usr/local/lib/libcs50.a /Users/kemal/C/zlib_download/zlib-1.3/libz.a
SRC_DIR = src
BIN_DIR = bin

all: $(BIN_DIR)/data-prism-mac

$(BIN_DIR)/data-prism-mac: $(SRC_DIR)/main.c $(SRC_DIR)/unzip.c $(SRC_DIR)/spec.c $(SRC_DIR)/parse.c $(SRC_DIR)/utils.c
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

clean:
	rm -f $(BIN_DIR)/data-prism-mac
