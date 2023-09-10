#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#define QUEUE_SIZE 2048
#define HEAP_SIZE 8192
#define NUM_RAN_COORDS 30
#define MAX_ELEVATION 999
#define INFINITY 8192

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

typedef struct {
    terrainCell * arr;
    int size;
    unsigned capacity;
} MinHeap;

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

Queue* createQueue(unsigned capacity)
{
    Queue* queue = (Queue*)malloc(
            sizeof(Queue));
    queue->capacity = capacity;
    queue->front = queue->size = 0;

    // This is important, see the enqueue
    queue->rear = capacity - 1;
    queue->array = (terrainCell *)malloc(
            queue->capacity * sizeof(terrainCell));
    return queue;
}

int isFull(Queue* queue)
{
    return (queue->size == queue->capacity);
}

int isEmpty(Queue* queue)
{
    return (queue->size == 0);
}

void enqueue(Queue* queue, terrainCell item)
{
    if (isFull(queue))
        return;
    queue->rear = (queue->rear + 1)
                  % queue->capacity;
    queue->array[queue->rear] = item;
    queue->size = queue->size + 1;
}

terrainCell dequeue(Queue* queue)
{
    if (isEmpty(queue))
        printf("Queue is empty");
    terrainCell item = queue->array[queue->front];
    queue->front = (queue->front + 1) % queue->capacity;
    queue->size = queue->size - 1;
    return item;
}

terrainCell front(Queue* queue)
{
    if (isEmpty(queue))
        printf("Queue is empty");
    return queue->array[queue->front];
}

terrainCell rear(Queue* queue)
{
    if (isEmpty(queue))
        printf("Queue is empty");
    return queue->array[queue->rear];
}

int getNeighbors(int x, int y, terrainCell map[y][x], terrainCell cell, terrainCell* neighbors) {
    int numNeighbors;
    if (cell.x_coord == 0 && cell.y_coord == 0) {
        neighbors[0] = map[cell.y_coord][cell.x_coord + 1];
        neighbors[1] = map[cell.y_coord + 1][cell.x_coord];
        numNeighbors = 2;
    } else if (cell.x_coord == 79 && cell.y_coord == 0) {
        neighbors[0] = map[cell.y_coord][cell.x_coord - 1];
        neighbors[1] = map[cell.y_coord + 1][cell.x_coord];
        numNeighbors = 2;
    } else if (cell.x_coord == 0 && cell.y_coord == 20) {
        neighbors[0] = map[cell.y_coord][cell.x_coord + 1];
        neighbors[1] = map[cell.y_coord - 1][cell.x_coord];
        numNeighbors = 2;
    } else if (cell.x_coord == 79 && cell.y_coord == 20) {
        neighbors[0] = map[cell.y_coord][cell.x_coord - 1];
        neighbors[1] = map[cell.y_coord - 1][cell.x_coord];
        numNeighbors = 2;
    } else if (cell.x_coord == 0) {
        neighbors[0] = map[cell.y_coord][cell.x_coord + 1];
        neighbors[1] = map[cell.y_coord - 1][cell.x_coord];
        neighbors[2] = map[cell.y_coord + 1][cell.x_coord];
        numNeighbors = 3;
    } else if (cell.y_coord == 0) {
        neighbors[0] = map[cell.y_coord + 1][cell.x_coord];
        neighbors[1] = map[cell.y_coord][cell.x_coord + 1];
        neighbors[2] = map[cell.y_coord][cell.x_coord - 1];
        numNeighbors = 3;
    } else if (cell.x_coord == 79) {
        neighbors[0] = map[cell.y_coord][cell.x_coord - 1];
        neighbors[1] = map[cell.y_coord - 1][cell.x_coord];
        neighbors[2] = map[cell.y_coord + 1][cell.x_coord];
        numNeighbors = 3;
    } else if (cell.y_coord == 20) {
        neighbors[0] = map[cell.y_coord - 1][cell.x_coord];
        neighbors[1] = map[cell.y_coord][cell.x_coord + 1];
        neighbors[2] = map[cell.y_coord][cell.x_coord - 1];
        numNeighbors = 3;
    } else {
        neighbors[0] = map[cell.y_coord][cell.x_coord + 1];
        neighbors[1] = map[cell.y_coord][cell.x_coord - 1];
        neighbors[2] = map[cell.y_coord + 1][cell.x_coord];
        neighbors[3] = map[cell.y_coord - 1][cell.x_coord];
        numNeighbors = 4;
    }

    return numNeighbors;
}

