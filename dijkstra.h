#ifndef DIJKSTRA_H
#define DIJKSTRA_H

#include "neighbors.h"

void DijkstraTrainers(int x, int y, terrainCell map[y][x], terrainCell start);
void Dijkstra(int x, int y, terrainCell map[y][x], terrainCell start, terrainCell end);

#endif