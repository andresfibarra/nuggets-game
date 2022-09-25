/*
 * File: server.c
 * Name: Team Placeholder
 * Date: March 1, 2022
 * Assignment: CS 50 Winter 2022, nuggets project 
 */

#include "../common/file.h"
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#include <assert.h>
#include <time.h>
#include <math.h>
#include "../support/message.h"
#include "../support/log.h"
#include "../game/game.h"
#include "../grid/grid.h"
#include "../player/player.h"
#include "../spectator/spectator.h"
#include "../common/file.h"

/* CONSTANTS */
static const int MaxNameLength = 50;   // max number of chars in playerName
static const int GoldTotal = 250;      // amount of gold in the game
static const int GoldMaxNumPiles = 30; // maximum number of gold piles

/* function prototypes */
void parseArgs(const int argc, char* argv[], int* seed);
game_t* initializeGame(char* mapFile);
bool handleMessage(void* arg, const addr_t from, const char* message);
void handlePlay(const char* name, const addr_t playerAddress, game_t* game);
void handleSpectate(const addr_t spectatorAddress, game_t* game);
char* generateLocation(grid_t* grid); 
bool handleKey(const char key, const addr_t playerAddress, game_t* game);
void handleQ(const addr_t playerAddress, game_t* game);
player_t* findPlayer(const addr_t playerAddress, game_t* game);
spectator_t* findSpectator(const addr_t address, game_t* game);
int movePlayer(player_t* player, int dx, int dy, game_t* game);
bool validateMove(int x, int y, int dx, int dy, game_t* game);
bool isWall(grid_t* masterGrid, int updatedX, int updatedY, int dx, int dy, game_t* game);
void updatePlayerInfo(player_t* player, int dx, int dy, game_t* game);
int getNumGold(grid_t* masterGrid, int updatedX, int updatedY, game_t* game);
bool handleGold(player_t* player, int numGoldCollected, game_t* game);
bool updateGoldCounter(int numGoldCollected, game_t* game);
void handleDisplay(game_t* game, player_t* player, int old_x, int old_y, int new_x, int new_y);
void broadcastDisplay(game_t* game);
void gameOver(game_t* game);
void sendOk(const addr_t playerAddress, char playerID);
void sendGrid(const addr_t playerAddress, grid_t* grid);
void sendGold(const addr_t playerAddress, const int n, const int p, const int r);
void sendDisplay(addr_t address, grid_t* grid); 
void sendQuit(const addr_t playerAddress, char* explanation);
void sendError(const addr_t playerAddress, char* explanation);

int main(const int argc, char* argv[])
{
  int seed = -1;                 // initialize seed
  parseArgs(argc, argv, &seed);  // verify arguments

  // if seed is provided
  if (seed > 0) {
    srand(seed);
  }
  else {
    srand(time(0)); 
  }

  // announce port number
  int portNumber = (int) message_init(stdin);
  if (portNumber == 0) {
    flog_v(stderr, "Could not initialize message module.\n");
    exit (1);
  }
  fprintf(stdout, "Port number: %d\n", portNumber);

  // initialize game
  char* mapName = argv[1];
  game_t* game = initializeGame(mapName);
  if (game == NULL) {
    flog_v(stderr, "NULL game.\n");
    exit (5);
  }

  message_loop(game, 0, NULL, NULL, handleMessage);

  //gameOver(game);

  flog_done(stderr);
  message_done();

}

/**************** parseArgs ****************/
/*
 * Parse and validate arguments
 * 
 * Caller provides:
 *  argc and argv, passed from main
 * 
 * Returns:
 *  Nothing, but exits non-zero if invalid arguments
 * 
 */
