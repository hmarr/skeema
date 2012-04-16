CC=gcc

INCLUDES= -Iinclude
CFLAGS= $(INCLUDES) -g -Wall -DDEBUG -std=c99 -pedantic

BIN= skeema
OBJ_SRCS= $(wildcard src/sk_objects/*.c)
SRCS= $(wildcard src/**.c)
OBJ_OBJS= $(patsubst %.c,%.o,$(OBJ_SRCS))
OBJS = $(patsubst %.c,%.o,$(SRCS))

%.c.o:
	$(CC) $(CFLAGS) -c $*

all: $(OBJ_OBJS) $(OBJS)
	gcc -o $(BIN) $(OBJ_OBJS) $(OBJS)

clean:
	rm -f $(BIN) $(OBJ_OBJS) $(OBJS)

