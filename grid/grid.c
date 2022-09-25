/* 
 * grid.c - grid module for nuggets game.
 *
 * see grid.h for more information.
 * 
 * placeholders, Winter 2022
 */

#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <stdbool.h>
#include <string.h>
#include <math.h>
#include "grid.h"
#include "../common/file.h"


/* CONSTANTS */
static const int GoldTotal = 250;      // amount of gold in the game
static const int GoldMinNumPiles = 10; // minimum number of gold piles
static const int GoldMaxNumPiles = 30; // maximum number of gold piles
 

typedef struct grid {
  char** data;
  int rows;
  int cols;
} grid_t;


/***** function prototype *****/
int grid_getRows(grid_t* grid);
int grid_getCols(grid_t* grid);
char grid_getData(grid_t* grid, int row, int col);
grid_t* grid_init();
void grid_loadMap(grid_t* grid, char* mapName);
void grid_distributeGold(grid_t* grid, int* piles);
int* grid_createGoldPiles();
bool handleTopLeft(grid_t* grid, int origR, int origC, int pointR, int pointC);
bool handleTopRight(grid_t* grid, int origR, int origC, int pointR, int pointC);
bool handleBottomLeft(grid_t* grid, int origR, int origC, int pointR, int pointC);
bool handleBottomRight(grid_t* grid, int origR, int origC, int pointR, int pointC);
bool handleSameRow(grid_t* grid, int origR, int origC, int pointR, int pointC);
bool handleSameColumn(grid_t* grid, int origR, int origC, int pointR, int pointC);
bool isItWall(char c);
char* grid_toString(grid_t* grid);
void grid_print(grid_t* grid);
void grid_updateMaster(grid_t* master, char playerID, int oldX, int oldY, int newX, int newY, char data);
void grid_refresh(grid_t* grid, char playerID, int oldX, int oldY, int newX, int newY, char data);
int randInt(int low, int high);

/********** grid_getRows **********/
int grid_getRows(grid_t* grid) {
  return grid->rows;
}

/********** grid_getCols **********/
int grid_getCols(grid_t* grid) {
  return grid->cols;
}

/********** grid_getData **********/
char grid_getData(grid_t* grid, int row, int col) 
{
  return grid->data[row][col]; 
}

void grid_setData(grid_t* grid, int row, int col, char newChar)
{
  grid->data[row][col] = newChar; 
}

/********** grid_init **********/
grid_t* grid_init() {
  grid_t* grid = malloc(sizeof(grid_t));

  if (grid == NULL) {
    return NULL;
  }
  else {
    grid->rows = 0;
    grid->cols = 0;
    return grid;
  }
}

/********** grid_init **********/
void grid_playerGridInit(grid_t* masterGrid, grid_t* playerGrid) 
{
  int numRows = grid_getRows(masterGrid); 
  int numCols = grid_getCols(masterGrid);  

  playerGrid->rows = numRows; 
  playerGrid->cols = numCols; 

  playerGrid->data = calloc(1,(numRows + 1) * sizeof(char*)); 

  char** temp = playerGrid->data; 
  for (int i = 0; i < numRows; i++) {
    temp[i] = malloc(numCols+1); 
  }

  for (int i = 0; i < numRows; i++) {
    for (int j = 0; j < numCols; j++) {
      playerGrid->data[i][j] = ' '; 
    }
    playerGrid->data[i][numCols] = '\0'; 
  }
}

/********** grid_loadMap **********/ 
void grid_loadMap(grid_t* grid, char* mapName) {
  FILE * map;
  if ((map = fopen(mapName, "r")) != NULL) { //open and validate map file

    char* length = file_readLine(map); //read first line to determine length
    grid->cols = strlen(length);
    free(length);
    rewind(map); //rewind to get accurate numLines reading
    grid->rows = file_numLines(map);
    rewind(map);

    grid->data = calloc(1,(grid->rows + 1) * sizeof(char*)); 

    char** temp = grid->data; 
    for (int i = 0; i < grid->rows; i++) {
      temp[i] = malloc(grid->cols+1); 
    }

    for (int i = 0; i < grid->rows; i++) {
      for (int j = 0; j < grid->cols; j++) {
        grid->data[i][j] = ' '; 
      }
        grid->data[i][grid->cols] = '\0'; 
    }
    
    char* line;
    int pos = 0;
    while ((line = file_readLine(map)) != NULL) { //read each line
      for (int x = 0; x < strlen(line); x++) {
        grid->data[pos][x] = line[x]; //copy to data char by char
      }
      free(line);
      pos++;
    }
    fclose(map);
  }
}

