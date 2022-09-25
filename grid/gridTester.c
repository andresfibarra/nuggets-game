/* 
 * gridTesting.c - grid module for nuggets game.
 *
 * Tester for grid.c -- currently just being used to text the gold allocation functions
 * 
 * placeholders, Winter 2022
 */

#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <string.h>

#include "grid.h"

int main(const int argc, char* argv[])
{
  /*srand(time(0)); 
  int* piles = createGoldPiles(); 
  int total = 0;
  for (int i = 0; i < 30; i++) {
    printf("goldPiles[%d] = %d\n", i, piles[i]); 
    total += piles[i];
  }

  printf("%d\n", total);*/

  grid_t* grid = grid_init();
  grid_loadMap(grid, "../maps/main.txt");
  for(int x = 0; x < grid_getRows(grid); x++) {
    for(int y = 0; y < grid_getCols(grid); y++){
      if (grid_getData(grid, x,y) == '\0') {
        printf("%c\n", grid_getData(grid, x,y));
      }
    }
  }
  int* piles = grid_createGoldPiles();
  grid_distributeGold(grid, piles);

  /*for (int x = 0; x < grid->rows; x++) {
    if (grid->data[x] != NULL) {
      printf("%s\n", grid->data[x]);
    }
  }*/
  char* str = grid_toString(grid);
  printf("%s\n", str);
  free(str);

}
