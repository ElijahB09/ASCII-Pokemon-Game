#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#define MAX_SIZE 4096

typedef struct {
    int x_coord;
    int y_coord;
    int elevation;
    char terrainPiece;
} terrainCell;

terrainCell queue[MAX_SIZE];
int front = -1;
int rear = -1;

void enqueue(terrainCell element) {
    if (rear == MAX_SIZE - 1) {
        printf("Queue is full");
        return;
    }
    if (front == -1) {
        front = 0;
    }
    rear++;
    queue[rear] = element;
}

terrainCell dequeue() {
    if (front == -1 || front > rear) {
        printf("Queue is empty");
    } else {
        terrainCell element = queue[front];
        front++;
        return element;
    }
}

int main(int argc, char *argv[]) {
    terrainCell map[21][80];
    terrainCell randomCells[10];
    terrainCell currentCell;
    int currentCellXCoord;
    int currentCellYCoord;
    int i;
    int j;
    int k;
    int rand_x_coords[10];
    int rand_y_coords[10];

    srand((unsigned int)time(NULL));

    // Generate 10 random points on the map between 1 - 19 and 1 - 78 exclusive
    for (i = 0; i < 10; i++) {
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
                map[i][j].elevation = 9;
            } else {
                for (k = 0; k < 10; k++) {
                    // Need to randomize terrain, there are :, ^, %, ., and ~
                    // Current plan, 10 coords, 5 terrain types, 2 seed coords for each
                    if (j == rand_x_coords[k] && i == rand_y_coords[k]) {
                        switch (k % 5) { // Should go between 0 - 4
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
                                map[i][j].terrainPiece = '%';
                                map[i][j].elevation = 9;
                                randomCells[k].terrainPiece = '%';
                                randomCells[k].elevation = 9;
                                randomCells[k].x_coord = j;
                                randomCells[k].y_coord = i;
                                break;
                            case 3:
                                map[i][j].terrainPiece = '~';
                                map[i][j].elevation = 5;
                                randomCells[k].terrainPiece = '~';
                                randomCells[k].elevation = 5;
                                randomCells[k].x_coord = j;
                                randomCells[k].y_coord = i;
                                break;
                            case 4:
                                map[i][j].terrainPiece = '.';
                                map[i][j].elevation = 1;
                                randomCells[k].terrainPiece = '.';
                                randomCells[k].elevation = 1;
                                randomCells[k].x_coord = j;
                                randomCells[k].y_coord = i;
                                break;
                        }
                    }
                }
            }
        }
    }

    for (i = 0; i < 10; i++) {
        enqueue(randomCells[i]);
    }

    while (!(front == -1 || front > rear)) {
        currentCell = dequeue();
        currentCellXCoord = currentCell.x_coord;
        currentCellYCoord = currentCell.y_coord;
        for (i = 0; i < 3; i++) {
            for (j = 0; j < 3; j++) {
                if (map[currentCellYCoord + (i - 1)][currentCellXCoord + (j - 1)].terrainPiece == '_') {
                    map[currentCellYCoord + (i - 1)][currentCellXCoord + (j - 1)].terrainPiece = currentCell.terrainPiece;
                    map[currentCellYCoord + (i - 1)][currentCellXCoord + (j - 1)].elevation = currentCell.elevation;
                    enqueue(map[currentCellYCoord + (i - 1)][currentCellXCoord + (j - 1)]);
                }
            }
        }
    }

    for (i = 0; i < 21; i++) {
        for (j = 0; j < 80; j++) {
            printf("%c", map[i][j].terrainPiece);
        }
        printf("\n");
    }

    return 0;
}