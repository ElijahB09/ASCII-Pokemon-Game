#ifndef WORLDGEN_H
#define WORLDGEN_H

#include "buildings.h"
#include "dijkstra.h"

#define QUEUE_SIZE 2048
#define NUM_RAN_COORDS 30
#define MAX_ELEVATION 9999

typedef struct {
    int x_coord, y_coord;
    char symbol;
} PlayerCharacter;

PokeMap* generateMap(PokeMap *map, PokeMap* world[401][401], int map_x, int map_y, PlayerCharacter *player);

#endif