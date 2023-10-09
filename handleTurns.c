#include "handleTurns.h"

#include <stdlib.h>
#include <stdio.h>

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

void insertTurns(TurnOrder * heap, Turn element) {
    if (heap->size < heap->capacity) {
        heap->arr[heap->size] = element;
        insertHelperTurns(heap, heap->size);
        heap->size++;
    }
}

void minHeapifyTurns(TurnOrder * heap, int index)
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

Turn* extractMinTurn(TurnOrder * heap)
{
    Turn *deleteItem = NULL;

    if (heap->size == 0) {
        printf("\nHeap id empty.");
        return deleteItem;
    }

    deleteItem = &heap->arr[0];
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
    turnOrder = buildPriority(num_npcs + 1);
    for (i = 0; i < num_npcs; i++) {
        t.priority = 10;
        t.characterSymbol = npcs[i]->symbol;
        t.currentMap = map;
        t.x_coord = npcs[i]->x_coord;
        t.y_coord = npcs[i]->y_coord;
        t.spawnedInTerrain = map->arr[t.y_coord][t.x_coord].terrainPiece;
        insertTurns(turnOrder, t);
    }
    t.priority = 10;
    t.y_coord = player->y_coord;
    t.x_coord = player->x_coord;
    t.currentMap = map;
    t.characterSymbol = player->symbol;
    t.spawnedInTerrain = map->arr[t.y_coord][t.x_coord].terrainPiece;
    insertTurns(turnOrder, t);

    return turnOrder;
}

void takeTurn(TurnOrder *heap, PokeMap *map) {
    Turn* t;
    int numNeighbors, i, minDistance, minX, minY;
    terrainCell neighbors[8];

    t = extractMinTurn(heap);
    printf("Character Symbol: %c\n\n", t->characterSymbol);
    if (t->characterSymbol == 'r' || t->characterSymbol == 'h') {
        numNeighbors = getNeighbors8Directions(X_BOUND, Y_BOUND, map->arr, map->arr[t->y_coord][t->x_coord], neighbors);
        minDistance = minX = minY = INT_MAX;
        for (i = 0; i < numNeighbors; i++) {
            if (t->characterSymbol == 'r') {
                if (minDistance > neighbors[i].rival_distance) {
                    minDistance = neighbors[i].rival_distance;
                    minY = neighbors[i].y_coord;
                    minX = neighbors[i].x_coord;
                }
            } else {
                if (minDistance > neighbors[i].hiker_distance) {
                    minDistance = neighbors[i].hiker_distance;
                    minY = neighbors[i].y_coord;
                    minX = neighbors[i].x_coord;
                }
            }
        }
        // Move character off of current cell
        map->arr[t->y_coord][t->x_coord].present_character = NULL;
        map->arr[t->y_coord][t->x_coord].character_present = 0;

        // Move character to new cell
        map->arr[minY][minX].present_character = &t->characterSymbol;
        map->arr[minY][minX].character_present = 1;
    } else {
        printf("Not Hiker or Rival\n\n");
    }
}