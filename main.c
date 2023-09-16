#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "minHeap.h"
#define QUEUE_SIZE 2048
#define HEAP_SIZE 8192
#define NUM_RAN_COORDS 30
#define MAX_ELEVATION 999
#define INFINITY 8192
#define X_BOUND 80
#define Y_BOUND 21

typedef struct {
    terrainCell arr[Y_BOUND][X_BOUND];
    int left_exit, right_exit, up_exit, down_exit;
    int world_x, world_y;
    int is_created;
} PokeMap;

int getNeighbors(int x, int y, terrainCell map[y][x], terrainCell cell, terrainCell* neighbors) {
    int numNeighbors;
    if (cell.x_coord == 0 && cell.y_coord == 0) {
        neighbors[0] = map[cell.y_coord][cell.x_coord + 1];
        neighbors[1] = map[cell.y_coord + 1][cell.x_coord];
        numNeighbors = 2;
    } else if (cell.x_coord == X_BOUND - 1 && cell.y_coord == 0) {
        neighbors[0] = map[cell.y_coord][cell.x_coord - 1];
        neighbors[1] = map[cell.y_coord + 1][cell.x_coord];
        numNeighbors = 2;
    } else if (cell.x_coord == 0 && cell.y_coord == Y_BOUND - 1) {
        neighbors[0] = map[cell.y_coord][cell.x_coord + 1];
        neighbors[1] = map[cell.y_coord - 1][cell.x_coord];
        numNeighbors = 2;
    } else if (cell.x_coord == X_BOUND - 1 && cell.y_coord == Y_BOUND - 1) {
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
    } else if (cell.x_coord == X_BOUND - 1) {
        neighbors[0] = map[cell.y_coord][cell.x_coord - 1];
        neighbors[1] = map[cell.y_coord - 1][cell.x_coord];
        neighbors[2] = map[cell.y_coord + 1][cell.x_coord];
        numNeighbors = 3;
    } else if (cell.y_coord == Y_BOUND - 1) {
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

void buildRoadsToPokeStuff(int x, int y, terrainCell map[y][x], int pokeMX, int pokeMY, int pokeCX, int pokeCY) {
    int i, j, k, numNeighbors, mart_done, center_done;
    terrainCell neighbors[4];
    mart_done = center_done = 0;

    for (i = 0; i < y; i++) {
        for (j = 0; j < x; j++) {
            if (map[i][j].terrainPiece == '#') {
                numNeighbors = getNeighbors(x, y, map, map[i][j], neighbors);
                for (k = 0; k < numNeighbors; k++) {
                    if (neighbors[k].terrainPiece == 'M') {
                        mart_done = 1;
                    }
                    else if (neighbors[k].terrainPiece == 'C') {
                        center_done = 1;
                    }
                }
            }
        }
    }

    if (mart_done == 0) {
        for (i = 0; i < y; i++) {
            if (map[i][pokeMX].terrainPiece == '#' && i - 1 > pokeMY) {
                for (j = pokeMY + 1; j < i; j++) {
                    map[j][pokeMX].terrainPiece = '#';
                    map[j][pokeMX].elevation = 0;
                }
            }
            else if (map[i][pokeMX].terrainPiece == '#' && i + 1 < pokeMY - 1) {
                for (j = pokeMY - 2; j > i; j--) {
                    map[j][pokeMX].terrainPiece = '#';
                    map[j][pokeMX].elevation = 0;
                }
            }
        }
    }
    if (center_done == 0) {
        for (i = 0; i < y; i++) {
            if (map[i][pokeCX].terrainPiece == '#' && i - 1 > pokeCY) {
                for (j = pokeCY + 1; j < i; j++) {
                    map[j][pokeCX].terrainPiece = '#';
                    map[j][pokeCX].elevation = 0;
                }
            }
            else if (map[i][pokeCX].terrainPiece == '#' && i + 1 < pokeCY - 1) {
                for (j = pokeCY - 2; j > i; j--) {
                    map[j][pokeCX].terrainPiece = '#';
                    map[j][pokeCX].elevation = 0;
                }
            }
        }
    }
}

void buildPokeStuff(int x, int y, terrainCell map[y][x]) {
    int pokemart_x_coord, pokecenter_x_coord, pokemart_y_coord, pokecenter_y_coord;
    int pokemart_placed = 0;
    int pokecenter_placed = 0;

    // Lower possible values because I want these to be more in the middle, not at edges
    pokemart_x_coord = (rand() % 65) + 5;
    pokecenter_x_coord = (rand() % 65) + 5;
    pokemart_y_coord = (rand() % 15) + 3;
    pokecenter_y_coord = (rand() % 15) + 3;

    while (pokemart_placed == 0) {
        if (map[pokemart_y_coord][pokemart_x_coord].terrainPiece == '#' || map[pokemart_y_coord][pokemart_x_coord].terrainPiece == 'M' || map[pokemart_y_coord][pokemart_x_coord].terrainPiece == 'C') {
            pokemart_x_coord = (rand() % 65) + 5;
            pokemart_y_coord = (rand() % 15) + 3;
        }
        else {
            if (map[pokemart_y_coord - 1][pokemart_x_coord - 1].terrainPiece != '#' && map[pokemart_y_coord - 1][pokemart_x_coord - 1].terrainPiece != 'M' && map[pokemart_y_coord - 1][pokemart_x_coord - 1].terrainPiece != 'C') {
                if (map[pokemart_y_coord - 1][pokemart_x_coord].terrainPiece != '#' && map[pokemart_y_coord - 1][pokemart_x_coord].terrainPiece != 'M' && map[pokemart_y_coord - 1][pokemart_x_coord].terrainPiece != 'C') {
                    if (map[pokemart_y_coord][pokemart_x_coord - 1].terrainPiece != '#' && map[pokemart_y_coord][pokemart_x_coord - 1].terrainPiece != 'M' && map[pokemart_y_coord][pokemart_x_coord - 1].terrainPiece != 'C') {
                        map[pokemart_y_coord][pokemart_x_coord].terrainPiece = 'M';
                        map[pokemart_y_coord - 1][pokemart_x_coord].terrainPiece = 'M';
                        map[pokemart_y_coord][pokemart_x_coord - 1].terrainPiece = 'M';
                        map[pokemart_y_coord - 1][pokemart_x_coord - 1].terrainPiece = 'M';
                        pokemart_placed = 1;
                    } else {
                        pokemart_x_coord = (rand() % 65) + 5;
                        pokemart_y_coord = (rand() % 15) + 3;
                    }
                } else {
                    pokemart_x_coord = (rand() % 65) + 5;
                    pokemart_y_coord = (rand() % 15) + 3;
                }
            }
            else {
                pokemart_x_coord = (rand() % 65) + 5;
                pokemart_y_coord = (rand() % 15) + 3;
            }
        }
    }

    while (pokecenter_placed == 0) {
        if (map[pokecenter_y_coord][pokecenter_x_coord].terrainPiece == '#' || map[pokecenter_y_coord][pokecenter_x_coord].terrainPiece == 'M' || map[pokecenter_y_coord][pokecenter_x_coord].terrainPiece == 'C') {
            pokecenter_x_coord = (rand() % 65) + 5;
            pokecenter_y_coord = (rand() % 15) + 3;
        }
        else {
            if (map[pokecenter_y_coord - 1][pokecenter_x_coord - 1].terrainPiece != '#' && map[pokecenter_y_coord - 1][pokecenter_x_coord - 1].terrainPiece != 'M' && map[pokecenter_y_coord - 1][pokecenter_x_coord - 1].terrainPiece != 'C') {
                if (map[pokecenter_y_coord - 1][pokecenter_x_coord].terrainPiece != '#' && map[pokecenter_y_coord - 1][pokecenter_x_coord].terrainPiece != 'M' && map[pokecenter_y_coord - 1][pokecenter_x_coord].terrainPiece != 'C' && pokemart_x_coord != pokecenter_x_coord) {
                    if (map[pokecenter_y_coord][pokecenter_x_coord - 1].terrainPiece != '#' && map[pokecenter_y_coord][pokecenter_x_coord - 1].terrainPiece != 'M' && map[pokecenter_y_coord][pokecenter_x_coord - 1].terrainPiece != 'C' && pokemart_x_coord != pokecenter_x_coord) {
                        map[pokecenter_y_coord][pokecenter_x_coord].terrainPiece = 'C';
                        map[pokecenter_y_coord - 1][pokecenter_x_coord].terrainPiece = 'C';
                        map[pokecenter_y_coord][pokecenter_x_coord - 1].terrainPiece = 'C';
                        map[pokecenter_y_coord - 1][pokecenter_x_coord - 1].terrainPiece = 'C';
                        pokecenter_placed = 1;
                    } else {
                        pokecenter_x_coord = (rand() % 65) + 5;
                        pokecenter_y_coord = (rand() % 15) + 3;
                    }
                } else {
                    pokecenter_x_coord = (rand() % 65) + 5;
                    pokecenter_y_coord = (rand() % 15) + 3;
                }
            }
            else {
                pokecenter_x_coord = (rand() % 65) + 5;
                pokecenter_y_coord = (rand() % 15) + 3;
            }
        }
    }
    buildRoadsToPokeStuff(x, y, map, pokemart_x_coord, pokemart_y_coord, pokecenter_x_coord, pokecenter_y_coord);
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
    temp = map[end.y_coord][end.x_coord];
    while ((temp.x_coord != start.x_coord) || (temp.y_coord != start.y_coord)) {
        map[temp.previous_y][temp.previous_x].elevation = 0;
        map[temp.previous_y][temp.previous_x].terrainPiece = '#';
        temp = map[temp.previous_y][temp.previous_x];
    }

    free(heap->arr);
    free(heap);
}

PokeMap* generateMap(int x, int y, PokeMap (*world)[401], int map_x, int map_y) {
    printf("%d\nx: %d\ny: %d\n", world[map_y][map_x].is_created, map_x, map_y);
    if (world[map_y][map_x].is_created == 1) {
        return  &world[map_y][map_x];
    }

    PokeMap *map = malloc(sizeof (PokeMap));
    terrainCell (*randomCells) = malloc(sizeof (terrainCell[NUM_RAN_COORDS]));
    terrainCell (*currentCell) = malloc(sizeof (terrainCell));
    int currentCellXCoord, currentCellYCoord, i, j, k, rand_x_coords[NUM_RAN_COORDS], rand_y_coords[NUM_RAN_COORDS], rand_path_left, rand_path_right, rand_path_up, rand_path_down;
    Queue* seeding_queue = createQueue(QUEUE_SIZE);
    // Generate 30 random points on the map between 1 - 78 and 1 - 19 exclusive
    for (i = 0; i < NUM_RAN_COORDS; i++) {
        rand_x_coords[i] = (rand() % 77) + 1;
        rand_y_coords[i] = (rand() % 18) + 1;
    }

    for (i = 0; i < Y_BOUND; i++) {
        for (j = 0; j < X_BOUND; j++) {
            map->arr[i][j].x_coord = j;
            map->arr[i][j].y_coord = i;
            // Using default values for each cell of _ and 0
            map->arr[i][j].terrainPiece = '_';
            map->arr[i][j].elevation = 0;
            if (i == 0 || j == 0 || i == Y_BOUND - 1 || j == X_BOUND - 1) {
                map->arr[i][j].terrainPiece = '%';
                map->arr[i][j].elevation = MAX_ELEVATION;
            } else {
                for (k = 0; k < NUM_RAN_COORDS; k++) {
                    // Need to randomize terrain, there are :, ^, %, ., and ~
                    // Current plan, Y_BOUND - 1 coords, 5 terrain types, many seed coords hopefully generates interesting terrain
                    if (j == rand_x_coords[k] && i == rand_y_coords[k]) {
                        switch (k % 6) { // Should go between 0 - 5
                            case 0:
                                map->arr[i][j].terrainPiece = ':';
                                map->arr[i][j].elevation = 2;
                                randomCells[k] = map->arr[i][j];
                                break;
                            case 1:
                                map->arr[i][j].terrainPiece = '^';
                                map->arr[i][j].elevation = 3;
                                randomCells[k] = map->arr[i][j];
                                break;
                            case 2:
                            case 3:
                                map->arr[i][j].terrainPiece = '.';
                                map->arr[i][j].elevation = 1;
                                randomCells[k] = map->arr[i][j];
                                break;
                            case 4:
                                map->arr[i][j].terrainPiece = '~';
                                map->arr[i][j].elevation = 3;
                                randomCells[k] = map->arr[i][j];
                                break;
                            case 5:
                                map->arr[i][j].terrainPiece = '%';
                                map->arr[i][j].elevation = 4;
                                randomCells[k] = map->arr[i][j];
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
                if (map->arr[currentCellYCoord + (i - 1)][currentCellXCoord + (j - 1)].terrainPiece == '_') {
                    map->arr[currentCellYCoord + (i - 1)][currentCellXCoord + (j - 1)].terrainPiece = currentCell->terrainPiece;
                    map->arr[currentCellYCoord + (i - 1)][currentCellXCoord + (j - 1)].elevation = currentCell->elevation;
                    enqueue(seeding_queue, map->arr[currentCellYCoord + (i - 1)][currentCellXCoord + (j - 1)]);
                }
            }
        }
    }
    map->world_x = map_x;
    map->world_y = map_y;
    map->is_created = 1;

    if (map_x == 400 && map_y == 400) {
        // lower right

    } else if (map_x == 400 && map_y == 0) {
        // Upper right

    } else if (map_x == 0 && map_y == 400) {
        // Lower left

    } else if (map_x == 0 && map_y == 0) {
        // Upper left

    } else if (map_x == 400) {

    } else if (map_x == 0) {

    } else if (map_y == 400) {

    } else if (map_y == 0) {

    } else {
        rand_path_right = (rand() % 18) + 1;
        rand_path_left = (rand() % 18) + 1;
        rand_path_down = (rand() % 77) + 1;
        rand_path_up = (rand() % 77) + 1;

        map->left_exit = rand_path_left;
        map->right_exit = rand_path_right;
        map->up_exit = rand_path_up;
        map->down_exit = rand_path_down;

        map->arr[rand_path_left][0].terrainPiece = '#';
        map->arr[rand_path_left][0].elevation = 0;

        map->arr[rand_path_right][X_BOUND - 1].terrainPiece = '#';
        map->arr[rand_path_right][X_BOUND - 1].elevation = 0;

        map->arr[0][rand_path_up].terrainPiece = '#';
        map->arr[0][rand_path_up].elevation = 0;

        map->arr[Y_BOUND - 1][rand_path_down].terrainPiece = '#';
        map->arr[Y_BOUND - 1][rand_path_down].elevation = 0;

        Dijkstra(X_BOUND, Y_BOUND, map->arr, map->arr[rand_path_left][0], map->arr[rand_path_right][X_BOUND - 1]);
        Dijkstra(X_BOUND, Y_BOUND, map->arr, map->arr[0][rand_path_up], map->arr[Y_BOUND - 1][rand_path_down]);

        buildPokeStuff(X_BOUND, Y_BOUND, map->arr);
    }

    free(randomCells);
    free(currentCell);
    free(seeding_queue->array);
    free(seeding_queue);

    world[map_y][map_x] = *map;

    return &world[map_y][map_x];
}

int main(int argc, char *argv[]) {
    PokeMap (*world)[401] = malloc(sizeof (PokeMap[401][401]));
    PokeMap *currentMap = malloc(sizeof (PokeMap));
    int current_x, current_y, i, j;
    char userInput;

    srand(time(NULL));

    current_x = current_y = 200;
    currentMap = generateMap(X_BOUND, Y_BOUND, world, current_x, current_y);

    userInput = 'x';

    while (userInput != 'q') {
        for (i = 0; i < Y_BOUND; i++) {
            for (j = 0; j < X_BOUND; j++) {
                printf("%c", currentMap->arr[i][j].terrainPiece);
            }
            printf("\n");
        }
        printf("You are currently at position (%d, %d). Input command:", currentMap->world_x, currentMap->world_y);
        scanf(" %c", &userInput);
        printf("\n");
        switch (userInput) {
            case 'l':
                current_x--;
                currentMap = generateMap(X_BOUND, Y_BOUND, world, current_x, current_y);
                break;
            case 'r':
                current_x++;
                currentMap = generateMap(X_BOUND, Y_BOUND, world, current_x, current_y);
                break;
            case 'u':
                current_y--;
                currentMap = generateMap(X_BOUND, Y_BOUND, world, current_x, current_y);
                break;
            case 'd':
                current_y++;
                currentMap = generateMap(X_BOUND, Y_BOUND, world, current_x, current_y);
                break;
            case 'q':
                printf("Now quitting\n");
                break;
            case 'f':
                break;
            default:
                printf("Bad input, commands are l, r, u, d, f, and q\n");
                break;
        }
    }

    free(world);
    free(currentMap);

    return 0;
}