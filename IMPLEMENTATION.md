# CS50 Nuggets
## Implementation Spec
### Placeholders, Winter, 2022


According to the [Requirements Spec](REQUIREMENTS.md), the Nuggets game requires two standalone programs: a client and a server.
Our design breaks the client program into two parts, player and spectator, and also includes grid and game modules.
We describe each program and module separately.
We do not describe the `support` library nor the modules that enable features that go beyond the spec.
We avoid repeating information that is provided in the requirements spec.

## Plan for division of labor

Andres: player module
Rebecca: spectator module, game module
Leina: server module 
Elliot: grid module

Everyone is expected to help with debugging and testing.

## Player

The player module holds information about each individual player, as well as functions to play the game and to communicate with the server

### Data structures

player_t, a struct with the following:

* `playerID` - the char holding playerID used by the server
* `realName` - string holding player's real name; used at the end of the game when displaying final results
* `address` - socket player is occupying; used to send messages
* `posX` - player's x location
* `posY` - player's y location
* `visibleGrid` - a grid containing all visible points
* `isStillPlaying` - boolean keeping track of if the player is still playing

### Definition of function prototypes

> For function, provide a brief description and then a code block with its function prototype.
> For example:

A function to parse the command-line arguments, initialize the game struct, initialize the message module, and (BEYOND SPEC) initialize analytics module.

```c
static int parseArgs(const int argc, char* argv[]);
```

A function to initialize a new player
```c
player_t* player_new(char letter, char* realName, addr_t address, int posX, int posY); 
```

A function to move the player
```c
void player_move (int dx, int dy); 
```

A function to update the visibility grid of each player
```c

A function to allow the player to receive message from server and determine what to do.
```c
void player_receieveMessage();
```

A function to update the players screen according to their location.
```c
void player_updateOutput();
```

A function to delete the player
```c
void player_delete(player_t* player); 
```

### Detailed pseudo code


#### `parseArgs`:

	validate commandline
	initialize message module
	print assigned port number

#### `player_new`:
    
    try allocating memory for player struct
    initialize new player using given inputs

#### `player_move`:

    add given dx to player's current x location
    add given dy to player's current y location

#### `player_updateGrid`:

  validate the player provided in the parameters
  set the visibility grid to the grid provided

#### `player_updateGold`:

  validate the player provided in the parameters
  set the player's gold count to the amount provided
\
#### `player_delete`: 

    free all memory allocated associated with player
---

## Server

### Data structures

* Array of booleans to represent visibility
* grid struct to represent map

### Definition of function prototypes

> For function, provide a brief description and then a code block with its function prototype.
> For example:

A function to parse the command-line arguments, initialize the game struct, initialize the message module, and (BEYOND SPEC) initialize analytics module.

```c
static int parseArgs(const int argc, char* argv[]);
```

A function to initialize the game, setting up the map and the random piles of gold.
```c
void initializeGame(FILE* mapFile);
```

A function to handle the messages receieved by players and spectators
```c
void handleMessage(const addr_t* sendingAddr, char* message); 
```

A function that handles the PLAY message
```c
void handlePlay(const char* name, const addr_t playerAddress, game_t* game);
```

A function that handles the SPECTATE message
```c
void handleSpectate(const addr_t spectatorAddress, game_t* game);
```

A function to generate the initial location for player
```c
char* generateLocation(grid_t* grid);
```

A function that handles the KEY message
```c
void handleKey(const char key, const addr_t playerAddress, game_t* game);
```

A function that handles when the Q key is pressed
```c
void handleQ(const addr_t playerAddress, game_t* game);
```

A function to find the player struct with the given player address
```c
player_t* findPlayer, const addr_t playerAddress, game_t* game);
```

A function that finds a spectator struct with the given address
```c
spectator_t* findSpectator(const addr_t address, game_t* game);
```

A function that moves a player in the given direction
```c
bool movePlayer(player_t* player, int dx, int dy, game_t* game);
```

A functiom to tell whether a move is valid
```c
bool validateMove(int x, int y, int dx, int dy, gane_t* game);
```

A function to tell whether a given point is a wall
```c
bool isWall(grid_t* masterGrid, int updatedX, int updatedY, int dx, int dy, game_t* game);
```

A player to update the player struct information
```c
void updatePlayerInfo(player_t* player, int dx, int dy, game_t* game);
```

A function to find the number if gold found in a given location
```c
int getNumGold(grid_t* masterGrid, int updatedX, int updatedY, game_t* game);
```

A function to handle whenever a player collects gold
```c
void handleGold(player_t* player, int numGoldCollected, game_t* game);
```

A function to update game struct info on gold collected
```c
void updateGoldCounter(int numGoldCollected, game_t* game);
```

A function to change the display of players whenever a player moves
```c
void handleDisplay(game_t* game, player_t* player, int old_x, int old_y, int new_x, int new_y);
```

A functiom to broadcast the DISPLAY message to players
```c
void broadcastDisplay(game_t* game);
```

A function to end the game when all the gold is collected
```c
void gameOver(game_t* game);
```

A functiom to construct and send OK message
```c
void sendOK(const addr_t playerAddress, char playerID);
```

A function to construct and send GRID message
```c
void sendGrid(const addr_t playerAddress, grid_t* grid);
```

A function to construct and send GOLD message
```c
void sendGold(const addr_t playerAddress, const int n, const int p, const int r);
```

A function to construct and send DISPLAY message
```c
void sendDisplay(addr_t address, grid_t* grid);
```

A function to construct and send QUIT message
```c
void sendQuit(const addr_t playerAddress, char* explaination);
```

A function to construct and send ERROR message
```c
void sendError(const addr_t playerAddress, char* explaination);
```

A function that splits given message into array of tokens.
```c
char** tokenize(char* mes);
```

A function to delete the array of tokens created by tokenize
```c
void deleteTokens(char** tokens);
```

### Detailed pseudo code


#### `parseArgs`:

	validate commandline
	verify map file can be opened for reading
	if seed provided
		verify it is a valid seed number
		seed the random-number generator with that seed
	else
		seed the random-number generator with getpid()

#### `initializeGame`

Checks that mapFile isn't NULL calls game_init

#### `handleMessage`

Handles messages receieved by players and spectators. Pseudocode: 
```
call tokenize() on message
  if first word in message = "KEY"
  call grid_movePlayer
