#include <stdio.h>

int main(int argc, char *argv[]) {
    char map[80][21];
    int i;
    int j;

    for (i = 0; i < 80; i++) {
        for (j = 0; j < 21; j++) {
            map[i][j] = '.';
        }
    }

    for (i = 0; i < 80; i++) {
        for (j = 0; j < 21; j++) {
            printf("%c", map[i][j]);
        }
        printf("\n");
    }

    return 0;
}