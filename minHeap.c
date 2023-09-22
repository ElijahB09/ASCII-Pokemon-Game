#include "minHeap.h"
#include <stdlib.h>
#include <stdio.h>

MinHeap* buildMinHeap(unsigned capacity) {
    MinHeap* heap = (MinHeap *)malloc(sizeof(MinHeap));

    if (heap == NULL) {
        printf("Uh oh, computer broke");
        return NULL;
    }

    heap->size = 0;
    heap->capacity = capacity;

    heap->arr = (terrainCell *)malloc(capacity * sizeof(terrainCell));
    if (heap->arr == NULL) {
        printf("Uh oh, computer broke");
        return NULL;
    }

    return heap;
}

void insertHelper(MinHeap* heap, int index) {
    int parent = (index - 1) / 2;

    if (heap->arr[parent].distance > heap->arr[index].distance) {
        terrainCell temp = heap->arr[parent];
        heap->arr[parent] = heap->arr[index];
        heap->arr[index] = temp;

        insertHelper(heap, parent);
    }
}

void insert(MinHeap* heap, terrainCell element) {
    if (heap->size < heap->capacity) {
        heap->arr[heap->size] = element;
        insertHelper(heap, heap->size);
        heap->size++;
    }
}

void minHeapify(MinHeap * heap, int index)
{
    int left = index * 2 + 1;
    int right = index * 2 + 2;
    int min = index;

    if (left >= heap->size || left < 0)
        left = -1;
    if (right >= heap->size || right < 0)
        right = -1;

    if (left != -1 && heap->arr[left].distance < heap->arr[index].distance)
        min = left;
    if (right != -1 && heap->arr[right].distance < heap->arr[index].distance)
        min = right;

    if (min != index) {
        terrainCell temp = heap->arr[min];
        heap->arr[min] = heap->arr[index];
        heap->arr[index] = temp;

        minHeapify(heap, min);
    }
}

terrainCell extractMin(MinHeap* heap)
{
    terrainCell deleteItem;
    //Adding a default value here to help get rid of warnings
    deleteItem.x_coord = deleteItem.y_coord = -1;

    if (heap->size == 0) {
        printf("\nHeap id empty.");
        return deleteItem;
    }

    deleteItem = heap->arr[0];
    heap->arr[0] = heap->arr[heap->size - 1];
    heap->size--;

    minHeapify(heap, 0);
    return deleteItem;
}