# Makefile for CS50 Nuggets
#
# Team 6 - Placehoders; Winter 22

.PHONY: all clean

############## default: make all libs and programs ##########
all: 
	make -C client
	make -C common
	make -C game
	make -C player
	make -C server
	make -C support

############## clean  ##########
clean:
	rm -f *~
	make -C client clean
	make -C common clean
	make -C game clean
	make -C player clean
	make -C server clean
	make -C support clean
