#ifndef QUEUE_H

typedef struct {
    int x_coord, y_coord, elevation, previous_x, previous_y, visited;
    char terrainPiece;
    long distance;
} terrainCell;

typedef struct {
    int front, rear, size;
    unsigned capacity;
    terrainCell * array;
} Queue;

Queue* createQueue(unsigned capacity);
int isFull(Queue* queue);
int isEmpty(Queue* queue);
void enqueue(Queue* queue, terrainCell item);
terrainCell dequeue(Queue* queue);
terrainCell front(Queue* queue);
terrainCell rear(Queue* queue);

#endif