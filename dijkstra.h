#ifndef DIJKSTRA_H
#define DIJKSTRA_H

#include "neighbors.h"

void DijkstraTrainers(int x, int y, PokeMap *map, terrainCell start);
void Dijkstra(int x, int y, PokeMap *map, terrainCell start, terrainCell end);

#endif