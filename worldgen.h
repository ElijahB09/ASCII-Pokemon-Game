#ifndef WORLDGEN_H
#define WORLDGEN_H

#define QUEUE_SIZE 2048
#define NUM_RAN_COORDS 30
#define MAX_ELEVATION 9999

#include "buildings.h"

PokeMap* generateMap(PokeMap *map, PokeMap* world[401][401], int map_x, int map_y, int num_npcs, PlayerCharacter *player, NPC *npcs[num_npcs]);

#endif