void parseArgs(const int argc, char* argv[], int* seed)
{
  if (argc > 3 || argc < 2)
  {
    flog_v(stderr, "invalid number of arguments");
    exit(2);
  }

  if (argc == 3) {   
    //checking for negative 
    if (argv[3][0] == '-'){
      flog_v(stderr, "seed cannot be negative");
      exit(1);
    }
    for (int i = 0; i< strlen(argv[2]); i++){
      //if (number[i] > '9' || number[i] < '0')
      if (!isdigit(argv[2][i])){
        flog_v(stderr, "seed must be an integer");
        exit(3);
      }
    }
    *seed = atoi(argv[2]);
  }

  FILE* fp = fopen(argv[1], "r");
  if (fp == NULL)
  {   
    flog_s(stderr,"%s unable to be read", argv[1]);
    exit(4);
  }
  fclose(fp);

}

/**************** initializeGame ****************/
/*
 * Initialize game
 * 
 * Caller provides:
 *  valid pointer to mapfile.
 * 
 * Returns:
 *  pointer to a game struct
 * 
 * Note:
 *  Allocates memory for game struct. 
 *  Must be freed by calling game_delete.
 */
game_t* initializeGame(char* mapFile)
{
  game_t* game = game_init(mapFile);
  return game;
}

/**************** handleMessage ****************/
/*
 * Handle messages from client
 * 
 * Caller provides:
 *  game arg, valid address, valid message
 * 
 * Returns:
 *  true if error or no more gold
 *  false if gold left
 * 
 * Note:
 *  assume that message is parsed
 *  calls appropriate handle functions
 */
bool handleMessage(void* arg, const addr_t from, const char* message)
{
  // if arguments are NULL, return true and terminate loop
  if (message == NULL || !message_isAddr(from) || arg == NULL) {
    flog_v(stderr, "Unable to handle message.\n");
    return true;
  }

  game_t* game = (game_t*) arg;

  if (strncmp(message, "PLAY ", strlen("PLAY ")) == 0) {
    const char* name = message + strlen("PLAY ");
    handlePlay(name, from, game);
  }

  else if (strncmp(message, "KEY ", strlen("KEY ")) == 0) {
    const char key = message[strlen("KEY ")];
    if (handleKey(key, from, game)) {
      return true; 
    } else {
      return false; 
    }
  }

  else if (strncmp(message, "SPECTATE", strlen("SPECTATE")) == 0) {
    handleSpectate(from, game);
  }

  else if (strncmp(message, "QUIT ", strlen("QUIT ")) == 0) {
    handleQ(from, game);
  }

  else {
    sendError(from, "Not a valid message.\n");
    flog_v(stderr, "Malformed message detected.\n");
  }

  if (game_getGoldCounter(game) == GoldTotal) {
    return true;
  }

  return false;

}

/**************** handlePlay ****************/
/*
 * Handle PLAY message
 * 
 * Caller provides:
 *  name (message from client), address, and pointer to game struct
 * 
 * Returns:
 *  Nothing
 * 
 * Note:
 *  player is added only if possible
 */
void handlePlay(const char* name, const addr_t playerAddress, game_t* game)
{
  if (!message_isAddr(playerAddress)) {
    flog_v(stderr, "Not a valid playerAddress.\n");
  }

  if (name == NULL) {
    sendQuit(playerAddress, "Sorry - you must provide player's name.\n");
  }

  // initialize player struct
  char* nameCpy = malloc(sizeof(char) * (MaxNameLength + 1)); 
  assert(nameCpy != NULL); 
  int i;
  for (i = 0; i < strlen(name) && i < MaxNameLength; i++) {
    if (!isgraph(name[i]) && !isblank(name[i])) {
      nameCpy[i] = '_';
    }
    else {
      nameCpy[i] = name[i];
    }
  }
  nameCpy[i] = '\0';

  char playerID = 65 + game_getNumPlayers(game);                           // 65 is ASCII value of A
  char* location = generateLocation(game_getMasterGrid(game));
  int x;
  int y;
  sscanf(location, "%d %d", &x, &y);
  player_t* player = player_new(playerID, nameCpy, playerAddress, x, y);
  grid_playerGridInit(game_getMasterGrid(game), player->visibleGrid);
  grid_calculateVisibility(player->visibleGrid, game_getMasterGrid(game), player_getID(player), player->posX, player->posY);
  grid_setData(player_getGrid(player), x, y, '@'); 

  if (game_addPlayer(game, player)) {
    sendOk(playerAddress, playerID);                                       // send OK message
  }
  else {
    sendQuit(playerAddress, "Game is full: no more players can join.\n");  // send message to player to quit
  }

  sendGrid(playerAddress, game_getMasterGrid(game));

  sendGold(playerAddress, 0, 0, GoldTotal - game_getGoldCounter(game));

  sendDisplay(playerAddress, player_getGrid(player));
  handleDisplay(game, player, -1, -1, player_getX(player), player_getY(player));

  // clean up
  free(nameCpy); 
  free(location); 
}

