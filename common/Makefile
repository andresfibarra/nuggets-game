# Makefile for CS50 Nuggets common library
#
# Group 6 'Placeholders'
# March 2022

OBJS = file.o
LIB = common.a
CFLAGS = -Wall -pedantic -std=c11 -ggdb $(FLAGS)
CC = gcc
MAKE = make

$(LIB): $(OBJS)
	ar cr $(LIB) $(OBJS)

file.o: file.h

clean:
	rm -f core
	rm -f $(LIB) *~ *.o
