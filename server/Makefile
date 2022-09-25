# Makefile for server.c

S = ../support
G = ../grid
A = ../game
P = ../player
E = ../spectator
C = ../common


OBJS = server.o $G/grid.o $A/game.o $P/player.o $E/spectator.o 
LIBS = -lm
LLIBS = $S/support.a $C/common.a
CFLAGS =  -Wall -pedantic -std=c11 -ggdb -I$G -I$A -I$S -I$P -I$C
CC = gcc

.PHONY: all clean

all: server

server: $(OBJS) $(LLIBS) $(LIBS)
	$(CC) $(CFLAGS) $^ -o $@

server.o: server.c grid.o game.o player.o spectator.o
grid.o: $G/grid.h
game.o: $G/grid.h $P/player.h $E/spectator.h
player.o: $G/grid.h
spectator.o:

clean:
	rm -f *~ *.o core