/**************** handleSpectate ****************/
/*
 * Handle SPECTATE message
 * 
 * Caller provides:
 *  address, pointer to game struct
 * 
 * Returns:
 *  Nothing
 * 
 */
void handleSpectate(const addr_t spectatorAddress, game_t* game)
{
  if (!message_isAddr(spectatorAddress)) {
    flog_v(stderr, "Not a valid playerAddress.\n");
  }

  spectator_t* newSpectator = spectator_new(spectatorAddress);

  addr_t oldSpectatorAddress;
  if (game_getSpectator(game) != NULL) {
    oldSpectatorAddress = game_getSpectator(game)->address;
    sendQuit(oldSpectatorAddress, "You have been replaced by a new spectator.\n");
    spectator_delete(game_getSpectator(game));
  }
  game_addSpectator(game, newSpectator);

  sendGrid(spectatorAddress, game_getMasterGrid(game));

  int r = GoldTotal - game_getGoldCounter(game);
  sendGold(spectatorAddress, 0, 0, r);    // spectator always receives n=0, p=0

  sendDisplay(spectatorAddress, game_getMasterGrid(game));
}


/**************** generateLocation() ****************/
/*
 * Helper function to generate initial location for player
 *
 * Caller provides:
 *  pointer to grid function
 * 
 * We return: 
 *  a string with the generated location of form "x y"
 * 
 * Note:
 *  returned string must be freed later
 * 
 */
char* generateLocation(grid_t* grid)
{
  int rows = grid_getRows(grid);
  int cols = grid_getCols(grid);

  int x = randInt(0, rows);
  int y = randInt(0, cols);

  while (grid_getData(grid, x, y) != '.') {
    x = randInt(0, rows);
    y = randInt(0, cols);
  }

  char* location = malloc(100);
  assert(location != NULL);  
  sprintf(location, "%d %d", x, y);

  return location;
}

/**************** handleKey ****************/
/*
 * Handle KEY message from player
 * 
 * Caller provides:
 *  character representing key pressed, player address, and pointer to game struct
 * 
 * Returns:
 *   true if movePlayer returns 2 (to break out of message_loop)
 *   false otherwise
 * 
 */
