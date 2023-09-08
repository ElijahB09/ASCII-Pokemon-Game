#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#define QUEUE_SIZE 2048
#define NUM_RAN_COORDS 30
#define MAX_ELEVATION 99

typedef struct {
    int x_coord;
    int y_coord;
    int elevation;
    char terrainPiece;
} terrainCell;

typedef struct {
    int front, rear, size;
    int capacity;
    terrainCell *array;
} Queue;

// function to create a queue
// of given capacity.
// It initializes size of queue as 0
Queue* createQueue(int capacity)
{
    Queue* queue = (Queue*)malloc(
            sizeof(Queue));
    queue->capacity = capacity;
    queue->front = queue->size = 0;

    // This is important, see the enqueue
    queue->rear = capacity - 1;
    queue->array = (terrainCell *)malloc(
            queue->capacity * sizeof(int));
    return queue;
}

// Queue is full when size becomes
// equal to the capacity
int isFull(Queue* queue)
{
    return (queue->size == queue->capacity);
}

// Queue is empty when size is 0
int isEmpty(Queue* queue)
{
    return (queue->size == 0);
}

// Function to add an item to the queue.
// It changes rear and size
void enqueue(Queue* queue, terrainCell item)
{
    if (isFull(queue))
        return;
    queue->rear = queue->rear + 1 % queue->capacity;
    queue->array[queue->rear] = item;
    queue->size = queue->size + 1;
}

// Function to remove an item from queue.
// It changes front and size
terrainCell dequeue(Queue* queue)
{
    if (isEmpty(queue))
        printf("Queue is empty");
    terrainCell item = queue->array[queue->front];
    queue->front = (queue->front + 1) % queue->capacity;
    queue->size = queue->size - 1;
    return item;
}

// Function to get front of queue
terrainCell front(Queue* queue)
{
    if (isEmpty(queue))
        printf("Queue is empty");
    return queue->array[queue->front];
}

// Function to get rear of queue
terrainCell rear(Queue* queue)
{
    if (isEmpty(queue))
        printf("Queue is empty");
    return queue->array[queue->rear];
}

//void Dijkstra(int x, int y, terrainCell map[y][x], int elevations[y][x], terrainCell start) {
//
//}

int main(int argc, char *argv[]) {
    terrainCell (*map)[80] = malloc(sizeof (terrainCell[21][80]));
    int (*elevations)[80] = malloc(sizeof (int[21][80]));
    terrainCell (*randomCells) = malloc(sizeof (terrainCell[NUM_RAN_COORDS]));
    terrainCell (*currentCell) = malloc(sizeof (terrainCell));
    int currentCellXCoord;
    int currentCellYCoord;
    int i;
    int j;
    int k;
    int rand_x_coords[NUM_RAN_COORDS];
    int rand_y_coords[NUM_RAN_COORDS];
    int rand_path_left;
    int rand_path_right;
    int rand_path_up;
    int rand_path_down;
    Queue* seeding_queue = createQueue(QUEUE_SIZE);

    srand((unsigned int) time(NULL));

    // Generate 20 random points on the map between 1 - 78 and 1 - 19 exclusive
    for (i = 0; i < NUM_RAN_COORDS; i++) {
        rand_x_coords[i] = (rand() % 77) + 1;
        rand_y_coords[i] = (rand() % 18) + 1;
    }

    for (i = 0; i < 21; i++) {
        for (j = 0; j < 80; j++) {
            map[i][j].x_coord = j;
            map[i][j].y_coord = i;
            // Using default values for each cell of _ and 0
            map[i][j].terrainPiece = '_';
            map[i][j].elevation = elevations[i][j] = 0;
            if (i == 0 || j == 0 || i == 20 || j == 79) {
                map[i][j].terrainPiece = '%';
                map[i][j].elevation = elevations[i][j] = MAX_ELEVATION;
            } else {
                for (k = 0; k < NUM_RAN_COORDS; k++) {
                    // Need to randomize terrain, there are :, ^, %, ., and ~
                    // Current plan, 20 coords, 5 terrain types, many seed coords hopefully generates interesting terrain
                    if (j == rand_x_coords[k] && i == rand_y_coords[k]) {
                        switch (k % 6) { // Should go between 0 - 5
                            case 0:
                                map[i][j].terrainPiece = ':';
                                map[i][j].elevation = elevations[i][j] = 2;
                                randomCells[k] = map[i][j];
                                break;
                            case 1:
                                map[i][j].terrainPiece = '^';
                                map[i][j].elevation = elevations[i][j] = 3;
                                randomCells[k] = map[i][j];
                                break;
                            case 2:
                                map[i][j].terrainPiece = '.';
                                map[i][j].elevation = elevations[i][j] = 1;
                                randomCells[k] = map[i][j];
                                break;
                            case 3:
                                map[i][j].terrainPiece = '.';
                                map[i][j].elevation = elevations[i][j] = 1;
                                randomCells[k] = map[i][j];
                                break;
                            case 4:
                                map[i][j].terrainPiece = '~';
                                map[i][j].elevation = elevations[i][j] = 5;
                                randomCells[k] = map[i][j];
                                break;
                            case 5:
                                map[i][j].terrainPiece = '%';
                                map[i][j].elevation = elevations[i][j] = 6;
                                randomCells[k] = map[i][j];
                                break;
                        }
                    }
                }
            }
        }
    }

    for (i = 0; i < NUM_RAN_COORDS; i++) {
        enqueue(seeding_queue, randomCells[i]);
    }

    while (!(seeding_queue->front == -1 || seeding_queue->front > seeding_queue->rear)) {
        *currentCell = dequeue(seeding_queue);
        currentCellXCoord = currentCell->x_coord;
        currentCellYCoord = currentCell->y_coord;
        for (i = 0; i < 3; i++) {
            for (j = 0; j < 3; j++) {
                if (map[currentCellYCoord + (i - 1)][currentCellXCoord + (j - 1)].terrainPiece == '_') {
                    map[currentCellYCoord + (i - 1)][currentCellXCoord + (j - 1)].terrainPiece = currentCell->terrainPiece;
                    map[currentCellYCoord + (i - 1)][currentCellXCoord + (j - 1)].elevation = elevations[i][j] = currentCell->elevation;
                    enqueue(seeding_queue, map[currentCellYCoord + (i - 1)][currentCellXCoord + (j - 1)]);
                }
            }
        }
    }

    rand_path_left = (rand() % 18) + 1;
    rand_path_right = (rand() % 18) + 1;
    rand_path_up = (rand() % 77) + 1;
    rand_path_down = (rand() % 77) + 1;

    map[rand_path_left][0].terrainPiece = '#';
    map[rand_path_left][0].elevation = elevations[rand_path_left][0] = 0;

    map[rand_path_right][79].terrainPiece = '#';
    map[rand_path_right][79].elevation = elevations[rand_path_right][79] = 0;

    map[0][rand_path_up].terrainPiece = '#';
    map[0][rand_path_up].elevation = elevations[0][rand_path_up] = 0;

    map[20][rand_path_down].terrainPiece = '#';
    map[20][rand_path_down].elevation = elevations[20][rand_path_down] = 0;

    printf("Left: %d, Right: %d, Up: %d, Down: %d\n", rand_path_left, rand_path_right, rand_path_up, rand_path_down);

    //Dijkstra(80, 21, map, elevations, map[rand_path_left][0]);

    for (i = 0; i < 21; i++) {
        for (j = 0; j < 80; j++) {
            printf("%c", map[i][j].terrainPiece);
        }
        printf("\n");
    }

    free(map);
    free(elevations);
    free(randomCells);
    free(currentCell);

    return 0;
}