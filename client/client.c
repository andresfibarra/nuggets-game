/*
 * Usage: ./client hostName port [playerName]
 * 
 * File: player.c
 * Name: Team Placeholder
 * Date: March 1, 2022
 * Assignment: CS 50 Winter 2022, nuggets project
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>
#include <string.h>
#include <ncurses.h>
#include "message.h"
#include "log.h"

/******* global types *******/
typedef struct client {
	char playerID; 
	char* realName; 
	bool isPlayer; 
	addr_t serverAddress; 
} client_t; 

/******* global variables types *******/
const int STATUS_LENGTH = 50; 

//Function prototype declarations. Functions are defined below
static int parseArgs(const int argc, char* argv[], client_t** client);
void initClient(client_t* client);
void initNetwork(); 
bool handleInput(void* arg);
bool handleMessage(void* arg, const addr_t from, const char* message);
void handleGrid(const char* message);
void handleGold(const char* message, client_t* client); 
void handleDisplay(const char* message);
void handleQuit(const char* message, client_t* client);
void handleError(const char* message);
void client_delete(client_t* client); 

/**************** main() ****************/
int main(const int argc, char* argv[])
{
	client_t* client = malloc(sizeof(client_t)); 
	assert(client != NULL); 

	int parseArgsReturn = parseArgs(argc, argv, &client); 
	if (parseArgsReturn != 0) {
		return parseArgsReturn; 
	}

	initClient(client); 
	message_loop(client, 0, NULL, handleInput, handleMessage); 
	message_done(); 

	client_delete(client); 

	return 0; 
}

/**************** parseArgs() ****************/
/* 
 * Validate and unpack command-line args
 */
static int parseArgs(const int argc, char* argv[], client_t** client)
{
	if (argc != 3 && argc != 4) {
		fprintf(stderr, "%s: usage: ./client hostName port [playerName]\n", argv[0]); 
		return(1); 
	}

	if (client == NULL) {
		fprintf(stderr, "%s: Error: client is null\n", argv[0]); 
		return(2); 
	}

	int clientPort = message_init(NULL); 
	if (clientPort == 0) {
		fprintf(stderr, "%s: Error: unable to get port number for client\n", argv[0]); 
		return(4); 
	}
	//message_init(stderr); 

	if (!message_setAddr(argv[1], argv[2], &((*client)->serverAddress))) {  
		fprintf(stderr, "%s: Error: failed to set server address\n", argv[0]);
		return(3);
	}

	if (argc == 3) {
		(*client)->isPlayer = false; 
		(*client)->realName = NULL; 
	} else if (argc == 4) {
		(*client)->isPlayer = true; 
		(*client)->realName = argv[3]; 
	}
 
	return 0; 
}

/**************** initClient() ****************/
/* 
 * Sets up nCurses and calls function to initialize network 
 * We assume: 
 *   client has been initialized
 */
void initClient(client_t* client)
{
	initscr(); 
	cbreak(); 
	noecho(); 
	start_color(); 
	init_pair(1, COLOR_YELLOW, COLOR_BLACK);
	attron(COLOR_PAIR(1)); 

	initNetwork(client); 

	refresh(); //draw screen to start
}

/**************** initClient() ****************/
/* 
 * Initializes network by sending first message to server
 * We do:
 *   if client is a player, send "PLAY [realname]" to server
 *   if client is a spectator, send "SPECTATE" to server
 * We assume: 
 *   message is non-null
 *   client has been initialized
 */
void initNetwork(client_t* client)
{
	char* message; 

	if (client->isPlayer) {
		message = malloc(strlen("PLAY ") + strlen(client->realName) + 1); 
		assert(message != NULL); 
		sprintf(message, "PLAY %s", client->realName); 
		message_send(client->serverAddress, message); 
	} else {
		message = malloc(strlen("SPECTATE") + 1); 
		assert(message != NULL); 
		strcpy(message, "SPECTATE"); 
		message_send(client->serverAddress, message); 
	}
	free(message); 
	message = NULL; 
}

/**************** handleInput() ****************/
/* 
 * Function passed to message_loop to handle reading inputs from user
 * Note: 
 *   arg is always the client struct
 * We return: 
 *   false on all occasions to continue message_loop
 * We assume: 
 *   Client has been initialized
 */
bool handleInput(void* arg)
{
	client_t* client = arg;               							//cast arg to client_t

	int key = getch();
	char keyMessage[strlen("KEY " + 2)];
	
	sprintf(keyMessage, "KEY %c", key); 
	if (client->isPlayer) {
		message_send(client->serverAddress, keyMessage); 	// send it to the server
	}
	refresh(); 

	return false; 
}

/**************** handleMessage() ****************/
/* 
 * Function passed to message_loop to handle reading messages from server
 * We do: 
 *   Call appropriate handling method depending on the message receieved
 *   If malformed message receieved, an error is printed to the status line
 * We return: 
 *   true if QUIT message receieved (to end message_loop)
 *   false otherwise
 * Note:
 *   arg is always the client struct
 */
