# CS50 Nuggets
## Design Spec
### Placeholder, Winter 2022

According to the [Requirements Spec](REQUIREMENTS.md), the Nuggets game requires two standalone programs: a client and a server.
Our design also includes the grid modules.
We describe each program and module separately.
We do not describe the `support` library nor the modules that enable features that go beyond the spec.
We avoid repeating information that is provided in the requirements spec.

## Player

The *client* acts in one of two modes:

1. *spectator*, the passive spectator mode described in the requirements spec.
2. *player*, the interactive game-playing mode described in the requirements spec.

### User interface

### Functional decomposition into modules

`player_new` instantiates a player module for use in the game
`player_move` validates a key for movement and moves accordingly
`player_send` sends messages between players

 
### Pseudo code for logic/algorithmic flow

player_new:
```
instantiate a player
return it to the caller
```

player_move:
```
recieve a key
validate the player can move there
move the player in that direction
```

player_send:
```
send the message given to the player
```
### Major data structures

*players_t*, a struct with at least two fields:
* array representing visibility
* location
* player ID
* boolean to represent whether or not they are still playing
* amount of gold collected

---

## Server

### User interface
See the requirements spec for the command-line interface.
There is no interaction with the user.

### Inputs and outputs
_Input_: A map file is the only input. It is a text file with exactly NR lines and in which every line has exactly NC characters. The dots represent empty spots; gold pieces and players may occupy these. The hashes represent passageways; players may occupy these. One room is non-convex. Some passageways bend, and some fork. Some rooms have multiple entrances and some rooms have passages that do not lead anywhere.

_Output_: Printed summary starting with the phrase "GAME OVER:", followed by a simple textual table with one row for every player (including any who quit early) and with three columns: player letter, player purse, and player name.

_Log files_: Logging to stderr useful information to save in logfile.

### Functional decomposition into modules

1. main, parses arguments and initializes other functions
2. initializeGame
3. handleMessage
4. movePlayer
5. gameOver 


### Pseudo code for logic/algorithmic flow

The server will run as follows:

#### main
```
execute from a command line per the requirement spec
parse the command line, validate parameters
call initializeGame() to set up data structures
initialize the 'message' module
print the port number on which we wait
call message_loop(), to await clients
check if max number of player and assign player id
if client is player
    handleMessage
if client is spectator
    updateMainGrid
call gameOver() to inform all clients the game has ended
clean up
```

#### handleMessage
```
keep checking for message from players
if Q
  remove player from game
  send message to all other players summary of player who is quitting
if not Q
  if the new location is in hashtable of possible spots
    if there is gold
      update number of gold for that player
      update total number of gold left
  communicate back to player
```

#### initializeGame
```
initialize a game struct
return it to the caller
```

#### movePlayer
```
validate the parameters
validate the player's move
move the player
update the master grid
update each player's grids
```

#### gameOver
```
if number of nuggets = 0
  send game over message to all players including summary of game
```

### Major data structures

* Array of booleans to represent visibility
* grid struct to represent map

---

## Grid module

The grid module stores information about the map, and provides functions to create data structures representing the map

### Functional decomposition

1. grid_init
2. loadMap, initializes the structures associated to the map
3. grid_distributeGold
4. grid_calculateVisibility
5. grid_updateMaster
6. grid_updatePlayers

### Pseudo code for logic/algorithmic flow

#### grid_init
```
initialize the grid struct
initialize the 2d array
```

#### loadMap
The `loadMap` function reads the map file into a string and generate the locations of the gold.
```
open the map file
set each char equivilent to the corresponding position in the map file
grid_distributeGold()
```

#### grid_distributeGold
builds an array for the gold and distributes it across the map
```
loop through the pile of gold provided
generate a random point
find a point that is a .
put the gold pile at that point
```

#### calculateVisibility
```
Given player location and past locations
calculate what should be displayed (yet to figure out how exactly to calculate)
store visbility points in an array
return array
```

### Major data structures

*grid*: The *grid* module holds information about the grid itself. It has at least 2 fields:
* data: a 2d char array representing the characters in the map
* rows: amount of rows in the file
* columns: amount of columns in the file

## Game

The game module stores information about the state of the game, the players inside the game,
and any visibility matters within each game.

### Functional decomposition
1. game_init
2. game_addPlayer
3. game_incrementGold

### Psuedocode for logic/algorithmic flow

#### game_init:
```
initialize the game struct
initialize each player in the game
load the master grid
```

#### game_addPlayer:
```
check if a player can join the game
add the player to the array
```

#### game_incrementGold
```
add the amount provided in parameter to the total gold collected
```

### Major Data Structures
*game*: where the `game` struct holds information about the game. It holds multiple fields:
* array of players
* one spectator
* the master grid
* array containing gold piles

## Client

### User Interface
See the requirements spec for both the command-line and interactive UI.

command line:
```
./client hostname port [playername]
```
### Inputs and outputs

Input: the player inputs keystroke to either move around the game or quit, as detailed below.

```
Player Keystrokes:
  h -> move left
  l -> move right
  j -> move down
  k -> move up
  y -> move diagonally up and left
  u -> move diagonally up and right
  b -> move diagonally down and left
  n -> move diagonally down and right
  Q -> quit the game

Spectator Keystrokes:
  Q -> quit the game
```

Output: Shows a status line of the display on the first line and the game grid on subsequent lines.

Log: log to stderr useful information that can be saved in a logfile

### Functional decomposition into modules

We anticipate the following methods

1. client_init
2. client_initNetwork
3. client_delete
4. handleInput
5. handleMessage
6. handleGrid
7. handleGold
8. handleDisplay
9. handleQuit
10. handleError

### Psuedocode of logic/algorithmic flow

#### client_init
```
set up nCurses
draw the screen
```

#### client_initNetwork
```
build a key message from user input
send it to the server
```

#### handleMessage
```
handleGrid
handleGold
handleDisolay
or handleQuit
```
## Division of Labor

Andres: player module
Rebecca: server module
Leina: server module
Elliot: grid module

Everyone is expected to help with debugging
