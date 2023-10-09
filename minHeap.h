#ifndef MINHEAP_H
#define MINHEAP_H

#include "queue.h"

MinHeap* buildMinHeap(unsigned capacity);
void insert(MinHeap* heap, terrainCell element);
void minHeapify(MinHeap * heap, int index);
terrainCell extractMin(MinHeap* heap);

#endif