if first word in message = "SPECTATOR"
  call spectator_new
  call game_addspectator
if first word in message = "QUIT"
  set that player struct's isStillPlaying to false
if first word in message = "PLAYER"
    call player_new
    call game_addplayer
```

#### `handlePlay`
```
validate the parameters
initialize the player struct
update the visibleGrid
if the player can be added to the game
  send OK message
else
  quit the game
```

#### `handleSpectate`
```
validate the parameters
initialize the spectator
if there is a spectator
  replace the old spectator with the new one
```

#### `generateLocation`
```
generate random point in the grid
regenerate until the point is a .
return a string containing the point values
```

#### `handleKey`
```
read the key provided
switch the key
  evaluate each possible case
    handle the key
```

#### `handleQ`
```
find the player with the address provided
if spectator, find spectator with given address
sendQuit for the player
```

#### `findPlayer`
```
for each player
  if the address equals a player in the players array
    return the player
return NULL if it gets to this point
```

#### `findSpectator`
```
validate the parameters
check if the address provided is equivilent to spectator address
return NULL if otherwise
```

#### `movePlayer`
```
validate the parameters
validate the points provided
return true if successful, false otherwise
```

#### `validateMove`
```
if the point provided is a wall
  return true
return false
```

#### `isWall`
```
if the point returns a {|,-,+, } return true
return false
```

#### `updatePlayerInfo`
```
validate the parameters
move the player
update the visibleGrid of the player
```

#### `getNumGold`
```
if the location is gold
  return the amount of gold found
return 0 if non-gold point
```

#### `handleGold`
```
send the GOLD message to all players
send the GOLD message to the spectator
```

#### `updateGoldCounter`
```
for each player in the game
  refresh their grid
broadcast display
```

#### `broadcastDisplay`
```
for each player in the game
  send the display message
send the display message to the spectator
```

#### `gameOver`
```
create a summary string
add quit message to summary
print the summary of the player's round to each player
delete the game
```

#### `tokenize`

Split the message into an array of strings


---

## Spectator module

### Data structures


spectator_t, a struct that holds the addr_t of the spectator

### Definition of function prototypes

Detailed descriptions of each function are provided as a paragraph comment in `spectator.h` .

```c
  spectator_t* spectator_new(addr_t* address); 
  void spectator_send(spectator_t* spectator, char* message); 
  void spectator_delete(spectator_t* spectator); 

```

### Detailed pseudo code

#### spectator_new

Allocates memory for a spectator struct and initialized values. Returns a pointer to the newly minted spectator

#### spectator_send

Function to send message to spectator

#### spectator_delete

Frees memory allocated to spectator and deletes it

---

## Client

### Data structures

struct client_t, where the struct contains
- playerID
- real name
- is player
- server address

### Definition of function prototypes
```c
static int parseArgs(const int argc, char* argv[], client_t** client);
void initClient(client_t* client);
void initNetwork(client_t* client);
bool handleInput(void* arg);
bool handleMessage(void* arg, const addr_t from, const char* message);
void handleGrid(const char* message);
void handleGold(const char* message, client_t* client);
void handleDisplay(const char* message);
void handleQuit(const char* message);
void client_delete(client_t* client);
```

### Detailed Psuedocode
#### parseArgs:
```
if there are less than 2 or more than 3 arguments
  return an error
