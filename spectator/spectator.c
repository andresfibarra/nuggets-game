/*
 * File: spectator.c
 * Name: Team Placeholder
 * Date: March 1, 2022
 * Assignment: CS 50 Winter 2022, nuggets project
 * 
 * see spectator.h for more information
 */


#include <stdio.h>
#include <stdlib.h>
#include "../support/message.h"
#include "../support/log.h"
#include "spectator.h"




addr_t spectator_getAddress(spectator_t* spectator)
{
  return spectator->address;
}


/**************** spectator_new() ****************/
/* see spectator.h for description */
spectator_t* spectator_new(addr_t address)
{
  spectator_t* spectator = malloc(sizeof(spectator_t));

  if (spectator == NULL) {
    free(spectator);
    return NULL;
  }
  else {
    spectator->address = address;
  }

  return spectator;
}

/**************** spectator_send() ****************/
/* see spectator.h for description */
void spectator_send(spectator_t* spectator, char* message)
{
  if (spectator == NULL) {
    flog_v(stderr, "Cannot send message to NULL spectator.\n");
  }
  else if (message == NULL) {
    flog_v(stderr, "Cannot send NULL message.\n");
  }
  else {
    message_send(spectator->address, message);
  }
}

/**************** spectator_delete() ****************/
/* see spectator.h for description */
void spectator_delete(spectator_t* spectator)
{
  if (spectator != NULL) {
    free(spectator);
  }
}
