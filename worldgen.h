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

typedef struct {
    int x_coord, y_coord;
    char symbol;
} NPC;

PokeMap* generateMap(PokeMap *map, PokeMap* world[401][401], int map_x, int map_y, int num_npcs, PlayerCharacter *player, NPC *npcs[num_npcs]);

#endif