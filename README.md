# Group 6 'Placeholders'
## Andres Ibarra, Elliot Krewson, Rebecca Liu, Leina Sato

# Nuggets

This repository contains the code for the CS50 "Nuggets" game, in which players explore a set of rooms and passageways in search of gold nuggets.
The rooms and passages are defined by a *map* loaded by the server at the start of the game.
The gold nuggets are randomly distributed in *piles* within the rooms.
Up to 26 players, and one spectator, may play a given game.
Each player is randomly dropped into a room when joining the game.
Players move about, collecting nuggets when they move onto a pile.
When all gold nuggets are collected, the game ends and a summary is printed.

## Materials provided

See the [support library](support/README.md) for some useful modules.

See the [maps](maps/README.md) for some draft maps.

## Assumptions
We do not assume anything that has not already been provided in the 
assignment README.md as well as the Requirements Spec.

## Directories

### Client
The client acts as an interactive module between the player and the server, sendiing and
recieving messages throughout the game. The client directory contains:
1. `client.c` - a program defining methods that handle and produce messages
2. `Makefile` - builds the client executable program
3. `.gitignore` - Removes specified files from git

### Server
The server runs the functionality of the game, handling all the moving parts of each
player and game content. It takes messages specified by the client and acts upon them.
It also constructs methods and sends them back to client.
The server directory contains the following:
1. `server.c` - a program defining server functions
2. `Makefule` - builds the server executable
3. `.gitignore` - Removes specified files from git

### Game
The game holds valuable information about the game currently in progress. This information
includes the array of player structs, the spectator, the master grid, the number of players,
the amount of gold collected in total and the piles of available gold. The game houses functions
that can get and manipulate the data listed above. 
The game directory contains the following:
1. `game.c` - a file containing accessor methods for game
2. `game.h` - a headerfile defining all fucntions in game.c
3. `Makefile` - defines the files needed to implement game

### Player
The player houses information for each participant (except for the spectator) in the game.
The player struct contains the player's ID, their real name, address, current position,
amount of gold collected, a grid containing visible points, and a boolean to define their
playing status. The module contains accessor methods for the player struct. The directory
contains the following:
1. `player.c` - a file containing the accessor methods for the player struct
2. `player.h` - a headerfile defining the player struct and each method in the module
3. `Makefile` - defines the files needed to implement player

### Spectator
The spectator acts as a birds-eye view of the entire game. The spectator can view the
master grid, see tht total amount of gold collected, and can be replaced by the most
recent client to return a SPECTATE message. The spectator struct only contains an address.
The spectator directory contains the following:
1. `spectator.c` - a file containing the accessor methods for the player struct
2. `spectator.h` - a headerfile defining the spectator struct and its functions

### Grid
The grid is a module that contains the grid struct and methods that handle the player
and master grids. The struct contains a 2d array of characters representing the data in
the map, an int containg the rows in the map, and an int containing the columns in the map.
Important functions in the grid module include `grid_loadMap` and `grid_calculateVisibility`.
The grid directory contains the following:
1. `grid.c` - a file containing the functions necessary for grid
2. `grid.h` - a headerfile defining the grid struct and functions
3. `gridTester.c` - a file to test loadMap, distributeGoldm and calculateVisibility
4. `Makefile` - builds the gridTester executable and defines files needed to implement
5. `.gitignore` - Removes specified files from git

### Common
A library containing files that can be used by any file.
The common library contains:
1. `file.c` - a file containing functions to read .txt files
2. `file.h` - the file headerfile
3. `Makefile` - builds the `common.a` file, or the common library

## Testing outcomes
1. When a player travels through a tunnel, '#'s are changed to '.'s
2. When multiple players play, their icons get mixed up and oftentimes will have
a trailing letter behind them as they move
3. You can see gold in rooms where you currently aren't
4. When a player tries to move into another player's spot, it does not act correctly
5. Each player sees themselves as their letter when they first join, before moving, not as the @ symbol
6. spectator cannot quit with 'Q' must use 'control-C'

