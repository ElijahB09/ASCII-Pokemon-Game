#include "handleTurns.h"

#include <stdlib.h>
#include <stdio.h>

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

Turn extractMinTurns(TurnOrder * heap)
{
    Turn deleteItem;
    //Adding a default value here to help get rid of warnings
    deleteItem.x_coord = deleteItem.y_coord = -1;

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
    Turn *t;
    TurnOrder *turnOrder;

    // +1 for the player
    turnOrder = buildPriority(num_npcs + 1);
    for (i = 0; i < num_npcs; i++) {
        t = malloc(sizeof (Turn));
        t->priority = 10;
        t->characterSymbol = npcs[i]->symbol;
        t->currentMap = map;
        t->x_coord = npcs[i]->x_coord;
        t->y_coord = npcs[i]->y_coord;
        insertTurns(turnOrder, *t);
    }
    t = malloc(sizeof (Turn));
    t->priority = 10;
    t->y_coord = player->y_coord;
    t->x_coord = player->x_coord;
    t->currentMap = map;
    t->characterSymbol = player->symbol;
    insertTurns(turnOrder, *t);

    return turnOrder;
}