#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main(int argc, char *argv[]) {
    char map[21][80];
    int i;
    int j;
    int k;
    int l;
    int rand_x_coords[10];
    int rand_y_coords[10];
    srand((unsigned int) time(NULL));

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
            if (i == 0 || j == 0 || i == 20 || j == 79) {
                map[i][j] = '%';
            } else {
                for (k = 0; k < 10; k++) {
                    if (j == rand_x_coords[k] && i == rand_y_coords[k]) {
                        map[i][j] = '#';
                    } else {
                        map[i][j] = '.';
                    }
                }
            }
        }
    }

    for (i = 0; i < 21; i++) {
        for (j = 0; j < 80; j++) {
            printf("%c", map[i][j]);
        }
        printf("\n");
    }

    return 0;
}