bool handleKey(const char key, const addr_t playerAddress, game_t* game)
{
  if (!message_isAddr(playerAddress)) {  // check if valid address
    flog_v(stderr, "Not a valid playerAddress.\n");
    return false;
  }

  // find player with given playerAddress
  player_t* player = findPlayer(playerAddress, game);
  
  int movePlayerReturn = 0; 

  switch (key) {
    // if Q is pressed, quit game
    case 'Q':
      handleQ(playerAddress, game);
      break;
    
    // up
    case 'k':
      movePlayerReturn = movePlayer(player, -1, 0, game);
      break;

    // down
    case 'j':
      movePlayerReturn = movePlayer(player, 1, 0, game);
      break;

    // right
    case 'l':
      movePlayerReturn = movePlayer(player, 0, 1, game);
      break;
    
    // left
    case 'h':
      movePlayerReturn = movePlayer(player, 0, -1, game);
      break;

    // diagonally left up
    case 'y':
      movePlayerReturn = movePlayer(player, -1, -1, game);
      break;

    // diagnoally right up
    case 'u':
      movePlayerReturn = movePlayer(player, -1, 1, game);
      break;

    // diagonally left down
    case 'b':
      movePlayerReturn = movePlayer(player, 1, -1, game);
      break;
    
    // diagonally right down
    case 'n':
      movePlayerReturn = movePlayer(player, 1, 1, game);
      break;

    // SPRINTS
    // up
    case 'K':
      while ((movePlayerReturn = movePlayer(player, -1, 0, game)) == 1);
      break;

    // down
    case 'J':
      while ((movePlayerReturn = movePlayer(player, 1, 0, game)) == 1);
      break;

    // right
    case 'L':
      while ((movePlayerReturn = movePlayer(player, 0, 1, game)) == 1);
      break;
    
    // left
    case 'H':
      while ((movePlayerReturn = movePlayer(player, 0, -1, game)) == 1);
      break;

    // diagonally left up
    case 'Y':
      while ((movePlayerReturn = movePlayer(player, -1, -1, game)) == 1);
      break;

    // diagnoally right up
    case 'U':
      while ((movePlayerReturn = movePlayer(player, -1, 1, game)) == 1);
      break;

    // diagonally left down
    case 'B':
      while ((movePlayerReturn = movePlayer(player, 1, -1, game)) == 1);
      break;
    
    // diagonally right down
    case 'N':
      while ((movePlayerReturn = movePlayer(player, 1, 1, game)) == 1);
      break;
    
    
    // send error message to player
    default:
      sendError(playerAddress, "invalid key");
      break;
  }

  if (movePlayerReturn == 2) {
    return true; 
  } else {
    return false; 
  }
}

/**************** handleQ ****************/
/*
 * Handle when key Q is pressed
 * 
 * Caller provides:
 *  player address and pointer to game struct
 * 
 * Returns:
 *  Nothing
 * 
 * Note:
 *  Sends quit message to player or spectator with messages formatted accordingly
 */
void handleQ(const addr_t playerAddress, game_t* game)
{
  player_t* player = findPlayer(playerAddress, game);

  if (player != NULL) {  // if pressed by a player
    sendQuit(playerAddress, "Thanks for playing!\n");
  }
  else {
    // check to see if Q was pressed by spectator
    spectator_t* spectator = findSpectator(playerAddress, game);

    if (spectator == NULL) {
      flog_v(stderr, "NULL player, cannot be quitted.\n");
      return;
    }
    else {
      sendQuit(playerAddress, "Thanks for watching!\n");
    }
  }
}

/**************** findPlayer ****************/
/*
 * Helper function to find player struct with given player address
 * 
 * Caller provides:
 *  player address and pointer to game struct
 * 
 * Returns:
 *  pointer to a player struct if found
 *  NULL if not found
 * 
 */
player_t* findPlayer(const addr_t playerAddress, game_t* game)
{
  player_t* currPlayer; 

  for (int i = 0; i < game_getNumPlayers(game); i++) {
    currPlayer = game_getPlayerByIndex(game, i);
    if (message_eqAddr(currPlayer->address, playerAddress)) {
      return currPlayer;
    }
  }
  return NULL;
}

/**************** findSpectator ****************/
/*
 * Helper function to find spectator struct with given address
 * 
 * Caller provides:
 *  address and pointer to game struct
 * 
 * Returns:
 *  pointer to spectator struct if found
 *  NULL if not found
 * 
 */
spectator_t* findSpectator(const addr_t address, game_t* game)
{
  if (game_getSpectator(game) == NULL) {
    return NULL;
  }
  if (message_eqAddr(game_getSpectator(game)->address, address)) {
    return game_getSpectator(game);
  }
  return NULL;
}

