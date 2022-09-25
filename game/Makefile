# Makefile for game.c

S = ../support
G = ../grid
V = ../server
P = ../player
E = ../spectator


OBJS = game.o $P/player.h $E/spectator.h $G/grid.h
LIBS = -lm
LLIBS = $S/support.a 
CFLAGS =  -Wall -pedantic -std=c11 -ggdb -I$S -I$P -I$E -I$G -I$V
CC = gcc

.PHONY: all clean

all: game.o

game.o: game.h $P/player.h $E/spectator.h $G/grid.h 
player.o: $P/player.h ../support/message.h ../grid/grid.h ../support/log.h
spectator.o: $E/spectator.h ../support/message.h ../support/log.h
grid.o: $G/grid.h
server.o: $G/grid.h ../support/message.h ../support/log.h game.h 


clean:
	rm -f *~ *.o core