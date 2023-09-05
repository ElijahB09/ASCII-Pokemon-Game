#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#define MAX_SIZE 2048
#define NUM_RAN_COORDS 20
#define MAX_ELEVATION 99

typedef struct {
    int x_coord;
    int y_coord;
    int elevation;
    char terrainPiece;
} terrainCell;

typedef struct {
    int front;
    int rear;
    terrainCell arr[MAX_SIZE];
} queue;

void enqueue(queue *arr, terrainCell element) {
    if (arr->rear == MAX_SIZE - 1) {
        printf("Queue is full");
        return;
    }
    if (arr->front == -1) {
        arr->front = 0;
    }
    arr->rear++;
    arr->arr[arr->rear] = element;
}

terrainCell dequeue(queue *arr) {
    if (arr->front == -1 || arr->front > arr->rear) {
        printf("Queue is empty");
    } else {
        terrainCell element = arr->arr[arr->front];
        arr->front++;
        return element;
    }
}

int main(int argc, char *argv[]) {
    terrainCell map[21][80];
    terrainCell randomCells[NUM_RAN_COORDS];
    terrainCell currentCell;
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
    queue seeding_queue;

    srand((unsigned int)time(NULL));

    // Generate 20 random points on the map between 1 - 78 and 1 - 19 exclusive
    for (i = 0; i < NUM_RAN_COORDS; i++) {
        rand_x_coords[i] = (rand() % 77) + 1;
        rand_y_coords[i] = (rand() % 18) + 1;
    }

    for (i = 0; i < 21; i++) {
        for (j = 0; j < 80; j++) {
            map[i][j].x_coord = j;
            map[i][j].y_coord = i;
            // Using default vals for each cell of _ and 0
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
                                randomCells[k].terrainPiece = ':';
                                randomCells[k].elevation = 2;
                                randomCells[k].x_coord = j;
                                randomCells[k].y_coord = i;
                                break;
                            case 1:
                                map[i][j].terrainPiece = '^';
                                map[i][j].elevation = 3;
                                randomCells[k].terrainPiece = '^';
                                randomCells[k].elevation = 3;
                                randomCells[k].x_coord = j;
                                randomCells[k].y_coord = i;
                                break;
                            case 2:
                                map[i][j].terrainPiece = '.';
                                map[i][j].elevation = 1;
                                randomCells[k].terrainPiece = '.';
                                randomCells[k].elevation = 1;
                                randomCells[k].x_coord = j;
                                randomCells[k].y_coord = i;
                                break;
                            case 3:
                                map[i][j].terrainPiece = '.';
                                map[i][j].elevation = 1;
                                randomCells[k].terrainPiece = '.';
                                randomCells[k].elevation = 1;
                                randomCells[k].x_coord = j;
                                randomCells[k].y_coord = i;
                                break;
                            case 4:
                                map[i][j].terrainPiece = '~';
                                map[i][j].elevation = 5;
                                randomCells[k].terrainPiece = '~';
                                randomCells[k].elevation = 5;
                                randomCells[k].x_coord = j;
                                randomCells[k].y_coord = i;
                                break;
                            case 5:
                                map[i][j].terrainPiece = '%';
                                map[i][j].elevation = 9;
                                randomCells[k].terrainPiece = '%';
                                randomCells[k].elevation = 9;
                                randomCells[k].x_coord = j;
                                randomCells[k].y_coord = i;
                                break;
                        }
                    }
                }
            }
        }
    }

    for (i = 0; i < NUM_RAN_COORDS; i++) {
        enqueue(&seeding_queue, randomCells[i]);
    }

    while (!(seeding_queue.front == -1 || seeding_queue.front > seeding_queue.rear)) {
        currentCell = dequeue(&seeding_queue);
        currentCellXCoord = currentCell.x_coord;
        currentCellYCoord = currentCell.y_coord;
        for (i = 0; i < 3; i++) {
            for (j = 0; j < 3; j++) {
                if (map[currentCellYCoord + (i - 1)][currentCellXCoord + (j - 1)].terrainPiece == '_') {
                    map[currentCellYCoord + (i - 1)][currentCellXCoord + (j - 1)].terrainPiece = currentCell.terrainPiece;
                    map[currentCellYCoord + (i - 1)][currentCellXCoord + (j - 1)].elevation = currentCell.elevation;
                    enqueue(&seeding_queue, map[currentCellYCoord + (i - 1)][currentCellXCoord + (j - 1)]);
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

    printf("Left: %d, Right: %d, Up: %d, Down: %d\n", rand_path_left, rand_path_right, rand_path_up, rand_path_down);

    for (i = 0; i < 21; i++) {
        for (j = 0; j < 80; j++) {
            printf("%c", map[i][j].terrainPiece);
        }
        printf("\n");
    }

    return 0;
}