/**************** movePlayer ****************/
/*
 * Move player in given direction
 * 
 * Caller provides:
 *  pointer to player struct, dx and dy, and pointer to game struct
 * 
 * Returns:
 *   1 (true) if player is moved
 *   2 if gameOverCalled (handleGold returns true)
 *   0 (false) otherwise
 * 
 * Note:
 *  
 */
int movePlayer(player_t* player, int dx, int dy, game_t* game)
{
  if (player == NULL) {
    return 0;
  }

  if ((dx == 0) & (dy == 0)) {
    return 0;
  }

  if (validateMove(player->posX, player->posY, dx, dy, game)) {
    
    int numGold = getNumGold(game_getMasterGrid(game), player_getX(player)+dx, player_getY(player)+dy, game);

    if (numGold != 0) {
      if (handleGold(player, numGold, game)) {
        return 2; 
      }
    }

    updatePlayerInfo(player, dx, dy, game);  // posX and posY gets updated

    handleDisplay(game, player, player->posX-dx, player->posY-dy, player->posX, player->posY);

    return 1;

  }
  else {
    return 0;
  }
}

/**************** validateMove ****************/
/*
 * Helper function to tell whether a move is valid
 * 
 * Caller provides:
 *  x and y positions, dx and dy, and pointer to game struct
 * 
 * Returns:
 *  true if valid move
 *  false otherwise
 * 
 */
bool validateMove(int x, int y, int dx, int dy, game_t* game)
{
  if (!isWall(game_getMasterGrid(game), x+dx, y+dy, dx, dy, game)) {
    return true;
  }
  return false;
}

/**************** isWall ****************/
/*
 * Helper function to tell whether a given location is a wall or not
 * 
 * Caller provides:
 *  pointer to masterGrid, x and y locations, dx and dy, and pointer to game struct
 * 
 * Returns:
 *  true if it is a wall
 *  false otherwise
 * 
 * Note:
 *  If a player already exists in given location, call movePlayer on that player
 */
bool isWall(grid_t* masterGrid, int updatedX, int updatedY, int dx, int dy, game_t* game)
{
  char newLoc = grid_getData(masterGrid, updatedX, updatedY); 
  if (newLoc == '|' || newLoc == '-' || newLoc == '+' || newLoc == ' ') {
    return true;
  }
  else if (newLoc == '.' || newLoc == '*' || newLoc == '#') {
    return false;
  }
  else {  // if player tries to move to a location where another player is at
    // call movePlayer on that player

    // find player struct
    char playerID = newLoc;
    player_t* player;
    for (int i = 0; i < game_getNumPlayers(game); i++) {
      if (game_getPlayerByIndex(game, i)->playerID == playerID) {
        player = game_getPlayerByIndex(game, i);
        break;
      }
    }

    if (player == NULL) {
      flog_v(stderr, "NULL player cannot be moved.\n");
    }

    movePlayer(player, -dx, -dy, game);
    
    return false;
  }
}

/**************** updatePlayerInfo ****************/
/*
 * Updates player struct
 * 
 * Caller provides:
 *  pointer to player struct, dx and dy, and pointer to game struct
 * 
 * Returns:
 *  Nothing
 * 
 */
void updatePlayerInfo(player_t* player, int dx, int dy, game_t* game)
{
  if (player == NULL) {
    flog_v(stderr, "NULL player.\n");
  }

  player_move(player, dx, dy);

  // update visibleGrid of player
  grid_calculateVisibility(player->visibleGrid, game_getMasterGrid(game),player_getID(player), player->posX, player->posY);
}


/**************** getNumGold ****************/
/*
 * Find number of gold found in a given location
 * 
 * Caller provides:
 *  pointer to masterGrid, x and y locations, pointer to game struct
 * 
 * Returns:
 *  number of gold found in given location
 *  0 if no gold found
 * 
 */
