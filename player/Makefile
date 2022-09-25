# Makefile for player.c


OBJS = player.o
LIBS = ../support/support.a
CFLAGS =  -Wall -pedantic -std=c11 -ggdb -I../gameboard -I../support
CC = gcc

.PHONY: all clean

all: player.o

player.o: player.h

clean:
	rm -f *~ *.o core