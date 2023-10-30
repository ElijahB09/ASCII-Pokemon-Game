#ifndef MINHEAP_H
#define MINHEAP_H

#include "queue.h"

class MinHeap {
public:
    MinHeap(unsigned capacity);
    void insert(const terrainCell& element);
    terrainCell extractMin();
    int getSize();

private:
    void minHeapify(int index);
    void insertHelper(int index);
    terrainCell* array;
    int capacity;
    int size;
};

#endif
