#ifndef HANDLETURNS_H
#define HANDLETURNS_H
#include "worldgen.h"
#include "neighbors.h"

typedef struct {
    char characterSymbol;
    char spawnedInTerrain;
    int priority, x_coord, y_coord;
    PokeMap *currentMap;
} Turn;

typedef struct {
    Turn* arr;
    int size;
    unsigned capacity;
} TurnOrder;

TurnOrder* buildPriority(unsigned capacity);
void insertTurns(TurnOrder * heap, Turn element);
Turn extractMinTurn(TurnOrder * heap);
TurnOrder* createTurnPriority(int num_npcs, NPC *npcs[num_npcs], PlayerCharacter *player, PokeMap *map);
void takeTurn(TurnOrder* heap, PokeMap *map);

#endif