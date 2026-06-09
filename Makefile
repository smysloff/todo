
# file: Makefile

all:
	@cc -o todo -Wall -Werror -Wextra -Os main.c

clean:
	@rm -rf todo todo.list

.PHONY: all clean