int getNumGold(grid_t* masterGrid, int updatedX, int updatedY, game_t* game)
{
  char newLoc = grid_getData(masterGrid, updatedX, updatedY); 
  if (newLoc == '*') {
    int numGold = 0;
    for (int i = 0; i < GoldMaxNumPiles; i++) {
      if (game_getGoldPiles(game)[i] != 0) {
        numGold = game_getGoldPiles(game)[i];
        game_getGoldPiles(game)[i] = 0;
        break;
      }
    }
    return numGold;
  }
  else {
    return 0;
  }
}

/**************** handleGold ****************/
/*
 * Handle whenever a player collects gold
 * 
 * Caller provides:
 *  pointer to player struct, number of gold just collected, and pointer to game struct
 * 
 * Returns:
 *   true if gameOver condition is reached (updateGoldCounter returns true)
 *   false otherwise
 * 
 * Note:
 *  Sends GOLD message to all players
 */
bool handleGold(player_t* player, int numGoldCollected, game_t* game)
{
  player_updateGold(player, numGoldCollected);
  if (updateGoldCounter(numGoldCollected, game)) {
    return true; 
  }

  player_t* currPlayer; 

  // send GOLD message to all players
  for (int i = 0; i < game_getNumPlayers(game); i++) {
    currPlayer = game_getPlayerByIndex(game, i);
    int p = currPlayer->gold;
    int r = GoldTotal - game_getGoldCounter(game);
    sendGold(player->address, numGoldCollected, p, r);
  }
  // send GOLD message to spectator
  if (game_getSpectator(game) != NULL) {
    sendGold(game_getSpectator(game)->address, numGoldCollected, 0, GoldTotal - game_getGoldCounter(game));
  }
  return false; 
}


/**************** updateGoldCounter ****************/
/*
 * Helper function to update game struct information on gold collected
 * 
 * Caller provides:
 *  number of gold just collected and pointer to game struct
 * 
 * Returns:
 *   true if gameOver() called
 *   false otherwise
 * 
 */
bool updateGoldCounter(int numGoldCollected, game_t* game)
{
  game_incrementGoldCounter(game, numGoldCollected);
  if (game_getGoldCounter(game) >= GoldTotal) {
    gameOver(game);
    return true; 
  }
  return false; 
}

/**************** handleDisplay ****************/
/*
 * Helper function to change display of players whenever a player moves
 * 
 * Caller provides:
 *  pointer to game struct, pointer to player struct, old locations, and new locations
 * 
 * Returns:
 *  Nothing
 * 
 */
void handleDisplay(game_t* game, player_t* player, int old_x, int old_y, int new_x, int new_y)
{
  for (int i = 0; i < game_getNumPlayers(game); i++) {
    player_t* player = game_getPlayerByIndex(game, i);

    char data = ' ';
    if (old_x < 0 && old_y < 0) {
      data = player_getID(player);
    }

    grid_updateMaster(game_getMasterGrid(game), player_getID(player), old_x, old_y, new_x, new_y, data);
    grid_refresh(player->visibleGrid, player->playerID, old_x, old_y, new_x, new_y, data);
  }
  broadcastDisplay(game);
}

/**************** broadcastDisplay ****************/
/*
 * Helper function to broadcast DISPLAY message to players
 * 
 * Caller provides:
 *  pointer to game struct
 * 
 * Returns:
 *  Nothing
 * 
 */
void broadcastDisplay(game_t* game)
{
  for (int i = 0; i < game_getNumPlayers(game); i++) {
    player_t* player = game_getPlayerByIndex(game, i);
    // sendDisplay(player->address, player-> visibleGrid);
    sendDisplay(player_getAddress(player), player_getGrid(player));
  }
  if (game_getSpectator(game) != NULL) {
    sendDisplay(game_getSpectator(game)->address, game_getMasterGrid(game));
  }
}

/**************** gameOver ****************/
/*
 * End the game when all gold has been collected
 * 
 * Caller provides:
 *  pointer to game struct
 * 
 * Returns:
 *  Nothing
 * 
 * Note:
 *  all allocated memory is freed
 *  sends summary to all clients
 */
