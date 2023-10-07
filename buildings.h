#ifndef BUILDINGS_H
#define BUILDINGS_H
#define X_BOUND 80
#define Y_BOUND 21
#include "minHeap.h"

typedef struct {
    terrainCell arr[Y_BOUND][X_BOUND];
    int left_exit, right_exit, up_exit, down_exit;
    int world_x, world_y;
    int is_created;
} PokeMap;

void buildPokeStuffFancy(int x, int y, PokeMap *map);

#endif