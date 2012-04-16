CC=gcc

INCLUDES= -Iinclude
CFLAGS= $(INCLUDES) -g -Wall

SRCS= $(wildcard src/*.c)
OBJS = $(patsubst %.c,%.o,$(SRCS))

%.c.o:
	$(CC) $(CFLAGS) -c $*.

all: $(OBJS)
	gcc -o skeema $(OBJS)

