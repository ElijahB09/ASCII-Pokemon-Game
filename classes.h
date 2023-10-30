#ifndef CLASSES_H
#define CLASSES_H

#define Y_BOUND 21
#define X_BOUND 80

class PlayerCharacter {
public:
    int x_coord, y_coord;
    char symbol;
};

class NPC {
public:
    int x_coord, y_coord, defeated;
    char symbol;
};

class terrainCell {
public:
    int x_coord, y_coord, elevation, previous_x, previous_y, visited, buildable, distance, hiker_distance, rival_distance, hiker_total_distance, rival_total_distance, rival_visited, hiker_visited, character_present;
    char terrainPiece;
    NPC *npc;
    PlayerCharacter *player;
};

class Turn {
public:
    char characterSymbol;
    char spawnedInTerrain;
    char direction;
    int priority, x_coord, y_coord;
};

class TurnOrder {
public:
    Turn* arr;
    int size;
    int capacity;
};

class PokeMap {
public:
    terrainCell arr[Y_BOUND][X_BOUND];
    int left_exit, right_exit, up_exit, down_exit;
    int world_x, world_y;
    int is_created;
    TurnOrder* order;
    NPC** npcs;
};

#endif