void buildPokeStuff(int x, int y, terrainCell map[y][x]) {
    int pokemart_coord, pokestop_coord, num_neighbors, i, j, k, m, n;
    terrainCell neighbors[4];
    int pokemart_placed, pokestop_placed = 0;

    // Lower possible values because I want these to be more in the middle, not at edges
    pokemart_coord = (rand() % 65) + 5;
    pokestop_coord = (rand() % 65) + 5;
    printf("Pokemart: %d\nPokestop: %d\n\n", pokemart_coord, pokestop_coord);

    for (i = 0; i < y; i++) {
        for (j = 0; j < x; j++) {
            if (j == pokemart_coord && map[i][j].terrainPiece == '#') {
                num_neighbors = getNeighbors(x, y, map, map[i][j], neighbors);
                for (k = 0; k < num_neighbors; k++) {
                    if (neighbors[k].terrainPiece != '#' && neighbors[k].terrainPiece != 'S' && pokemart_placed == 0) {
                        map[neighbors[k].y_coord][neighbors[k].x_coord].terrainPiece = 'M';
                        map[neighbors[k].y_coord][neighbors[k].x_coord].elevation = MAX_ELEVATION;
                        pokemart_placed = 1;
                    }
                }
            }
            else if (j == pokestop_coord && map[i][j].terrainPiece == '#') {
                num_neighbors = getNeighbors(x, y, map, map[i][j], neighbors);
                for (k = 0; k < num_neighbors; k++) {
                    if (neighbors[k].terrainPiece != '#' && neighbors[k].terrainPiece != 'M' && pokestop_placed == 0) {
                        map[neighbors[k].y_coord][neighbors[k].x_coord].terrainPiece = 'S';
                        map[neighbors[k].y_coord][neighbors[k].x_coord].elevation = MAX_ELEVATION;
                        pokestop_placed = 1;
                    }
                }
            }
        }
    }
}

void Dijkstra(int x, int y, terrainCell map[y][x], terrainCell start, terrainCell end) {
    int i, j, numNeighbors, tempDistance;
    MinHeap *heap;
    terrainCell temp;
    terrainCell neighbors[4];

    for (i = 0; i < y; i++) {
        for (j = 0; j < x; j++) {
            map[i][j].distance = INFINITY;
            map[i][j].visited = 0;
            map[i][j].previous_x = -1;
            map[i][j].previous_y = -1;
        }
    }
    map[start.y_coord][start.x_coord].distance = 0;

    heap = buildMinHeap(HEAP_SIZE);
    insert(heap, map[start.y_coord][start.x_coord]);
    while (heap->size != 0) {
        temp = extractMin(heap);
        if ((temp.x_coord == end.x_coord) && (temp.y_coord == end.y_coord)) {
            break;
        }
        if (map[temp.y_coord][temp.x_coord].visited == 1) {
            continue;
        }
        map[temp.y_coord][temp.x_coord].visited = 1;
        numNeighbors = getNeighbors(x, y, map, temp, neighbors);
        for (i = 0; i < numNeighbors; i++) {
            if (map[neighbors[i].y_coord][neighbors[i].x_coord].visited == 0) {
                tempDistance = temp.distance + neighbors[i].elevation;
                if (tempDistance < neighbors[i].distance) {
                    map[neighbors[i].y_coord][neighbors[i].x_coord].distance = tempDistance;
                    map[neighbors[i].y_coord][neighbors[i].x_coord].previous_x = temp.x_coord;
                    map[neighbors[i].y_coord][neighbors[i].x_coord].previous_y = temp.y_coord;
                    insert(heap, map[neighbors[i].y_coord][neighbors[i].x_coord]);
                }
            }
        }
    }
    //printf("Finished Path finding\n\n");
    temp = map[end.y_coord][end.x_coord];
    //printf("x-coord: %d\ny-coord: %d\nprevious-x: %d\nprevious-y: %d\n\n", temp.x_coord, temp.y_coord, temp.previous_x, temp.previous_y);
    while ((temp.x_coord != start.x_coord) || (temp.y_coord != start.y_coord)) {
        map[temp.previous_y][temp.previous_x].elevation = 0;
        map[temp.previous_y][temp.previous_x].terrainPiece = '#';
        temp = map[temp.previous_y][temp.previous_x];
        //printf("Temp previous coords\nx: %d\ny: %d\n\n", temp.previous_x, temp.previous_y);
    }

    free(heap);
}

