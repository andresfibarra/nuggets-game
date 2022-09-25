/*
 * player.h - header file for CS50 player module
 * 
 * 
 * Leina Sato, Rebecca Liu, Andres Ibarra, Elliot Krewson
 * March 2022
 */

#ifndef __PLAYER_H
#define __PLAYER_H

#include <stdio.h>
#include <stdlib.h>
#include "../support/message.h"
#include "../grid/grid.h"
#include "../support/log.h"


/******* global types *******/
typedef struct player {
  char playerID;
  char* realName;
  addr_t address;
  int posX;
  int posY;
  int gold;
  grid_t* visibleGrid;  // subject to change
  bool isStillPlaying;
} player_t;

char player_getID(player_t* player); 

char* player_getName(player_t* player); 

addr_t player_getAddress(player_t* player); 

int player_getX(player_t* player); 

int player_getY(player_t* player); 

int player_getGold(player_t* player); 

grid_t* player_getGrid(player_t* player); 

bool player_getStatus(player_t* player); 


/**************** player_new ****************/
/*
 * Initializes a new player
 * 
 * Caller provides:
 *  valid playerID, pointer to realName, address, and x and y positions.
 * 
 * Returns:
 *  pointer to a player struct
 * 
 * Note:
 *  Allocates memory for player struct. 
 *  Must be freed by calling player_delete.
 */
player_t* 
player_new(char playerID, char* realName, addr_t address, int posX, int posY);




/**************** player_move ****************/
/*
 * moves the player
 * 
 * Caller provides:
 *  pointer to player struct, delta x and y
 * 
 * Returns:
 *  Nothing
 * 
 * Note:
 *  assumed that the validity of the move has already been checked
 */
void player_move(player_t* player, int dx, int dy);



void player_updateGrid(player_t* player, grid_t* grid); 

void player_updateGold(player_t* player, int amount);

/**************** player_send ****************/
/*
 * send given message to given player
 * 
 * Caller provides:
 *  pointer to a player struct and message to send
 * 
 * Returns: 
 *  nothing
 * 
 * Note: 
 *  nothing is freed
 */
void player_send(player_t* player, char* message);



/**************** player_delete ****************/
/*
 * deletes a player struct
 * 
 * Caller provides:
 *  pointer to player struct
 * 
 * Returns:
 *  nothing
 * 
 * Note:
 *  assumed that struct was created through call to player_new()
 */
void player_delete(player_t* player);


#endif //__PLAYER_H