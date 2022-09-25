# Makefile for CS50 NUGGETS 'Grid' Module
#
# Group 6 'Placeholders'
# March 2022

CC = gcc
CFLAGS = -Wall -pedantic -std=c11 -ggdb -I../common

OBJS = gridTester.o grid.o ../common/file.h
LIBS = -lm
LLIBS = ../common/common.a
VALGRIND = valgrind --leak-check=full --show-leak-kinds=all

gridTester: $(OBJS) $(LLIBS)
	$(CC) $(CFLAGS) $(OBJS) $(LLIBS) $(LIBS) -o gridTester

gridTester.o: grid.h
grid.o: grid.h ../common/file.h

clean:
	rm -f gridTester
	rm -f *~ *.o
	rm -rf *.dSYM
