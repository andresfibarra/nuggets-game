/*
 * File: game.c
 * Name: Team Placeholder
 * Date: March 1, 2022
 * Assignment: CS 50 Winter 2022, nuggets project
 * 
 * see game.h for more information
 */


#include <stdio.h>
#include <stdlib.h>
#include "../player/player.h"
#include "../spectator/spectator.h"
#include "../grid/grid.h"
#include "game.h"
#include <assert.h>

/* CONSTANTS */
static const int MaxPlayers = 26;      // maximum number of players

/* GAME STRUCT */
typedef struct game {
  player_t** players;  // array of players
  spectator_t* spectator;  // spectator
  grid_t* masterGrid;  // masterGrid
  int numPlayers;  // number of players ever played
  int goldCounter;  // number of gold found so far
  int* goldPiles;  // array of gold amounts
} game_t;


/* GETTER FUNCTIONS */

/**************** game_getMasterGrid() ****************/
/* see game.h for description */
grid_t* game_getMasterGrid(game_t* game)
{
  if (game->masterGrid != NULL) {
    return game->masterGrid; 
  } else {
    return NULL; 
  }
}

/**************** game_getGoldCounter() ****************/
/* see game.h for description */
int game_getGoldCounter(game_t* game)
{
  if (game->goldCounter != 0) {
    return game->goldCounter; 
  } 
  else {
    return 0; 
  }
}

/**************** game_getPlayers() ****************/
/* see game.h for description */
player_t** game_getPlayers(game_t* game)
{
  if (game->players != NULL) {
    return game->players; 
  } else {
    return NULL; 
  }
}

/**************** game_getPlayerByIndex() ****************/
/* see game.h for description */
player_t* game_getPlayerByIndex(game_t* game, int index)
{
  // player_t* player = game->players[index];
  if (index >= game->numPlayers) {
    return NULL; 
  } else if (game->players[index] != NULL) {
    return game->players[index]; 
  } else {
    return NULL; 
  }
}

/**************** game_getNumPlayers() ****************/
/* see game.h for description */
int game_getNumPlayers(game_t* game) 
{
  if (game->numPlayers != 0) {
    return game->numPlayers; 
  } else {
    return 0; 
  }
}

/**************** game_getSpectator() ****************/
/* see game.h for description */
spectator_t* game_getSpectator(game_t* game)
{
  if (game->spectator != NULL) {
    return game->spectator; 
  } else {
    return NULL; 
  }
}

/**************** game_getGoldPiles() ****************/
/* see game.h for description */
int* game_getGoldPiles(game_t* game)
{
  if (game->goldPiles != NULL) {
    return game->goldPiles; 
  } else {
    return NULL; 
  }
}

/**************** game_init() ****************/
/* see game.h for description */
game_t* game_init(char* mapName)
{
  game_t* game = malloc(sizeof(*game));
  assert(game != NULL); 

  if (game == NULL) {
    flog_v(stderr, "Error allocating memory for game.\n");
    return NULL;
  }
  
  game->players = calloc(MaxPlayers, sizeof(player_t*));
  game->numPlayers = 0;

  grid_t* grid = grid_init();
  grid_loadMap(grid, mapName);
  game->masterGrid = grid;

  game->spectator = NULL;
  game->goldCounter = 0;
  game->goldPiles = grid_createGoldPiles();

  grid_distributeGold(game->masterGrid, game->goldPiles);

  return game;
}

/**************** game_addPlayer() ****************/
/* see game.h for description */
bool game_addPlayer(game_t* game, player_t *player)
{
  if (game != NULL && player != NULL) {
    // if space left for more players
    if (game->numPlayers < MaxPlayers) {
      game->players[game->numPlayers++] = player;  // increment number of players
      return true;
    }
    else {
      return false;
    }
  }
  return false;
}

/**************** game_addSpectator() ****************/
/* see game.h for description */
void game_addSpectator(game_t* game, spectator_t* spec)
{
  if (game != NULL && spec != NULL){
      if (game->spectator != NULL){
        spectator_delete(game->spectator);
      }

    // save new spectator
    game->spectator = spec;
  }
}

/**************** game_incrementGoldCounter() ****************/
/* see game.h for description */
void game_incrementGoldCounter(game_t* game, int numGoldCollected)
{
  game->goldCounter += numGoldCollected;
}

/**************** game_deletePlayers() ****************/
/* see game.h for description */
void game_deletePlayers(game_t* game)
{
  for (int i = 0; i < game->numPlayers; i++) {
    player_delete(game->players[i]);  // delete player at current index
  }

  free(game->players);
}

/**************** game_delete() ****************/
/* see game.h for description */
void game_delete(game_t* game)
{
  game_deletePlayers(game);
  grid_delete(game->masterGrid);
  spectator_delete(game->spectator);
  free(game->goldPiles); 
  free(game);
  game = NULL; 
}