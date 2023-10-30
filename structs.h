#ifndef STRUCTS_H
#define STRUCTS_H

#define Y_BOUND 21
#define X_BOUND 80

typedef struct {
    int x_coord, y_coord;
    char symbol;
} PlayerCharacter;

typedef struct {
    int x_coord, y_coord, defeated;
    char symbol;
} NPC;

typedef struct {
    int x_coord, y_coord, elevation, previous_x, previous_y, visited, buildable, distance, hiker_distance, rival_distance, hiker_total_distance, rival_total_distance, rival_visited, hiker_visited, character_present;
    char terrainPiece;
    NPC *npc;
    PlayerCharacter *player;
} terrainCell;

typedef struct {
    terrainCell * arr;
    int size;
    unsigned capacity;
} MinHeap;

typedef struct {
    char characterSymbol;
    char spawnedInTerrain;
    char direction;
    int priority, x_coord, y_coord;
} Turn;

typedef struct {
    Turn* arr;
    int size;
    unsigned capacity;
} TurnOrder;

typedef struct {
    terrainCell arr[Y_BOUND][X_BOUND];
    int left_exit, right_exit, up_exit, down_exit;
    int world_x, world_y;
    int is_created;
    TurnOrder* order;
    NPC* npcs[];
} PokeMap;

typedef struct {
    int front, rear, size;
    unsigned capacity;
    terrainCell *array;
} Queue;

#endif