if the client is null
  return an error
if a port number is not retrievable
  return an error
if unsuccessfull set server address
  return an error
if correct amount of arguments
  initialize the variables
```

#### initClient:
```
set up ncurses
initialize network
draw screen
```

#### initNetwork:
```
if the client is a player
  send PLAY to server
if not
  set the message to SPECTATE
```

#### handleMessage:
```
break down each possible message
handle each message using functions below
```

#### handleGrid:
```
validate the size of the user window
resize the window until it can fit the grid
```

#### handleGold:
```
Print a message containing info about gold status
Print gold message specific to player
Print total nuggets unclaimed to spectator
```

#### handleDisplay
```
update the string based on parameters
```

#### handleQuit
```
add a quit message to stderr
```

#### handleError
```
print an error message
refresh the page
```

#### client_delete
```
free the memory allocated by client
```
---

## Grid module


### Data structures

> struct grid_t, where the struct contains
* gridArray - a 2D Array of chars representing each point in the map
* goldCounter - an int counting the amount of gold collected
* numberPiles - an int representing the amount of gold piles within the map
* goldPiles - an array of ints representing the amount of gold in each pile

### Definition of function prototypes

```c
int grid_getRows(grid_t* grid);
int grid_getCols(grid_t* grid);
char grid_getData(grid_t* grid;

grid_t* grid_init(void);
void grid_loadMap(grid_t* grid, char* mapName);
void grid_distributeGold(grid_t* grid, int* piles);
int* grid_createGoldPiles(void);
bool handleTopLeft(grid_t* grid, int origR, int origC, int pointR, int pointC);
bool handleTopRight(grid_t* grid, int origR, int origC, int pointR, int pointC);
bool handleBottomLeft(grid_t* grid, int origR, int origC, int pointR, int pointC);
bool handleBottomRight(grid_t* grid, int origR, int origC, int pointR, int pointC);
bool handleSameRow(grid_t* grid, int origR, int origC, int pointR, int pointC);
bool handleSameColumn(grid_t* grid, int origR, int origC, int pointR, int pointC);
bool isItWall(char c);
char* grid_toString(grid_t* grid);
void grid_print(grid_t* grid);
void grid_updateMaster(grid_t* master, int oldX, int oldY, int newX, int newY);
void grid_refresh(grid_t* grid, char playerID, int oldX, int oldY, int newX, int newY);
int randInt(int low, int high);
```
### Detailed pseudo code
grid_init:
```
allocate memory for grid
return the grid
```

grid_playerGridInit:
```
set rows and columns to rows and columns from masterGrid
allocate memory for grid data
set each position in the grid to a ' '
```

grid_loadMap:
```
read map file line by line
  add each character and space to string until newline reached (keeping track of which index)
  if character is a dot (.)
    insert value of index into hashtable (initially pointing to 0)
    insert value index into a temporary list of dot locations
pick a random number between 10 and 30 -> i
  choose locations of gold
  loops through locations of gold and place random number of nuggets
hashtable index -> amount of gold there
```

grid_distributeGold:
```
build an array that represents the size of each gold pile
for each number in that array
    pull a random point represented by a dot
    replace the dot with the number of gold
```

grid_createGoldPiles:
```
allocate memory for int array
while the gold allocated is less than 250
  generate a random number
  add that number to a gold pile
subtract the excess gold from a pile big enough
return the int array
```

grid_calculateVisibility:
```
for each row
    for each point in that row
        if the point has not been seen
            calculate a line to that point from origin
            evaluate all points within that line by checking upper and lower neighbors
                if both are room points
                    game point is visible
                else
                    continue-> move to next point
              
```

isItWall:
```
if the character is {#,|,-, ,+} return true
else return false
```

grid_print:
```
for each row in the grid
  print the string associated with that row
```

grid_updateMaster:
```
set the value of the new point to the value of the old point
set the value of the old point to .
```

grid_refresh:
```
if the value of the old point equals the playerID
  set the old point to .
if the new point is not a wall
  set the new point to the PlayerID
```

---

## Game module

### Data structures

struct game_t, a struct with fields containing:
- array of players
- spectator
- master grid

### Definition of function prototypes

intialize game and allocate memory
```c
void game_init(FILE* map)
```

handle spectators joining
```c
void game_addSpectator(spectator_t x)
```

handle players joining
```c
void game_addPlayer(player_t x);
```


=======

spectator_t, a struct that holds the addr_t of the spectator

### Definition of function prototypes

Detailed descriptions of each function are provided as a paragraph comment in `spectator.h` .

```c
  spectator_t* spectator_new(addr_t* address); 
  void spectator_send(spectator_t* spectator, char* message); 
  void spectator_delete(spectator_t* spectator); 

```

### Detailed pseudo code

#### spectator_new

Allocates memory for a spectator struct and initialized values. Returns a pointer to the newly minted spectator

#### spectator_send

Function to send message to spectator

#### spectator_delete

Frees memory allocated to spectator and deletes it



---

## Grid module


### Data structures

> struct grid_t, where the struct contains
* gridArray - a 2D Array of chars representing each point in the map
* goldCounter - an int counting the amount of gold collected
* numberPiles - an int representing the amount of gold piles within the map
* goldPiles - an array of ints representing the amount of gold in each pile

### Definition of function prototypes


A function that reads the map file into a string and generate the locations of the gold.
```c
void grid_loadMap(char* mapFile);
```

A function that distributes the gold.
```c
void grid_goldDistribute(grid_t* grid);
```

A function that determines which parts of the map are visible
```c 
void grid_calculateVisibility(player_t* player, bool hasSeen[][]);
```

A function returning the updated map when a player requests a move
```c
grid_t* grid_movePlayer(player_t* player, char keyPressed); 
```



### Detailed pseudo code
#### `void game_init`:
```
add players
add spectator
initialize grid by calling grid_init
set game-> map to grid_init(map)
```

#### `addSpectator`:

```
if currently a spectator in game
    send quit message
Add new spectator
```



grid_loadMap:
```
read map file line by line
  add each character and space to string until newline reached (keeping track of which index)
  if character is a dot (.)
    insert value of index into hashtable (initially pointing to 0)
    insert value index into a temporary list of dot locations
pick a random number between 10 and 30 -> i
  choose locations of gold
  loops through locations of gold and place random number of nuggets
hashtable index -> amount of gold there
```

grid_goldDistribute:
```
build an array that represents the size of each gold pile
for each number in that array
    pull a random point represented by a dot
    replace the dot with the number of gold
```

grid_calculateVisibility:
```
for each row
    for each point in that row
        if the point has not been seen
            calculate a line to that point from origin
            evaluate all points within that line by checking upper and lower neighbors
                if both are room points
                    game point is visible
                else
                    continue-> move to next point
            
            
```
---

## Game module

### Data structures

struct game_t, a struct with fields containing:
- array of players
- spectator
- master grid
- number of players
- gold counter
- array of gold piles

### Definition of function prototypes

intialize game and allocate memory
```c
void game_init(FILE* map)
```

handle spectators joining
```c
void game_addSpectator(spectator_t x)
```

handle players joining
```c
void game_addPlayer(player_t x);
```

increment the gold counter
```c
void game_incrementGoldCounter(game_t* game, int numGoldCollected);
```
delete the game and free memory
```

delete each player in the game
```c
void game_deletePlayers(game_t* game);
```

```c
void game_delete(game_t* game);
```

### Detailed pseudo code
#### `void game_init`:
```
add players
add spectator
initialize grid by calling grid_init
set game-> map to grid_init(map)
```

#### `addSpectator`:

```
if currently a spectator in game
    send quit message
Add new spectator
```


#### `addPlayer`:
```
if number of players less then MAX
    add player 
```

#### `incrementGoldCounter`
```
add the parameter to the gold counter
```

#### `game_deletePlayers`
```
for each player in the game
  delete the player
free the players array
```
#### `game_delete`
```
delete each player
delete the grid
delete the spectator
free the game
```
---

## Testing plan

### unit testing

create a `gridtest.c` that will print out a grid struct and test the grid methods


create a `servertest.c` that can test adding predefined players and spectators as well as handling sample messages, both valid and invalid. This testing program will randomly add gold to players, testing to see if the correct messages are displayed and if the game ends correctly

test out player and spectator through using the given server in shared directory


create a `servertest.c` that can test adding predefined players and spectators as well as handling sample messages, both valid and invalid. This testing program will randomly add gold to players, testing to see if the correct messages are displayed and if the game ends correctly

### system testing

run the game and test functionality
create a `testing.sh` to evaluate command-line arguments and make sure to catch errors on poor inputs


---

## Limitations

> Bulleted list of any limitations of your implementation.

[to add on later]

---

## Error Handling and Recovery

Players exiting due to an error or lost connection will not effect the flow of the game
Any input or run-time errors will result in a detailed error message printed to stderr with a non-zero exit
Running out of memory will trigger an error.
If a fatal error is encountered, we will free all memory and exit non-zero