bool handleMessage(void* arg, const addr_t from, const char* message)
{
	client_t* client = arg; 														//cast arg to client_t

	if (strncmp(message, "OK ", strlen("OK ")) == 0) {  //only happens if player
		sscanf(message, "OK %c", &(client->playerID)); 
	} else if (strncmp(message, "GRID ", strlen("GRID ")) == 0) { 
		handleGrid(message);
	} else if (strncmp(message, "GOLD ", strlen("GOLD ")) == 0) {
		handleGold(message, client);
	} else if (strncmp(message, "DISPLAY", strlen("DISPLAY")) == 0) {
		handleDisplay(message); 
	} else if (strncmp(message, "ERROR ", strlen("ERROR ")) == 0) {
		handleError(message);
	} else if (strncmp(message, "QUIT ", strlen("QUIT ")) == 0) {
		handleQuit(message, client);
		return true;
	} else {
		mvprintw(0, 0, "ERROR: received a malformed message");
		clrtoeol(); 
		refresh();
	}

	return false; 
}

/**************** handleGrid() ****************/
/*
 * Ensures that the user window is big enough (NR+1 x NC+1) according to the
 *   message sent by server
 * Caller provides: 
 *   Grid message
 * We do: 
 *   prompt the user to make the window bigger until the map 
 *     can fit on the screen
 * We assume: 
 *   The message is not malformed
 */
void handleGrid(const char* message)
{
	int uy, ux;               //screen boundaries
	int gridx, gridy; 				//screen boundaries asked for by server
	sscanf(message, "GRID %d %d", &gridy, &gridx); 

	getmaxyx(stdscr, uy, ux); 

	char key; 

	while(uy < (gridy+1) || ux < (gridx+1)) {
		mvprintw(0, 0, "Please resize your window: it must be at least %d high and %d wide. Press enter to continue", gridy+1, gridx+1); 
		
		key = getch(); 
		if (key == '\n') {
			getmaxyx(stdscr, uy, ux); 
		}
	}
	
	move(0, 0); 
	clrtoeol(); 
	refresh(); 
}

/**************** handleGold() ****************/
/*
 * Prints messages about gold in game status. Called by handleMessage
 * Caller provides: 
 *   Message (such as that sent by server)
 *   client struct for this client
 * We do: 
 *   Print message to screen about gold
 * We assume: 
 *   The message is not malformed (is of form GOLD n p r)
 */
void handleGold(const char* message, client_t* client)
{
	int n, p, r; 
	sscanf(message, "GOLD %d %d %d", &n, &p, &r); 

	if (client->isPlayer) {
		mvprintw(0, 0, "Player %c has %d nuggets (%d nuggets unclaimed).", client->playerID, p, r); 
		if (n > 0) {
			mvprintw(0, STATUS_LENGTH, "GOLD received: %d", n); 
		}
	} else {
		mvprintw(0, 0, "Spectator: %d nuggets unclaimed", r); 
	}

	clrtoeol(); 
	refresh(); 
}

/**************** handleDisplay() ****************/
/*
 * Updates string based on message received by server. Called by handleMessage
 * Caller provides: 
 *   Message (such as that sent by server)
 * We do: 
 *   Print the message to the window
 * We assume: 
 *   The message is not malformed (is of form DISPLAY\n[map]])
 */
void handleDisplay(const char* message)
{
	char* messageCpy = malloc(strlen(message)+1); 
	assert(messageCpy != NULL); 
	strcpy(messageCpy, message); 
	char* map = messageCpy + strlen("DISPLAY\n"); 
	char* mapCpy = malloc(strlen(message) - strlen("DISPLAY\n") + 1); 
	assert(mapCpy != NULL); 
	strcpy(mapCpy, map); 

	//set up cursor
	int x, y, currX, currY; 
	getbegyx(stdscr, y, x); 
	y++; 										// start the y on the first non status row
	move(y,x); 
	currX = x; 
	currY = y; 
	 
	//print map
	char c; 
	for (int i = 0; i < strlen(mapCpy); i++) {
		c = mapCpy[i]; 
		if (c == '\n') {
			currY++; 						//move cursor down a row
			currX = x; 					//move cursor back to first column
			move(currY, currX); 
		} else {
			addch(c); 
			currX++; 
			move(currY, currX); 
		}
	}

	//push map to screen
	move(0,0); 
	refresh(); 

	//clean up
	free(messageCpy); 
	free(mapCpy); 
}


/**************** handleQuit() ****************/
/*
 * Quits the game. Called by handleMessage
 * Caller provides: 
 *   Message (such as that sent by server)
 * We do: 
 *   Exit nCurses
 *   prints the explanation
 * We assume: 
 *   The message is not malformed (is of form QUIT [explanation])
 */
void handleQuit(const char* message, client_t* client)
{
	nocbreak(); 
	endwin(); 

	char* messageCpy = malloc(strlen(message)+1); 
	assert(messageCpy != NULL); 
	strcpy(messageCpy, message); 
	char* explanation = messageCpy + strlen("QUIT "); 
	fprintf(stderr, "%s\n", explanation); 

	free(messageCpy); 
}

/**************** handleError() ****************/
/*
 * Prints error messages after status line
 * Caller provides: 
 *   Message (such as that sent by server)
 * We do: 
 *   Print error explation
 * We assume: 
 *   The message is not malformed (is of form ERROR [explanation])
 */
void handleError(const char* message)
{
	char* messageCpy = malloc(strlen(message) + 1); 
	assert(messageCpy != NULL); 
	strcpy(messageCpy, message); 
	char* explanation = messageCpy + strlen("ERROR ");  
	mvprintw(0, STATUS_LENGTH, "%s", explanation); 
	refresh(); 

	free(messageCpy); 
}

/**************** client_delete() ****************/
/* 
 * Deletes client struct. Just a wrapper to delete the struct
 */
void client_delete(client_t* client)
{
	free(client); 
}
