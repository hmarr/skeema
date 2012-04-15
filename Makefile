all: debug

release:
	gcc -O2 -o skeema skeema.c object.c

debug:
	gcc -Wall -g -DDEBUG -o skeema skeema.c object.c
