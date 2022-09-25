/*
 * game.h - header file for CS50 game module
 * 
 * 
 * Leina Sato, Rebecca Liu, Andres Ibarra, Elliot Krewson
 * March 2022
 */

#include <stdio.h>
#include <stdlib.h>
#include "../player/player.h"
#include "../spectator/spectator.h"
#include "../grid/grid.h"

//global type -- opaque to user
typedef struct game game_t;

/**************** game_getMasterGrid() ****************/
/*
 * Getter for the masterGrid stored in the game struct
 * Caller provides:
 *  valid game struct 
 * We return:
 *   masterGrid if that field is initialized, NULL otherwise
 * We assume: 
 *   Game struct provided is valid and initialized
 */
grid_t* game_getMasterGrid(game_t* game); 

/**************** game_getGoldCounter() ****************/
/*
 * Getter for the goldCounter stored in the game struct
 * Caller provides:
 *  valid game struct 
 * We return:
 *   and int representing goldCounter if that field is initialized
 *   NULL otherwise
 * We assume: 
 *   Game struct provided is valid and initialized
 */
int game_getGoldCounter(game_t* game); 

/**************** game_getPlayers() ****************/
/*
 * Getter for the array of player_t's stored in the game struct
 * Caller provides:
 *  valid game struct 
 * We return:
 *   the array of player_t's in game if it is initialized
 *   NULL otherwise
 * We assume: 
 *   Game struct provided is valid and initialized
 */
player_t** game_getPlayers(game_t* game); 

/**************** game_getPlayerByIndex ****************/
/*
 * Getter for a specific player_t given its index in the array of player_t's
 *   as stored in the game struct
 * Caller provides:
 *   valid game struct 
 *   int indicating the index of the player
 * We return:
 *   NULL if invalid index or if the players array is not initialized
 *   a pointer to the specified player_t otherwise
 * We assume: 
 *   Game struct provided is valid and initialized
 */
player_t* game_getPlayerByIndex(game_t* game, int index); 

/**************** game_getNumPlayers() ****************/
/*
 * Getter for number of players stored in game
 * Caller provides:
 *  valid game struct 
 * We return:
 *   an int representing the number of players in game's players array
 *   NULL otherwise
 * We assume: 
 *   Game struct provided is valid and initialized
 */
int game_getNumPlayers(game_t* game); 

/**************** game_getSpectator() ****************/
/*
 * Getter for the spectator stored in game
 * Caller provides:
 *  valid game struct 
 * We return:
 *   a pointer to the spectator stored in game if it is initialized
 *   NULL otherwise
 * We assume: 
 *   Game struct provided is valid and initialized
 */
spectator_t* game_getSpectator(game_t* game); 

/**************** game_getGoldPiles() ****************/
/*
 * Getter for gold piles int array in game
 * Caller provides:
 *  valid game struct 
 * We return:
 *   an array of ints representing the gold piles, as stored in game
 *   NULL if that array is ont initialized
 * We assume: 
 *   Game struct provided is valid and initialized
 */
int* game_getGoldPiles(game_t* game); 

/**************** game_incrementGoldCounter() ****************/
/*
 * Setter for the goldCounter int in game
 * Caller provides:
 *   valid game struct 
 *   an int for by how much the goldCounter should be incremented
 * We do:
 *   Increment the goldCounter field in the game struct by numGoldCollected
 * We assume: 
 *   Game struct provided is valid and initialized
 *   numGoldCollected is a valid int (does not cause maxGold to be exceeded)
 */
void game_incrementGoldCounter(game_t* game, int numGoldCollected); 

/**************** game_init ****************/
/*
 * Initializes game struct
 * Caller provides:
 *  valid pointer to mapfile.
 * We return:
 *  pointer to a game struct
 * Caller is responsible for:
 *   Later freeing the returned game_t by calling game_delete
 * Note:
 *  Allocates memory for game struct. 
 *  Must be freed by calling game_delete.
 */
game_t* game_init(char* mapName);

/**************** game_addPlayer ****************/
/*
 * Add given player to array of players
 * Caller provides:
 *  valid pointer to game, and a player struct.
 * We return:
 *   true if player able to be added (MaxPlayers has not been reached)
 *   false otherwise
 * Note:
 *  The game can only have a maximum of 26 players. 
 */
bool game_addPlayer(game_t* game, player_t *player);

/**************** game_addSpectator ****************/
/*
 * Add given spectator to the game
 * Caller provides:
 *   valid pointer to game, and a spectator struct.
 * Caller is responsible for:
 *   Sending quit message to the old spectator if there is one
 * Note:
 *  The game can only have one spectator. 
 */
void game_addSpectator(game_t* game, spectator_t* spec);


/**************** game_deletePlayers ****************/
/*
 * Deletes and frees all memory in the array of players stored in grid
 * Caller provides:
 *  valid pointer to a game.
 * We assume: 
 *   the players array in game is initialized
 */
void game_deletePlayers(game_t* game);

/**************** game_delete ****************/
/*
 * delete the game and free all memory associated with it
 * Caller provides:
 *  valid pointer to a game. 
 */
void game_delete(game_t* game);