/********** grid_distributeGold **********/
void grid_distributeGold(grid_t* grid, int* piles) {
  for (int x = 0; x < GoldMaxNumPiles; x++) {//for each gold pile 
    if (piles[x] > 0) {
      int x = randInt(0, grid->rows);
      int y = randInt(0, grid->cols);

      //loop until rand generates a point that produces a '.'
      while (grid->data[x][y] != '.') {
        x = randInt(0, grid->rows);
        y = randInt(0, grid->cols);
      }
      grid->data[x][y] = '*'; //print gold on the map
    }
  }
}
/********** grid_createGoldPiles() **********/
/* see grid.h for description */
int* grid_createGoldPiles()
{    //THIS IS ALL INCOMPLETE -- HAVE TO DO DYNAMICALLY ALLOCATE ARRAY

  srand(time(0)); 

  int* goldPiles = calloc(GoldMaxNumPiles, sizeof(int)); 
  int goldAllocated = 0; 
  int rand; 
  int idx = 0; 

  while (goldAllocated < GoldTotal) {
    rand = randInt(1, GoldMaxNumPiles); //get random int between 1 and 70 inclusive
    goldPiles[idx] = rand; 

    goldAllocated += rand; 
    if (goldAllocated > GoldTotal) {
      goldPiles[idx] -= (goldAllocated-GoldTotal);
    }
    //enforce upper limit of GoldMaxNumPiles piles
    if (idx == GoldMaxNumPiles-2) {   //if we just filled the 29th pile
        goldPiles[GoldMaxNumPiles-1] = GoldTotal-goldAllocated;  //set the GoldMaxNumPiles-th pile to be equal to the remainder
        goldAllocated = GoldTotal;  
    }

    idx++; 
  }
  //index is set to first empty slot, or to last slot if all were filled

  //enforce lower limit of GoldMinNumPiles piles
  while (idx < GoldMinNumPiles) {
    for (int i = 0; i < idx; i++) {
      rand = randInt(1, goldPiles[i]); 
      if (rand == 0) {
        continue; 
      }
      goldPiles[i] -= rand; 
      goldPiles[idx] += rand; 
      idx++; 
      if ((idx) >= GoldMinNumPiles-1) {
          break; 
      }
    }
  }

  return goldPiles; 
}

/********** grid_calculateVisibility **********/
// x is the row
// y is the column
void grid_calculateVisibility(grid_t* playerGrid, grid_t* masterGrid, char playerID, int x, int y) {
  playerGrid->data[x][y] = '@';
  masterGrid->data[x][y] = playerID;
  for (int r = 0; r < grid_getRows(masterGrid); r++) {
    for (int c = 0; c < grid_getCols(masterGrid); c++) {
      if (masterGrid->data[r][c] != playerGrid->data[r][c]) {  // if player point differs from master point
        if (r == x) {
          // handle same row
          if (handleSameRow(masterGrid, x, y, r, c)) {
            playerGrid->data[r][c] = masterGrid->data[r][c];
          }
        }
        if (y == c) {
          // handle same column 
          if (handleSameColumn(masterGrid, x, y, r, c)) {
            playerGrid->data[r][c] = masterGrid->data[r][c];
          }
        }
        else if (r < x && c < y) {
          if (handleTopLeft(masterGrid, x, y, r, c)) {  // if visible
            playerGrid->data[r][c] = masterGrid->data[r][c];  // match the player point to master point
          }
        }
        else if (r < x && c > y) {
          // handle top right
          if (handleTopRight(masterGrid, x, y, r, c)) {
            playerGrid->data[r][c] = masterGrid->data[r][c];
          }
        }
        else if (r > x && c < y) {
          // handle bottom left
          if (handleBottomLeft(masterGrid, x, y, r, c)) {
            playerGrid->data[r][c] = masterGrid->data[r][c];
          }
        }
        else if (r > x && c > y) {
          // handle bottom right
          if (handleBottomRight(masterGrid, x, y, r, c)) {
            playerGrid->data[r][c] = masterGrid->data[r][c];
          }
        }
      }
      playerGrid->data[x][y] = '@';
    }
  }
  playerGrid->data[x][y] = '@';
}

