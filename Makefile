CC=gcc

INCLUDES= -Iinclude
CFLAGS= $(INCLUDES) -g -Wall -DDEBUG -std=c99 -pedantic

BIN= skeema
SRCS= $(wildcard src/*.c)
OBJS = $(patsubst %.c,%.o,$(SRCS))

%.c.o:
	$(CC) $(CFLAGS) -c $*.

all: $(OBJS)
	gcc -o $(BIN) $(OBJS)

clean:
	rm -f $(BIN) $(OBJS)

