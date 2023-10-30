#ifndef HANDLETURNS_H
#define HANDLETURNS_H

#include "handleBattle.h"

TurnOrder* buildPriority(unsigned capacity);
void insertTurns(TurnOrder *heap, Turn *element);
Turn extractMinTurn(TurnOrder *heap);
TurnOrder* createTurnPriority(int num_npcs, NPC *npcs[], PlayerCharacter *player, PokeMap *map);
int takeTurn(TurnOrder *heap, PokeMap *map, int num_npcs, NPC *npcs[]);

#endif