bool handleTopLeft(grid_t* grid, int origR, int origC, int pointR, int pointC) {
 float slope = ((float)(origR-pointR)) / (origC-pointC);
  // for each row between the player and the point
  for(int currentR = pointR + 1; currentR< origR; currentR++){
    // calculate intersecting columns
    float intersectC = (float)((currentR - origR) / slope) + origC;
    // check to see if intersect is on gridpoint
    if(intersectC == ceil(intersectC)){
      if(isItWall(grid->data[currentR][(int)intersectC])){
        return false;
      }
    }
    // else check if surround surrounding columns are walls
    else{
      int ceilingC = ceil(intersectC);
      int floorC = floor(intersectC);
      if(isItWall(grid->data[currentR][ceilingC]) && isItWall(grid->data[currentR][floorC])){
        return false;
      }
    }
  }
  // for each column btwn player and point
  for(int currentC = pointC + 1; currentC< origC; currentC++){
    //calculate intersecting rows
    float intersectR = (float)((currentC- origC) * slope) + origR;
    //check to see if intersect is on gridpoint
    if(intersectR == ceil(intersectR)){
      if(isItWall(grid->data[(int)intersectR][(int)currentC])){
        return false;
      }
    }
    // else check if surround surrounding rows are walls
    else{
      int ceilingR = ceil(intersectR);
      int floorR = floor(intersectR);
      if(isItWall(grid->data[ceilingR][currentC]) && isItWall(grid->data[floorR][currentC])){
        return false;
      }
    }
  }
  return true;

}

bool handleTopRight(grid_t* grid, int origR, int origC, int pointR, int pointC) 
{
  float slope = ((float)(origR-pointR)) / (origC-pointC);
  // for each row between the player and the point
  for (int currentR = pointR + 1; currentR< origR; currentR++){
    // calculate intersecting columns
    float intersectC = (float)((currentR - origR) / slope) + origC;
    // check to see if intersect is on gridpoint
    if(intersectC == ceil(intersectC)){
      if(isItWall(grid->data[currentR][(int)intersectC])){
        return false;
      }
    }
    // else check if surround surrounding columns are walls
    else{
      int ceilingC = ceil(intersectC);
      int floorC = floor(intersectC);
      if(isItWall(grid->data[currentR][ceilingC]) && isItWall(grid->data[currentR][floorC])){
        return false;
      }
    }
  }
  // for each column btwn player and point
  for(int currentC = origC + 1; currentC< pointC; currentC++){
    // calculate intersecting rows
    float intersectR = (float)((currentC- origC) * slope) + origR;
    // check to see if intersect is on gridpoint
    if(intersectR == ceil(intersectR)){
      if(isItWall(grid->data[(int)intersectR][(int)currentC])){
        return false;
      }
    }
    // else check if surround surrounding rows are walls
    else{
      int ceilingR = ceil(intersectR);
      int floorR = floor(intersectR);
      if(isItWall(grid->data[ceilingR][currentC]) && isItWall(grid->data[floorR][currentC])){
        return false;
      }
    }
  }
  return true;
}

bool handleBottomLeft(grid_t* grid, int origR, int origC, int pointR, int pointC) {
  float slope = ((float)(origR-pointR)) / (origC-pointC);
  // for each row between the player and the point
  for(int currentR = origR + 1; currentR< pointR; currentR++){
    // calculate intersecting columns
    float intersectC = (float)((currentR - origR) / slope) + origC;
    // check to see if intersect is on gridpoint
    if(intersectC == ceil(intersectC)){
      if(isItWall(grid->data[currentR][(int)intersectC])){
        return false;
      }
    }
    // else check if surround surrounding columns are walls
    else{
      int ceilingC = ceil(intersectC);
      int floorC = floor(intersectC);
      if(isItWall(grid->data[currentR][ceilingC]) && isItWall(grid->data[currentR][floorC])){
        return false;
      }
    }
  }
  // for each column btwn player and point
  for(int currentC = pointC + 1; currentC< origC; currentC++){
    // calculate intersecting rows
    float intersectR = (float)((currentC- origC) * slope) + origR;
    // check to see if intersect is on gridpoint
    if(intersectR == ceil(intersectR)){
      if(isItWall(grid->data[(int)intersectR][(int)currentC])){
        return false;
      }
    }
    // else check if surround surrounding rows are walls
    else{
      int ceilingR = ceil(intersectR);
      int floorR = floor(intersectR);
      if(isItWall(grid->data[ceilingR][currentC]) && isItWall(grid->data[floorR][currentC])){
        return false;
      }
    }
  }
  return true;
 
}

