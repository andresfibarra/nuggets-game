/*
 * spectator.h - header file for CS50 spectator module
 * 
 * 
 * Leina Sato, Rebecca Liu, Andres Ibarra, Elliot Krewson
 * March 2022
 */

#ifndef __SPECTATOR_H
#define __SPECTATOR_H

#include <stdio.h>
#include <stdlib.h>
#include "../support/message.h"
#include "../support/log.h"

/******* global types *******/
typedef struct spectator {
  addr_t address;
} spectator_t;

/**************** spectator_new ****************/
/*
 * Initializes a new spectator
 * 
 * Caller provides:
 *  valid address
 * 
 * Returns:
 *  pointer to a spectator
 * 
 * Note:
 *  Allocates memory for spectator struct. 
 *  Must be freed by calling spectator_delete.
 */
spectator_t* spectator_new(addr_t address);


/**************** spectator_send ****************/
/*
 * send given message to given spectator
 * 
 * Caller provides:
 *  pointer to a spectator struct and message to send
 * 
 * Returns: 
 *  nothing
 * 
 * Note: 
 *  nothing is freed
 */
void spectator_send(spectator_t* spectator, char* message);


/**************** spectator_delete ****************/
/*
 * deletes a spectator struct
 * 
 * Caller provides:
 *  pointer to spectator struct
 * 
 * Returns:
 *  nothing
 * 
 * Note:
 *  assumed that struct was created through call to spectator_new()
 */
void spectator_delete(spectator_t* spectator);


#endif //__SPECTATOR_H