#ifndef QUEUE_H
#define QUEUE_H

#include "structs.h"

Queue* createQueue(unsigned capacity);
int isFull(Queue* queue);
int isEmpty(Queue* queue);
void enqueue(Queue* queue, terrainCell item);
terrainCell dequeue(Queue* queue);

#endif