void gameOver(game_t* game)
{
  char summary[message_MaxBytes];
  sprintf(summary, "QUIT GAME OVER:\n");
  for (int i = 0; i < game_getNumPlayers(game); i++) {
    player_t* player = game_getPlayerByIndex(game, i);

    char playerReport[message_MaxBytes]; 
    sprintf(playerReport, "%c\t%d %s\n", player_getID(player), player_getGold(player), player_getName(player));
    strcat(summary, playerReport); 
  }
  for (int i = 0; i < game_getNumPlayers(game); i++) {
    player_t* player = game_getPlayerByIndex(game, i);
    message_send(player->address, summary);
  }
  if (game_getSpectator(game) != NULL) {
    message_send(game_getSpectator(game)->address, summary);
  }
  game_delete(game);

  fprintf(stderr, "%s\n", summary); 
}



/* SEND FUNCTIONS */

/**************** sendOk ****************/
/*
 * Construct and send OK message to player
 * 
 * Caller provides:
 *  player address and playerID
 * 
 * Returns:
 *  Nothing
 * 
 * Notes:
 *  assumed that player can join
 */
void sendOk(const addr_t playerAddress, char playerID)
{
  if (message_isAddr(playerAddress)) {
    char message[message_MaxBytes];
    sprintf(message, "OK %c\n", playerID);
    message_send(playerAddress, message);
  }
}

/**************** sendGrid ****************/
/*
 * Construct and send GRID message to client
 * 
 * Caller provides:
 *  address and pointer to grid
 * 
 * Returns:
 *  Nothing
 * 
 */
void sendGrid(const addr_t playerAddress, grid_t* grid) 
{
  if (message_isAddr(playerAddress)) {
    char message[message_MaxBytes];
    sprintf(message, "GRID %d %d\n", grid_getRows(grid), grid_getCols(grid));
    message_send(playerAddress, message);
  }
}

/**************** sendGold ****************/
/*
 * Construct and send GOLD message to client
 * 
 * Caller provides:
 *  address, n (amount just collected), p (amount that player has), r (amount left to be found)
 * 
 * Returns:
 *  Nothing
 * 
 */
void sendGold(const addr_t playerAddress, const int n, const int p, const int r)
{
  if (message_isAddr(playerAddress)) {
    char message[message_MaxBytes];
    sprintf(message, "GOLD %d %d %d\n", n, p, r);
    message_send(playerAddress, message);
  }
}

/**************** sendDisplay ****************/
/*
 * Construct and send DISPLAY message to client
 * 
 * Caller provides:
 *  address and pointer to grid
 * 
 * Returns:
 *  Nothing
 * 
 */
void sendDisplay(addr_t address, grid_t* grid)
{
  char message[message_MaxBytes];
  char* gridString = grid_toString(grid); 
  sprintf(message, "DISPLAY\n%s", gridString);
  message_send(address, message);
  free(gridString); 
}

/**************** sendQuit ****************/
/*
 * Construct and send QUIT message to client
 * 
 * Caller provides:
 *  address and explanation
 * 
 * Returns:
 *  Nothing
 * 
 */
void sendQuit(const addr_t playerAddress, char* explanation)
{
  if (message_isAddr(playerAddress)) {
    char message[message_MaxBytes];
    sprintf(message, "QUIT %s", explanation);
    message_send(playerAddress, message);
  }
}

/**************** sendError ****************/
/*
 * Construct and send ERROR message to client
 * 
 * Caller provides:
 *  address and explanation
 * 
 * Returns:
 *  Nothing
 * 
 */
void sendError(const addr_t playerAddress, char* explanation)
{
  if (message_isAddr(playerAddress)) {
    char message[message_MaxBytes];
    sprintf(message, "ERROR %s", explanation);
    message_send(playerAddress, message);
  }
}
