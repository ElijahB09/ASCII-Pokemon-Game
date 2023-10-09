#include "handleTurns.h"

#include <stdlib.h>
#include <stdio.h>
#include <limits.h>

#define X_BOUND 80
#define Y_BOUND 21

TurnOrder* buildPriority(unsigned capacity) {
    TurnOrder* heap = (TurnOrder *)malloc(sizeof(TurnOrder));

    if (heap == NULL) {
        printf("Uh oh, computer broke");
        return NULL;
    }

    heap->size = 0;
    heap->capacity = capacity;

    heap->arr = (Turn*) malloc(capacity * sizeof(Turn));
    if (heap->arr == NULL) {
        printf("Uh oh, computer broke");
        return NULL;
    }

    return heap;
}

void insertHelperTurns(TurnOrder * heap, int index) {
    int parent = (index - 1) / 2;

    if (heap->arr[parent].priority > heap->arr[index].priority) {
        Turn temp = heap->arr[parent];
        heap->arr[parent] = heap->arr[index];
        heap->arr[index] = temp;

        insertHelperTurns(heap, parent);
    }
}

void insertTurns(TurnOrder * heap, Turn *element) {
    if (heap->size < heap->capacity) {
        heap->arr[heap->size] = *element;
        insertHelperTurns(heap, heap->size);
        heap->size++;
    }
}

void minHeapifyTurns(TurnOrder *heap, int index)
{
    int left = index * 2 + 1;
    int right = index * 2 + 2;
    int min = index;

    if (left >= heap->size || left < 0)
        left = -1;
    if (right >= heap->size || right < 0)
        right = -1;

    if (left != -1 && heap->arr[left].priority < heap->arr[index].priority)
        min = left;
    if (right != -1 && heap->arr[right].priority < heap->arr[index].priority)
        min = right;

    if (min != index) {
        Turn temp = heap->arr[min];
        heap->arr[min] = heap->arr[index];
        heap->arr[index] = temp;

        minHeapifyTurns(heap, min);
    }
}

Turn extractMinTurn(TurnOrder * heap)
{
    Turn deleteItem;
    deleteItem.priority = -1;

    if (heap->size == 0) {
        printf("\nHeap id empty.");
        return deleteItem;
    }

    deleteItem = heap->arr[0];
    heap->arr[0] = heap->arr[heap->size - 1];
    heap->size--;

    minHeapifyTurns(heap, 0);
    return deleteItem;
}

TurnOrder* createTurnPriority(int num_npcs, NPC *npcs[num_npcs], PlayerCharacter *player, PokeMap *map) {
    int i;
    Turn t;
    TurnOrder *turnOrder;

    // +1 for the player
    turnOrder = buildPriority(num_npcs);
    for (i = 0; i < num_npcs; i++) {
        t.priority = 0;
        t.characterSymbol = npcs[i]->symbol;
        t.currentMap = map;
        t.x_coord = npcs[i]->x_coord;
        t.y_coord = npcs[i]->y_coord;
        t.spawnedInTerrain = map->arr[t.y_coord][t.x_coord].terrainPiece;
        insertTurns(turnOrder, &t);
    }
//    t.priority = 0;
//    t.y_coord = player->y_coord;
//    t.x_coord = player->x_coord;
//    t.currentMap = map;
//    t.characterSymbol = player->symbol;
//    t.spawnedInTerrain = map->arr[t.y_coord][t.x_coord].terrainPiece;
//    insertTurns(turnOrder, t);

    return turnOrder;
}

