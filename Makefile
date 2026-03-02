all:
	cc cb.c -Wall -Wextra -Werror -o cb

install:
	mkdir -p ~/.local/bin
	cp ./cb ~/.local/bin

.PHONY: all
