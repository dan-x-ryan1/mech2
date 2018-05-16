//#include "depthfirstsearch.h"

#ifndef MAZEGRID_H
#define MAZEGRID_H

//consts (read only) to hold to x and y dimensions for the grid
const int x_grid = 3;
const int y_grid = 3;

//enum to hold direction
enum direction {NORTH, SOUTH, EAST, WEST};

//struct that holds all required information about each sqaure more can easily be added as you need it
struct Square {
    int x_pos_;
    int y_pos_;
    bool Walls_[4];
    int id_number_;
    int edges_[4];
    int edge_count_;
    bool visited;
    int parents;
    bool parents_set;
};

void populateEdges(Square (grid)[x_grid][y_grid], int x, int y);
int findGridSquareNumber(Square (grid)[x_grid][y_grid], int direction, int x, int y);
void populateWalls(bool (x_walls)[3][4], bool (y_walls)[4][3]);
void populateGrid(bool x_walls[x_grid][y_grid+1], bool y_walls[x_grid+1][y_grid], struct Square (grid) [x_grid][y_grid]);
void setVisited(Square grid[x_grid][y_grid], int visited);
bool checkVisited(Square grid[x_grid][y_grid], int ID);
int getXPosFromID(Square grid[x_grid][y_grid], int ID);
int getYPosFromID(Square grid[x_grid][y_grid], int ID);
bool checkParents(Square grid[x_grid][y_grid], int ID);
void SetParents(Square grid[x_grid][y_grid], int ID, int ParentID);
int GetParents(Square grid[x_grid][y_grid], int ID);



#endif // MAZEGRID_H
