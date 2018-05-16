

#include "c_stack.h"
#include "mazegrid.h"


#ifndef DEPTHFIRSTSEARCH_H
#define DEPTHFIRSTSEARCH_H

const int MAX_ROUTE_SIZE = 20; //guess of 20 

void reconstruct_path(Square grid[x_grid][y_grid], int current, int *routetoosteps, int routetoo[MAX_ROUTE_SIZE]);
void depthfirstsearch(Square grid[x_grid][y_grid], int current, int goal, int routetoo[MAX_ROUTE_SIZE], int* routesize);

void directionFromRoute(Square grid[x_grid][y_grid], int route[MAX_ROUTE_SIZE], direction routedirection[MAX_ROUTE_SIZE], int routesize);

#endif // DEPTHFIRSTSEARCH_H
