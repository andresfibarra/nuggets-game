/* 
 * grid.h - header file for nuggets grid module
 *
 * placeholders, Winter 2022
 */

#include <stdlib.h>
#include <stdbool.h>

/********** global types **********/
// typedef struct grid {
//   char data[200][200];
//   int rows;
//   int cols;
// } grid_t;

typedef struct grid grid_t;

/*************** Accessor Methods **********/
int grid_getRows(grid_t* grid); 
int grid_getCols(grid_t* grid); 
char grid_getData(grid_t* grid, int row, int col);
void grid_setData(grid_t* grid, int row, int col, char newChar);

/********** grid_init **********/
/* Initialize the grid and allocate memory for the grid, but not its contents
 *
 * Caller provides:
 *  Nothing
 * We return:
 *  An empty, initialized grid
 *  NULL if the grid is NULL after allocation
 */

grid_t* grid_init(void);

/********** grid_playerGridInt **********/
/* Assigns data and allocates memory for the grid's contents
 * Only called by player's visibility grids
 *
 * Caller provides:
 *  The master grid
 *  The player grid being initialized
 */
void grid_playerGridInit(grid_t* masterGrid, grid_t* playerGrid); 

/********** grid_loadMap **********/
/* Opens the map file and initializes the contents of the master grid
 *
 * Caller provides:
 *  The grid initialized to master
 *  A path name to a map .txt file
 */
void grid_loadMap(grid_t* grid, char* mapName);

/********** grid_distributeGold **********/
/* Places gold piles in random, valid points on the map
 *
 * Caller provides:
 *  An array of ints representing gold piles
 * We do:
 *  Loop through points until they contain a '.'
 *  Swap the . with an *
 * Assumptions:
 *  We assume the caller provides an array produced by grid_createGoldPiles()
 */
void grid_distributeGold(grid_t* grid, int* piles);

/********** grid_toString **********/
/* Builds a string from the data content in the grid provided
 *
 * Caller provides:
 *  A grid struct
 * We return:
 *  A char* containing all of the data and line breaks
 * Caller is responsible for:
 *  Freeing the string produced by this function
 */
char* grid_toString(grid_t* grid);

/********** grid_print **********/
/* Prints the contents of the grid
 *
 * Notes:
 *  grid_print, toString, and the orig .txt file should all have the same output
 */
void grid_print(grid_t* grid);

/********** grid_calculateVisibility **********/
/* Examine the position of the player and determine which points are visible
 *
 * Caller provides:
 *  A player grid
 *  The master grid
 *  The player ID
 *  row position
 *  column position
 * We do:
 *  Examine each quadrant of the map and create a slope from the player's position
 *  to the point being examined
 *  Check both the floor and ceiling of each point as you move through the line drawn
 *  If both the floor and ceiling are invalid points, the point shall not be visible
 * Notes:
 *  Points that were once visible will always be visible
 */
void grid_calculateVisibility(grid_t* playerGrid, grid_t* masterGrid, 
    char playerID, int x, int y);

/********** isItWall **********/
/* Examines the validity of the point provided
 *
 * Caller provides:
 *  The value (char) of the point being observed
 * We return:
 *  True if the char is invalid
 *  False if the char is valid
 */
bool isItWall(char c);

/********** grid_updateMaster **********/
/* Edits the master grid every time a player moves
 *
 * Caller provides:
 *  the master grid
 *  the position the player was at
 *  the position the player is currently at
 */
void grid_updateMaster(grid_t* master, char playerID, int oldX, int oldY, int newX, int newY, char data); 

/********** grid_refresh **********/
/* Edits each player grid every time a player moves
 *
 * Caller provides:
 *  A visibility grid from a player
 *  the player's ID
 *  the position the player was at
 *  the position the player is currently at
 */
void grid_refresh(grid_t* grid, char playerID, int oldX, int oldY, int newX, int newY, char data); 

/*************** calculateVisibility helpers ***************/
/* Each of these functions a certain quadrant of the map
 * when calculating visibility
 *
 * Caller provides:
 *  The visibility grid
 *  The player's position
 *  The point being examined
 * Notes:
 *  We examine whether the ceiling and floor of the point provided are a wall
 * We return:
 *  False if both are a wall
 *  True otherwise
 */
bool handleBottomLeft(grid_t* grid, int origR, int origC, int pointR, int pointC);
bool handleBottomRight(grid_t* grid, int origR, int origC, int pointR, int pointC);
bool handleTopLeft(grid_t* grid, int origR, int origC, int pointR, int pointC);
bool handleTopRight(grid_t* grid, int origR, int origC, int pointR, int pointC);
bool handleSameColumn(grid_t* grid, int origR, int origC, int pointR, int pointC);
bool handleSameRow(grid_t* grid, int origR, int origC, int pointR, int pointC);

/********** grid_delete **********/
/* Free the memory allocated through the initialization of the grid struct
 *
 * Caller provides:
 *  A grid they wish to be taken off the heap
 */
void grid_delete(grid_t* grid);

/*
 * Create an array holding between 10 and 30 piles of gold
 * Caller is responsible for: 
 *   Later freeing the returned array
 * Note: 
 *   Right now the maximum size of a gold pile is 29. As a result, 
 *     there are always around 15 piles created
 */
int* grid_createGoldPiles(); 

/*
 * Function to return a random int in specified range
 * We assume:
 *   srand(time(0)) has been called previously in the program
 * We return; 
 *   0, if high <= low
 *   int in range [low, high) otherwise
 */
int randInt(int low, int high); 
