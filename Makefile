
# file: Makefile

SOURCES := src/main.c src/list.c src/util.c
CFLAGS := -Wall -Werror -Wextra -Os -std=c2x

all: build

build:
	@cc -o todo $(CFLAGS) -I./src $(SOURCES)

install: build
	@sudo cp todo /usr/local/bin/

uninstall:
	@sudo rm -rf /usr/local/bin/todo
	@rm -rf ~/.local/state/todo

clean:
	@rm -f todo

reset: clean uninstall install

.PHONY: all build install uninstall clean reset
