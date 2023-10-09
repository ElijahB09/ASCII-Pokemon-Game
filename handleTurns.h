#ifndef HANDLETURNS_H
#define HANDLE_TURNS_H
#include "worldgen.h"

typedef struct {
    char characterSymbol;
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
Turn extractMinTurns(TurnOrder * heap);
TurnOrder* createTurnPriority(int num_npcs, NPC *npcs[num_npcs], PlayerCharacter *player, PokeMap *map);

#endif