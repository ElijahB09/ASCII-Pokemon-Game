#include <stdio.h>

int main(int argc, char *argv[]) {
    char map[21][80];
    int i;
    int j;

    for (i = 0; i < 21; i++) {
        for (j = 0; j < 80; j++) {
            map[i][j] = '.';
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