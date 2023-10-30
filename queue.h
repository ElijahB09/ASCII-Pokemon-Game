#ifndef QUEUE_H
#define QUEUE_H

#include "classes.h"

class Queue {
public:
    Queue(unsigned capacity);
    bool isFull() const;
    bool isEmpty() const;
    void enqueue(const terrainCell& item);
    terrainCell dequeue();
    int front, rear, size;
    int capacity;
    terrainCell *array;
};

#endif