bool handleBottomRight(grid_t* grid, int origR, int origC, int pointR, int pointC) {
  float slope = ((float)(origR-pointR)) / (origC-pointC);
  // for each row between the player and the point
  for(int currentR = origR + 1; currentR< pointR; currentR++){
    // calculate intersecting columns
    float intersectC = (float)((currentR - origR) / slope) + origC;
    // check to see if intersect is on gridpoint
    if(intersectC == ceil(intersectC)){
      if(isItWall(grid->data[currentR][(int)intersectC])){
        return false;
      }
    }
    // else check if surround surrounding columns are walls
    else{
      int ceilingC = ceil(intersectC);
      int floorC = floor(intersectC);
      if(isItWall(grid->data[currentR][ceilingC]) && isItWall(grid->data[currentR][floorC])){
        return false;
      }
    }
  }
  // for each column btwn player and point
  for(int currentC = origC + 1; currentC< pointC; currentC++){
    // calculate intersecting rows
    float intersectR = (float)((currentC - origC) * slope) + origR;
    // check to see if intersect is on gridpoint
    if(intersectR == ceil(intersectR)){
      if(isItWall(grid->data[(int)intersectR][(int)currentC])){
        return false;
      }
    }
    // else check if surround surrounding rows are walls
    else {
      int ceilingR = ceil(intersectR);
      int floorR = floor(intersectR);
      if(isItWall(grid->data[ceilingR][currentC]) && isItWall(grid->data[floorR][currentC])){
        return false;
      }
    }
  }
  return true;
}

bool handleSameRow(grid_t* grid, int origR, int origC, int pointR, int pointC) {
  if(origC < pointC){
    for(int currentC = origC + 1; currentC< pointC; currentC++ ){
      if(isItWall(grid->data[origR][currentC])){
        return false;
      }
    }
  }
  else{
    for(int currentC = pointC + 1; currentC< origC; currentC++ ){
      if(isItWall(grid->data[origR][currentC])){
        return false;
      }
    }
  }
  return true;
}

bool handleSameColumn(grid_t* grid, int origR, int origC, int pointR, int pointC) 
{
  if(origR < pointR){
    for(int currentR = origR + 1; currentR < pointR; currentR++ ){
        if(isItWall(grid->data[currentR][origC])){
          return false;
        }
    }
  }
  else{
    for(int currentR = pointR + 1; currentR< origR; currentR++ ){
        if(isItWall(grid->data[currentR][origC])){
          return false;
        }
    }
  }
  return true;
}

bool isItWall(char c) {
  return (c == '#' || c == '|' ||
      c == '-' || c == '+' || c == ' ');
}
/********** grid_toString **********/
char* grid_toString(grid_t* grid) {
  char* string = (char*)malloc((grid->rows+1)*(grid->cols+1)); 
  string[0] = '\0'; 
  for (int x = 0; x < grid->rows; x++) {
    if (grid->data[x] != NULL) {
      if (strlen(string) == 0) {
        strcpy(string, grid->data[x]); 
      } else {
        strcat(string, grid->data[x]); 
      }
      strcat(string, "\n"); 
    }
  }
  return string; 
}

/********** grid_print **********/
void grid_print(grid_t* grid) {
  for (int x = 0; x < grid->rows; x++) {
    if (grid->data[x] != NULL) {
      printf("%s\n", grid->data[x]);
    }
  }
}

/********** grid_updateMaster **********/
void grid_updateMaster(grid_t* master, char playerID, int oldX, int oldY, int newX, int newY, char data) {
  if (oldX < 0 && oldY < 0) {  // when a new player joins
    master->data[newX][newY] = data;
  }
  else {
    master->data[newX][newY] = playerID;
    master->data[oldX][oldY] = '.';
  }
}

/********** grid_refresh **********/
void grid_refresh(grid_t* grid, char playerID, int oldX, int oldY, int newX, int newY, char data) {
  if (oldX < 0 && oldY < 0) {  // when a new player joins
    grid->data[newX][newY] = data;
  }
  else {
    if (grid->data[oldX][oldY] == playerID) { // if the playerID was already seen
      grid->data[oldX][oldY] = '.'; // replace with .
    }
    if (!isItWall(grid->data[newX][newY])) {
      grid->data[newX][newY] = '@';
    }
  }
}

/**************** randint() ****************/
/* see grid.h for description */
int randInt(int low, int high)
{
  int bigRand = rand(); 
  if (high <= low) {
    return 0; 
  } else {
    return (bigRand % (high-low)) + low; 
  }
}


void grid_delete(grid_t* grid)
{
  char** temp = grid->data; 
  for (int i = 0; i < grid->rows; i++) {
    free(temp[i]);
  }

  free(grid->data); 
  free(grid);
}