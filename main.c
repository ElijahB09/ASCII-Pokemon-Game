#include <stdio.h>
#include <stdlib.h>
#include <time.h>

typedef struct {
    int x_coord;
    int y_coord;
    int elevation;
    char terrainPiece;
} terrainCell;

int main(int argc, char *argv[]) {
    terrainCell map[21][80];
    int i;
    int j;
    int k;
    int rand_x_coords[10];
    int rand_y_coords[10];
    //time_t t;

    srand((unsigned int)time(NULL));

    // Generate 10 random points on the map between 1 - 19 and 1 - 78 exclusive
    for (i = 0; i < 10; i++) {
        rand_x_coords[i] = (rand() % 77) + 1;
        rand_y_coords[i] = (rand() % 18) + 1;
    }

    for (i = 0; i < 10; i++) {
        printf("x-coord: %d\n", rand_x_coords[i]);
        printf("y-coord: %d\n", rand_y_coords[i]);
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
                                break;
                            case 1:
                                map[i][j].terrainPiece = '^';
                                map[i][j].elevation = 3;
                                break;
                            case 2:
                                map[i][j].terrainPiece = '%';
                                map[i][j].elevation = 9;
                                break;
                            case 3:
                                map[i][j].terrainPiece = '~';
                                map[i][j].elevation = 5;
                                break;
                            case 4:
                                map[i][j].terrainPiece = '.';
                                map[i][j].elevation = 1;
                                break;
                        }
                    }
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