int main(int argc, char *argv[]) {
    terrainCell (*map)[80] = malloc(sizeof (terrainCell) * 21 * 80);
    terrainCell (*randomCells) = malloc(sizeof (terrainCell[NUM_RAN_COORDS]));
    terrainCell (*currentCell) = malloc(sizeof (terrainCell));
    int currentCellXCoord, currentCellYCoord, i, j, k, rand_x_coords[NUM_RAN_COORDS], rand_y_coords[NUM_RAN_COORDS], rand_path_left, rand_path_right, rand_path_up, rand_path_down;
    Queue* seeding_queue = createQueue(QUEUE_SIZE);

    srand((unsigned int) time(NULL));

    // Generate 30 random points on the map between 1 - 78 and 1 - 19 exclusive
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
            map[i][j].elevation = 0;
            if (i == 0 || j == 0 || i == 20 || j == 79) {
                map[i][j].terrainPiece = '%';
                map[i][j].elevation = MAX_ELEVATION;
            } else {
                for (k = 0; k < NUM_RAN_COORDS; k++) {
                    // Need to randomize terrain, there are :, ^, %, ., and ~
                    // Current plan, 20 coords, 5 terrain types, many seed coords hopefully generates interesting terrain
                    if (j == rand_x_coords[k] && i == rand_y_coords[k]) {
                        switch (k % 6) { // Should go between 0 - 5
                            case 0:
                                map[i][j].terrainPiece = ':';
                                map[i][j].elevation = 2;
                                randomCells[k] = map[i][j];
                                break;
                            case 1:
                                map[i][j].terrainPiece = '^';
                                map[i][j].elevation = 3;
                                randomCells[k] = map[i][j];
                                break;
                            case 2:
                                map[i][j].terrainPiece = '.';
                                map[i][j].elevation = 1;
                                randomCells[k] = map[i][j];
                                break;
                            case 3:
                                map[i][j].terrainPiece = '.';
                                map[i][j].elevation = 1;
                                randomCells[k] = map[i][j];
                                break;
                            case 4:
                                map[i][j].terrainPiece = '~';
                                map[i][j].elevation = 3;
                                randomCells[k] = map[i][j];
                                break;
                            case 5:
                                map[i][j].terrainPiece = '%';
                                map[i][j].elevation = 4;
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
                    map[currentCellYCoord + (i - 1)][currentCellXCoord + (j - 1)].elevation = currentCell->elevation;
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
    map[rand_path_left][0].elevation = 0;

    map[rand_path_right][79].terrainPiece = '#';
    map[rand_path_right][79].elevation = 0;

    map[0][rand_path_up].terrainPiece = '#';
    map[0][rand_path_up].elevation = 0;

    map[20][rand_path_down].terrainPiece = '#';
    map[20][rand_path_down].elevation = 0;

    Dijkstra(80, 21, map, map[rand_path_left][0], map[rand_path_right][79]);
    Dijkstra(80, 21, map, map[0][rand_path_up], map[20][rand_path_down]);

    buildPokeStuff(80, 21, map);

    for (i = 0; i < 21; i++) {
        for (j = 0; j < 80; j++) {
            printf("%c", map[i][j].terrainPiece);
        }
        printf("\n");
    }

    free(map);
    free(randomCells);
    free(currentCell);
    free(seeding_queue);

    return 0;
}