void takeTurn(TurnOrder *heap, PokeMap *map) {
    Turn t;
    int numNeighbors, i, minDistance, minX, minY;
    terrainCell neighbors[8];

    t = extractMinTurn(heap);

    printf("Character Moving: %c\n\n", t.characterSymbol);
    if (t.characterSymbol == 'r' || t.characterSymbol == 'h') {
        numNeighbors = getNeighbors8Directions(X_BOUND, Y_BOUND, map->arr, map->arr[t.y_coord][t.x_coord], neighbors);
        minDistance = minX = minY = INT_MAX;
        for (i = 0; i < numNeighbors; i++) {
            if (t.characterSymbol == 'r') {
                if (minDistance > neighbors[i].rival_total_distance && map->arr[neighbors[i].y_coord][neighbors[i].x_coord].character_present == 0) {
                    minDistance = neighbors[i].rival_total_distance;
                    minY = neighbors[i].y_coord;
                    minX = neighbors[i].x_coord;
                }
            } else {
                if (minDistance > neighbors[i].hiker_total_distance && map->arr[neighbors[i].y_coord][neighbors[i].x_coord].character_present == 0) {
                    minDistance = neighbors[i].hiker_total_distance;
                    minY = neighbors[i].y_coord;
                    minX = neighbors[i].x_coord;
                }
            }
        }
        // Move character off of current cell
        map->arr[t.y_coord][t.x_coord].character_present = 0;

        // Move character to new cell
        map->arr[minY][minX].npc = map->arr[t.y_coord][t.x_coord].npc;
        map->arr[t.y_coord][t.x_coord].npc = NULL;
        map->arr[minY][minX].character_present = 1;

        if (t.characterSymbol == 'r') {
            t.priority += map->arr[minY][minX].rival_distance;
        } else if (t.characterSymbol == 'h') {
            t.priority += map->arr[minY][minX].hiker_distance;
        }
        t.x_coord = minX;
        t.y_coord = minY;

        insertTurns(heap, &t);
    } else if (t.characterSymbol == 'p' || t.characterSymbol == 'w' || t.characterSymbol == 's' || t.characterSymbol == 'e') {
        if (t.characterSymbol == 'p') {
            if (t.direction != 'l' && t.direction != 'u' && t.direction != 'w' && t.direction != 'z' && t.direction != 'r' && t.direction != 'd' && t.direction != 'x' && t.direction != 'e') {
                int coinflip = rand() % 4;
                switch (coinflip) {
                    case 0:
                        t.direction = 'l';
                        break;
                    case 1:
                        t.direction = 'u';
                        break;
                    case 2:
                        t.direction = 'w';
                        break;
                    case 3:
                        t.direction = 'z';
                        break;
                }
            }
            switch (t.direction) {
                case 'l':
                    if (map->arr[t.y_coord][t.x_coord - 1].character_present == 0 && map->arr[t.y_coord][t.x_coord - 1].rival_distance != INT_MAX) {
                        // Move character off of current cell
                        map->arr[t.y_coord][t.x_coord].character_present = 0;

                        // Move character to new cell
                        map->arr[t.y_coord][t.x_coord - 1].npc = map->arr[t.y_coord][t.x_coord].npc;
                        map->arr[t.y_coord][t.x_coord].npc = NULL;
                        map->arr[t.y_coord][t.x_coord - 1].character_present = 1;
                        t.x_coord = t.x_coord - 1;
                        t.y_coord = t.y_coord;
                        insertTurns(heap, &t);
                    } else {
                        t.direction = 'r';
                        if (map->arr[t.y_coord][t.x_coord + 1].character_present == 0 && map->arr[t.y_coord][t.x_coord + 1].rival_distance != INT_MAX) {
                            // Move character off of current cell
                            map->arr[t.y_coord][t.x_coord].character_present = 0;

                            // Move character to new cell
                            map->arr[t.y_coord][t.x_coord + 1].npc = map->arr[t.y_coord][t.x_coord].npc;
                            map->arr[t.y_coord][t.x_coord].npc = NULL;
                            map->arr[t.y_coord][t.x_coord + 1].character_present = 1;
                            t.x_coord = t.x_coord + 1;
                            t.y_coord = t.y_coord;
                            insertTurns(heap, &t);
                        }
                    }
                    break;
                case 'r':
                    if (map->arr[t.y_coord][t.x_coord + 1].character_present == 0 && map->arr[t.y_coord][t.x_coord + 1].rival_distance != INT_MAX) {
                        // Move character off of current cell
                        map->arr[t.y_coord][t.x_coord].character_present = 0;

                        // Move character to new cell
                        map->arr[t.y_coord][t.x_coord + 1].npc = map->arr[t.y_coord][t.x_coord].npc;
                        map->arr[t.y_coord][t.x_coord].npc = NULL;
                        map->arr[t.y_coord][t.x_coord + 1].character_present = 1;
                        t.x_coord = t.x_coord + 1;
                        t.y_coord = t.y_coord;
                        insertTurns(heap, &t);
                    } else {
                        t.direction = 'l';
                        if (map->arr[t.y_coord][t.x_coord - 1].character_present == 0 && map->arr[t.y_coord][t.x_coord - 1].rival_distance != INT_MAX) {
                            // Move character off of current cell
                            map->arr[t.y_coord][t.x_coord].character_present = 0;

                            // Move character to new cell
                            map->arr[t.y_coord][t.x_coord - 1].npc = map->arr[t.y_coord][t.x_coord].npc;
                            map->arr[t.y_coord][t.x_coord].npc = NULL;
                            map->arr[t.y_coord][t.x_coord - 1].character_present = 1;
                            t.x_coord = t.x_coord - 1;
                            t.y_coord = t.y_coord;
                            insertTurns(heap, &t);
                        }
                    }
                    break;
                case 'u':
                    if (map->arr[t.y_coord - 1][t.x_coord].character_present == 0 && map->arr[t.y_coord - 1][t.x_coord].rival_distance != INT_MAX) {
                        // Move character off of current cell
                        map->arr[t.y_coord][t.x_coord].character_present = 0;

                        // Move character to new cell
                        map->arr[t.y_coord - 1][t.x_coord].npc = map->arr[t.y_coord][t.x_coord].npc;
                        map->arr[t.y_coord][t.x_coord].npc = NULL;
                        map->arr[t.y_coord - 1][t.x_coord].character_present = 1;
                        t.x_coord = t.x_coord;
                        t.y_coord = t.y_coord - 1;
                        insertTurns(heap, &t);
                    } else {
                        t.direction = 'd';
                        if (map->arr[t.y_coord + 1][t.x_coord].character_present == 0 && map->arr[t.y_coord + 1][t.x_coord].rival_distance != INT_MAX) {
                            // Move character off of current cell
                            map->arr[t.y_coord][t.x_coord].character_present = 0;

                            // Move character to new cell
                            map->arr[t.y_coord + 1][t.x_coord].npc = map->arr[t.y_coord][t.x_coord].npc;
                            map->arr[t.y_coord][t.x_coord].npc = NULL;
                            map->arr[t.y_coord + 1][t.x_coord].character_present = 1;
                            t.x_coord = t.x_coord;
                            t.y_coord = t.y_coord + 1;
                            insertTurns(heap, &t);
                        }
                    }
                    break;
                case 'd':
                    if (map->arr[t.y_coord + 1][t.x_coord].character_present == 0 && map->arr[t.y_coord + 1][t.x_coord].rival_distance != INT_MAX) {
                        // Move character off of current cell
                        map->arr[t.y_coord][t.x_coord].character_present = 0;

                        // Move character to new cell
                        map->arr[t.y_coord + 1][t.x_coord].npc = map->arr[t.y_coord][t.x_coord].npc;
                        map->arr[t.y_coord][t.x_coord].npc = NULL;
                        map->arr[t.y_coord + 1][t.x_coord].character_present = 1;
                        t.x_coord = t.x_coord;
                        t.y_coord = t.y_coord + 1;
                        insertTurns(heap, &t);
                    } else {
                        t.direction = 'u';
                        if (map->arr[t.y_coord - 1][t.x_coord].character_present == 0 && map->arr[t.y_coord - 1][t.x_coord].rival_distance != INT_MAX) {
                            // Move character off of current cell
                            map->arr[t.y_coord][t.x_coord].character_present = 0;

                            // Move character to new cell
                            map->arr[t.y_coord - 1][t.x_coord].npc = map->arr[t.y_coord][t.x_coord].npc;
                            map->arr[t.y_coord][t.x_coord].npc = NULL;
                            map->arr[t.y_coord - 1][t.x_coord].character_present = 1;
                            t.x_coord = t.x_coord;
                            t.y_coord = t.y_coord - 1;
                            insertTurns(heap, &t);
                        }
                    }
                    break;
                case 'w':
                    if (map->arr[t.y_coord - 1][t.x_coord - 1].character_present == 0 && map->arr[t.y_coord - 1][t.x_coord - 1].rival_distance != INT_MAX) {
                        // Move character off of current cell
                        map->arr[t.y_coord][t.x_coord].character_present = 0;

                        // Move character to new cell
                        map->arr[t.y_coord - 1][t.x_coord - 1].npc = map->arr[t.y_coord][t.x_coord].npc;
                        map->arr[t.y_coord][t.x_coord].npc = NULL;
                        map->arr[t.y_coord - 1][t.x_coord - 1].character_present = 1;
                        t.x_coord = t.x_coord - 1;
                        t.y_coord = t.y_coord - 1;
                        insertTurns(heap, &t);
                    } else {
                        t.direction = 'x';
                        if (map->arr[t.y_coord + 1][t.x_coord + 1].character_present == 0 && map->arr[t.y_coord + 1][t.x_coord].rival_distance != INT_MAX) {
                            // Move character off of current cell
                            map->arr[t.y_coord][t.x_coord].character_present = 0;

                            // Move character to new cell
                            map->arr[t.y_coord + 1][t.x_coord + 1].npc = map->arr[t.y_coord][t.x_coord].npc;
                            map->arr[t.y_coord][t.x_coord].npc = NULL;
                            map->arr[t.y_coord + 1][t.x_coord + 1].character_present = 1;
                            t.x_coord = t.x_coord + 1;
                            t.y_coord = t.y_coord + 1;
                            insertTurns(heap, &t);
                        }
                    }
                    break;
                case 'x':
                    if (map->arr[t.y_coord + 1][t.x_coord + 1].character_present == 0 && map->arr[t.y_coord + 1][t.x_coord + 1].rival_distance != INT_MAX) {
                        // Move character off of current cell
                        map->arr[t.y_coord][t.x_coord].character_present = 0;

                        // Move character to new cell
                        map->arr[t.y_coord + 1][t.x_coord + 1].npc = map->arr[t.y_coord][t.x_coord].npc;
                        map->arr[t.y_coord][t.x_coord].npc = NULL;
                        map->arr[t.y_coord + 1][t.x_coord + 1].character_present = 1;
                        t.x_coord = t.x_coord + 1;
                        t.y_coord = t.y_coord + 1;
                        insertTurns(heap, &t);
                    } else {
                        t.direction = 'w';
                        if (map->arr[t.y_coord - 1][t.x_coord - 1].character_present == 0 && map->arr[t.y_coord - 1][t.x_coord - 1].rival_distance != INT_MAX) {
                            // Move character off of current cell
                            map->arr[t.y_coord][t.x_coord].character_present = 0;

                            // Move character to new cell
                            map->arr[t.y_coord - 1][t.x_coord - 1].npc = map->arr[t.y_coord][t.x_coord].npc;
                            map->arr[t.y_coord][t.x_coord].npc = NULL;
                            map->arr[t.y_coord - 1][t.x_coord - 1].character_present = 1;
                            t.x_coord = t.x_coord - 1;
                            t.y_coord = t.y_coord - 1;
                            insertTurns(heap, &t);
                        }
                    }
                    break;
                case 'z':
                    if (map->arr[t.y_coord + 1][t.x_coord - 1].character_present == 0 && map->arr[t.y_coord + 1][t.x_coord - 1].rival_distance != INT_MAX) {
                        // Move character off of current cell
                        map->arr[t.y_coord][t.x_coord].character_present = 0;

                        // Move character to new cell
                        map->arr[t.y_coord + 1][t.x_coord - 1].npc = map->arr[t.y_coord][t.x_coord].npc;
                        map->arr[t.y_coord][t.x_coord].npc = NULL;
                        map->arr[t.y_coord + 1][t.x_coord - 1].character_present = 1;
                        t.x_coord = t.x_coord - 1;
                        t.y_coord = t.y_coord + 1;
                        insertTurns(heap, &t);
                    } else {
                        t.direction = 'e';
                        if (map->arr[t.y_coord - 1][t.x_coord + 1].character_present == 0 && map->arr[t.y_coord - 1][t.x_coord + 1].rival_distance != INT_MAX) {
                            // Move character off of current cell
                            map->arr[t.y_coord][t.x_coord].character_present = 0;

                            // Move character to new cell
                            map->arr[t.y_coord - 1][t.x_coord + 1].npc = map->arr[t.y_coord][t.x_coord].npc;
                            map->arr[t.y_coord][t.x_coord].npc = NULL;
                            map->arr[t.y_coord - 1][t.x_coord + 1].character_present = 1;
                            t.x_coord = t.x_coord + 1;
                            t.y_coord = t.y_coord - 1;
                            insertTurns(heap, &t);
                        }
                    }
                    break;
                case 'e':
                    if (map->arr[t.y_coord - 1][t.x_coord + 1].character_present == 0 && map->arr[t.y_coord - 1][t.x_coord + 1].rival_distance != INT_MAX) {
                        // Move character off of current cell
                        map->arr[t.y_coord][t.x_coord].character_present = 0;

                        // Move character to new cell
                        map->arr[t.y_coord - 1][t.x_coord + 1].npc = map->arr[t.y_coord][t.x_coord].npc;
                        map->arr[t.y_coord][t.x_coord].npc = NULL;
                        map->arr[t.y_coord - 1][t.x_coord + 1].character_present = 1;
                        t.x_coord = t.x_coord + 1;
                        t.y_coord = t.y_coord - 1;
                        insertTurns(heap, &t);
                    } else {
                        t.direction = 'z';
                        if (map->arr[t.y_coord + 1][t.x_coord - 1].character_present == 0 && map->arr[t.y_coord + 1][t.x_coord - 1].rival_distance != INT_MAX) {
                            // Move character off of current cell
                            map->arr[t.y_coord][t.x_coord].character_present = 0;

                            // Move character to new cell
                            map->arr[t.y_coord + 1][t.x_coord - 1].npc = map->arr[t.y_coord][t.x_coord].npc;
                            map->arr[t.y_coord][t.x_coord].npc = NULL;
                            map->arr[t.y_coord + 1][t.x_coord - 1].character_present = 1;
                            t.x_coord = t.x_coord - 1;
                            t.y_coord = t.y_coord + 1;
                            insertTurns(heap, &t);
                        }
                    }
                    break;
            }
        }
    }
}