#ifndef QUEUE_H
#define QUEUE_H

typedef struct {
    int x_coord, y_coord, elevation, previous_x, previous_y, visited, buildable, distance, hiker_distance, rival_distance, hiker_total_distance, rival_total_distance, rival_visited, hiker_visited, character_present;
    char terrainPiece;
    char *present_character;
} terrainCell;

typedef struct {
    int front, rear, size;
    unsigned capacity;
    terrainCell *array;
} Queue;

Queue* createQueue(unsigned capacity);
int isFull(Queue* queue);
int isEmpty(Queue* queue);
void enqueue(Queue* queue, terrainCell item);
terrainCell dequeue(Queue* queue);

#endif