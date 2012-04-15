all: debug

release:
	gcc -O2 -o skeema skeema.c utils.c object.c scanner.c parser.c

debug:
	gcc -Wall -g -DDEBUG -o skeema skeema.c utils.c object.c scanner.c parser.c
