/*
 * File: player.c
 * Name: Team Placeholder
 * Date: March 1, 2022
 * Assignment: CS 50 Winter 2022, nuggets project
 * 
 * see player.h for more information
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../support/message.h"
#include "player.h"
#include "../grid/grid.h"
#include "../support/log.h"


/**************** getter functions ****************/
char player_getID(player_t* player)
{
  return player->playerID;
}

char* player_getName(player_t* player)
{
  return player->realName;
}

addr_t player_getAddress(player_t* player)
{
  return player->address;
}

/**************** player_new() ****************/
/* see player.h for description */
player_t* 
player_new(char playerID, char* realName, addr_t address, int posX, int posY)
{
  // allocate memory for player struct
  player_t* player = malloc(sizeof(player_t));

  if (player == NULL) {
    free(player);
    return NULL;
  }
  else {
    player->playerID = playerID;
    player->realName = malloc(sizeof(realName));
    strcpy(player->realName, realName);
    player->address = address;
    player->posX = posX;
    player->posY = posY;
    player->gold = 0;
    player->visibleGrid = grid_init(); 
    player->isStillPlaying = true;

    return player;
  }
  return 0; 
}

int player_getX(player_t* player)
{
  return player->posX;
}

int player_getY(player_t* player)
{
  return player->posY;
}

int player_getGold(player_t* player)
{
  return player->gold;
}


grid_t* player_getGrid(player_t* player)
{
  return player->visibleGrid;
}


bool player_getStatus(player_t* player)
{
  return player->isStillPlaying;
}

/**************** local functions ****************/

/**************** player_move() ****************/
/* see player.h for description */
void player_move(player_t* player, int dx, int dy)
{
  if (player == NULL) {
    flog_v(stderr, "Cannot move NULL player.\n");
  }
  else {
    player->posX += dx;
    player->posY += dy;
  }
}


/**************** player_updateGrid() ****************/
/* see player.h for description */
void player_updateGrid(player_t* player, grid_t* grid)
{
  if (player == NULL) {
    flog_v(stderr, "Cannot change grid for NULL player.\n");
  }
  else {
    player->visibleGrid = grid;
  }
}

/**************** player_updateGold() ****************/
/* see player.h for description */
void player_updateGold(player_t* player, int amount)
{
  if (player == NULL) {
    flog_v(stderr, "Cannot add to NULL player.\n");
  }
  else {
    player->gold += amount;
  }
}

/**************** player_send() ****************/
/* see player.h for description */
void player_send(player_t* player, char* message)
{
  if (player == NULL) {
    flog_v(stderr, "Cannot send message to NULL player.\n");
  }
  else if (message == NULL) {
    flog_v(stderr, "Cannot send NULL message.\n");
  }
  else {
    message_send(player->address, message);
  }
}

/**************** player_delete() ****************/
/* see player.h for description */
void player_delete(player_t* player)
{
  if (player != NULL) {
    free(player->realName);
    grid_delete(player->visibleGrid); 
    free(player);
  }
}
