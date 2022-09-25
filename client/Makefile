# Makefile for client.c

S = ../support

OBJS = client.o
LIBS = -lncurses
LLIBS = $S/support.a
CFLAGS =  -Wall -pedantic -std=c11 -ggdb -I$S
CC = gcc
MAKE = make

.PHONY: all clean

all: client

client: $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) $(LLIBS) $(LIBS) -o client

client.o: client.c

clean:
	rm -f *~ *.o core
