# CS50 Nuggets 'Grid' Directory

## Group 6 'Placeholders'
### March 2022

## Grid
The grid directory houses both the c file and headerfile for the Grid module. 
The grid struct contains a 2D array representation of the map. The struct also
contains information regarding the max rows and columns of the map used for the game. 
The module includes methods including initializing a grid, loading the master map,
calculating visibility for players at a certain location, and updating all grids when players
move or other shifts in the game.

### Grid struct
1. A double pointer to char, representing a 2D array of characters that will act as
the data of the map
2. An integer storing the max amount of rows the map contains, found through the `file_numLines`
function in the common directory.
3. An integer storing the max amount of columns the map contains, found by retrieving the
string length of any row in the map

### loadMap
`grid_loadMap` is responsible for building the master grid and allocating memory for its
contents. 
A file path to a map is provided, and the function opens the file and reads the contents
to determine the values for data, rows, and cols.
The caller waits to call `grid_distributeGold` until after the map is already loaded

### calculateVisibility
`grid_calculateVisibility` finds any previously-invisible points and evaluates them in
correlation to the player's position to determine if they are now visible
We adopted the method for how to calculate the visibility of each point from the knowledge
units and videos provided in class.
Multiple helper methods are provided, like `handleTopLeft`, `handleTopRight`, and so on, to
make calculations for visibility smoother and more efficient.
Each helper returns a boolean, determining whether the point in question is now visible

