#ifndef NEIGHBORS_H
#define NEIGHBORS_H
#include "minHeap.h"

int getNeighbors(int x, int y, terrainCell map[y][x], terrainCell cell, terrainCell* neighbors);
int getNeighbors8Directions(int x, int y, terrainCell map[y][x], terrainCell cell, terrainCell* neighbors);

#endif