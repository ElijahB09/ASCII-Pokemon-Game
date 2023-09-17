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

void buildPokeStuffFancy(int x, int y, PokeMap *map) {

    int coin_flip, rand_mart_x, rand_center_x, rand_mart_y, rand_center_y, i, j, k, m, n, road_start_x, road_end_x, road_start_y, road_end_y, started_x, ended_x, road_in_column, road_in_row, started_y, ended_y;
    int mart_placed, center_placed, num_neighbors, size;
    int valid_mart_y_coords[y];
    int valid_center_y_coords[y];
    int valid_mart_x_coords[x];
    int valid_center_x_coords[x];
    terrainCell neighbors[4];

    ended_x = started_x = ended_y = started_y = mart_placed = center_placed = 0;
    road_in_column = road_in_row = 0;
    coin_flip = rand() % 2;

    // Find where road pieces start and end on the x-axis
    for (i = 0; i < x; i++) {
        for (j = 0; j < y; j++) {
            if (map->arr[j][i].terrainPiece == '#' && started_x == 0) {
                road_start_x = i;
                started_x = 1;
                road_in_column = 1;
            }
            if (map->arr[j][i].terrainPiece == '#') {
                road_in_column = 1;
            }
        }
        if (ended_x == 0 && (started_x == 1 && (road_in_column == 0 || i == x - 1))) {
            road_end_x = i - 1;
            ended_x = 1;
        }
        road_in_column = 0;
    }

    // Find where road pieces start and end on the y-axis
    for (i = 0; i < y; i++) {
        for (j = 0; j < x; j++) {
            if (map->arr[i][j].terrainPiece == '#' && started_y == 0) {
                road_start_y = i;
                started_y = 1;
                road_in_row = 1;
            }
            if (map->arr[i][j].terrainPiece == '#') {
                road_in_row = 1;
            }
        }
        if (ended_y == 0 && (started_y == 1 && (road_in_row == 0 || i == x - 1))) {
            road_end_y = i - 1;
            ended_y = 1;
        }
        road_in_row = 0;
    }

    if (coin_flip == 0) {
        // use x axis for placement
        while (mart_placed == 0) {
            rand_mart_x = rand() % (road_end_x - road_start_x + 1) + road_start_x;

            k = 0;
            size = 0;
            for (i = 0; i < y; i++) {
                if (map->arr[i][rand_mart_x].terrainPiece == '#') {
                    valid_mart_y_coords[k] = i;
                    size++;
                    k++;
                }
            }

            k = 0;
            while (mart_placed == 0 && k < size) {
                num_neighbors = getNeighbors(x, y, map->arr, map->arr[valid_mart_y_coords[k]][rand_mart_x], neighbors);
                printf("%d\n\n", num_neighbors);
                for (i = 0; i < num_neighbors && mart_placed == 0; i++) {
                    if (map->arr[neighbors[i].y_coord][neighbors[i].x_coord].buildable == 1) {
                        map->arr[neighbors[i].y_coord][neighbors[i].x_coord].terrainPiece = 'M';
                        map->arr[neighbors[i].y_coord][neighbors[i].x_coord].buildable = 0;
                        mart_placed = 1;
//                    Below is some logic for later if I choose to implement it
//                    if (neighbors[i].y_coord > valid_mart_y_coords[k]) {
//                        map->arr[neighbors[i].y_coord][neighbors[i].x_coord].terrainPiece = 'M';
//                        map->arr[neighbors[i].y_coord][neighbors[i].x_coord].buildable = 0;
//                        mart_placed = 1;
//                    } else if (neighbors[i].y_coord < valid_mart_y_coords[k]) {
//                        map->arr[neighbors[i].y_coord][neighbors[i].x_coord].terrainPiece = 'M';
//                        map->arr[neighbors[i].y_coord][neighbors[i].x_coord].buildable = 0;
//                        mart_placed = 1;
//                    } else {
//                        map->arr[neighbors[i].y_coord][neighbors[i].x_coord].terrainPiece = 'M';
//                        map->arr[neighbors[i].y_coord][neighbors[i].x_coord].buildable = 0;
//                        mart_placed = 1;
//                    }
                    }
                }
                k++;
            }
        }
        while (center_placed == 0) {
            rand_center_x = rand() % (road_end_x - road_start_x + 1) + road_start_x;

            // Prevent building overlap
            while (rand_center_x == rand_mart_x || rand_center_x == rand_mart_x++ || rand_center_x == rand_mart_x-- || rand_center_x == rand_mart_x + 2 || rand_center_x == rand_mart_x - 2) {
                rand_center_x = rand() % (road_end_x - road_start_x + 1) + road_start_x;
            }

            k = 0;
            size = 0;
            for (i = 0; i < y; i++) {
                if (map->arr[i][rand_center_x].terrainPiece == '#') {
                    valid_center_y_coords[k] = i;
                    size++;
                    k++;
                }
            }

            k = 0;
            while (center_placed == 0 && k < size) {
                num_neighbors = getNeighbors(x, y, map->arr, map->arr[valid_center_y_coords[k]][rand_center_x], neighbors);
                for (i = 0; i < num_neighbors && center_placed == 0; i++) {
                    if (map->arr[neighbors[i].y_coord][neighbors[i].x_coord].buildable == 1) {
                        map->arr[neighbors[i].y_coord][neighbors[i].x_coord].terrainPiece = 'C';
                        map->arr[neighbors[i].y_coord][neighbors[i].x_coord].buildable = 0;
                        center_placed = 1;
                    }
                }
                k++;
            }
        }
    } else {
        // use y axis for placement
        while (mart_placed == 0) {
            rand_mart_y = rand() % (road_end_x - road_start_x + 1) + road_start_x;

            k = 0;
            size = 0;
            for (i = 0; i < x; i++) {
                if (map->arr[rand_mart_y][i].terrainPiece == '#') {
                    valid_mart_x_coords[k] = i;
                    size++;
                    k++;
                }
            }

            k = 0;
            while (mart_placed == 0 && k < size) {
                num_neighbors = getNeighbors(x, y, map->arr, map->arr[rand_mart_y][valid_mart_x_coords[k]], neighbors);
                for (i = 0; i < num_neighbors && mart_placed == 0; i++) {
                    if (map->arr[neighbors[i].y_coord][neighbors[i].x_coord].buildable == 1) {
                        map->arr[neighbors[i].y_coord][neighbors[i].x_coord].terrainPiece = 'M';
                        map->arr[neighbors[i].y_coord][neighbors[i].x_coord].buildable = 0;
                        mart_placed = 1;
                    }
                }
                k++;
            }
        }
        while (center_placed == 0) {
            rand_center_y = rand() % (road_end_x - road_start_x + 1) + road_start_x;

            // Prevent building overlap
            while (rand_center_y == rand_mart_y || rand_center_y == rand_mart_y++ || rand_center_y == rand_mart_y-- ||
                   rand_center_y == rand_mart_y + 2 || rand_center_y == rand_mart_y - 2) {
                rand_center_y = rand() % (road_end_x - road_start_x + 1) + road_start_x;
            }

            k = 0;
            size = 0;
            for (i = 0; i < x; i++) {
                if (map->arr[rand_center_y][i].terrainPiece == '#') {
                    valid_center_x_coords[k] = i;
                    size++;
                    k++;
                }
            }

            k = 0;
            while (center_placed == 0 && k < size) {
                num_neighbors = getNeighbors(x, y, map->arr, map->arr[rand_center_y][valid_center_x_coords[k]],
                                             neighbors);
                for (i = 0; i < num_neighbors && center_placed == 0; i++) {
                    if (map->arr[neighbors[i].y_coord][neighbors[i].x_coord].buildable == 1) {
                        map->arr[neighbors[i].y_coord][neighbors[i].x_coord].terrainPiece = 'C';
                        map->arr[neighbors[i].y_coord][neighbors[i].x_coord].buildable = 0;
                        center_placed = 1;
                    }
                }
                k++;
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
    temp = map[end.y_coord][end.x_coord];
    while ((temp.x_coord != start.x_coord) || (temp.y_coord != start.y_coord)) {
        map[temp.previous_y][temp.previous_x].elevation = 0;
        map[temp.previous_y][temp.previous_x].terrainPiece = '#';
        map[temp.previous_y][temp.previous_x].buildable = 0;
        temp = map[temp.previous_y][temp.previous_x];
    }

    free(heap->arr);
    free(heap);
}

PokeMap* generateMap(int x, int y, PokeMap *map, PokeMap (*world)[401], int map_x, int map_y) {
    if (world[map_y][map_x].is_created == 1) {
        printf("Input x: %d, input y: %d", map_x, map_y);
        printf("World x: %d, World y: %d\n\n", world[map_y][map_x].world_x, world[map_y][map_x].world_y);
        return &world[map_y][map_x];
    }
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
                map->arr[i][j].buildable = 0;
            } else {
                for (k = 0; k < NUM_RAN_COORDS; k++) {
                    // Need to randomize terrain, there are :, ^, %, ., and ~
                    // Current plan, Y_BOUND - 1 coords, 5 terrain types, many seed coords hopefully generates interesting terrain
                    if (j == rand_x_coords[k] && i == rand_y_coords[k]) {
                        switch (k % 6) { // Should go between 0 - 5
                            case 0:
                                map->arr[i][j].terrainPiece = ':';
                                map->arr[i][j].elevation = 2;
                                map->arr[i][j].buildable = 1;
                                randomCells[k] = map->arr[i][j];
                                break;
                            case 1:
                                map->arr[i][j].terrainPiece = '^';
                                map->arr[i][j].elevation = 3;
                                map->arr[i][j].buildable = 1;
                                randomCells[k] = map->arr[i][j];
                                break;
                            case 2:
                            case 3:
                                map->arr[i][j].terrainPiece = '.';
                                map->arr[i][j].elevation = 1;
                                map->arr[i][j].buildable = 1;
                                randomCells[k] = map->arr[i][j];
                                break;
                            case 4:
                                map->arr[i][j].terrainPiece = '~';
                                map->arr[i][j].elevation = 3;
                                map->arr[i][j].buildable = 1;
                                randomCells[k] = map->arr[i][j];
                                break;
                            case 5:
                                map->arr[i][j].terrainPiece = '%';
                                map->arr[i][j].elevation = 4;
                                map->arr[i][j].buildable = 1;
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
                    map->arr[currentCellYCoord + (i - 1)][currentCellXCoord + (j - 1)].buildable = currentCell->buildable;
                    enqueue(seeding_queue, map->arr[currentCellYCoord + (i - 1)][currentCellXCoord + (j - 1)]);
                }
            }
        }
    }
    map->world_x = map_x;
    map->world_y = map_y;
    if (map_x == 400 && map_y == 400) {
        // lower right
        rand_path_left = (rand() % 18) + 1;
        rand_path_up = (rand() % 77) + 1;
        map->left_exit = rand_path_left;
        map->up_exit = rand_path_up;

        map->arr[rand_path_left][0].terrainPiece = '#';
        map->arr[rand_path_left][0].elevation = 0;
        map->arr[0][rand_path_up].terrainPiece = '#';
        map->arr[0][rand_path_up].elevation = 0;

        Dijkstra(X_BOUND, Y_BOUND, map->arr, map->arr[rand_path_left][0], map->arr[0][rand_path_up]);

    } else if (map_x == 400 && map_y == 0) {
        // Upper right
        rand_path_left = (rand() % 18) + 1;
        rand_path_down = (rand() % 77) + 1;
        map->left_exit = rand_path_left;
        map->down_exit = rand_path_down;

        map->arr[rand_path_left][0].terrainPiece = '#';
        map->arr[rand_path_left][0].elevation = 0;
        map->arr[Y_BOUND - 1][rand_path_down].terrainPiece = '#';
        map->arr[Y_BOUND - 1][rand_path_down].elevation = 0;

        Dijkstra(X_BOUND, Y_BOUND, map->arr, map->arr[rand_path_left][0], map->arr[Y_BOUND - 1][rand_path_down]);

    } else if (map_x == 0 && map_y == 400) {
        // Lower left
        rand_path_right = (rand() % 18) + 1;
        rand_path_up = (rand() % 77) + 1;
        map->right_exit = rand_path_right;
        map->up_exit = rand_path_up;

        map->arr[rand_path_right][X_BOUND - 1].terrainPiece = '#';
        map->arr[rand_path_right][X_BOUND - 1].elevation = 0;
        map->arr[0][rand_path_up].terrainPiece = '#';
        map->arr[0][rand_path_up].elevation = 0;

        Dijkstra(X_BOUND, Y_BOUND, map->arr, map->arr[0][rand_path_up], map->arr[rand_path_right][X_BOUND - 1]);

    } else if (map_x == 0 && map_y == 0) {
        // Upper left
        rand_path_right = (rand() % 18) + 1;
        rand_path_down = (rand() % 77) + 1;
        map->right_exit = rand_path_right;
        map->down_exit = rand_path_down;

        map->arr[rand_path_right][X_BOUND - 1].terrainPiece = '#';
        map->arr[rand_path_right][X_BOUND - 1].elevation = 0;
        map->arr[Y_BOUND - 1][rand_path_down].terrainPiece = '#';
        map->arr[Y_BOUND - 1][rand_path_down].elevation = 0;
        Dijkstra(X_BOUND, Y_BOUND, map->arr, map->arr[Y_BOUND - 1][rand_path_down], map->arr[rand_path_right][X_BOUND - 1]);
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
    }
    buildPokeStuffFancy(X_BOUND, Y_BOUND, map);
    map->is_created = 1;
    world[map_y][map_x] = *map;

    free(randomCells);
    free(currentCell);
    free(seeding_queue->array);
    free(seeding_queue);

    return &world[map_y][map_x];
}

int main(int argc, char *argv[]) {
    PokeMap (*world)[401] = malloc(sizeof (PokeMap[401][401]));
    int current_x, current_y, i, j, fly_x, fly_y;
    char userInput;

    srand(time(NULL));

    current_x = 200;
    current_y = 200;
    world[current_y][current_x] = *generateMap(X_BOUND, Y_BOUND, &world[current_y][current_x], world, current_x, current_y);

    userInput = 'x';
    fly_x = fly_y = -999;

    while (userInput != 'q') {
        for (i = 0; i < Y_BOUND; i++) {
            for (j = 0; j < X_BOUND; j++) {
                printf("%c", world[current_y][current_x].arr[i][j].terrainPiece);
            }
            printf("\n");
        }
        printf("You are currently at position (%d, %d). Input command:", world[current_y][current_x].world_x - 200, world[current_y][current_x].world_y - 200);

        if (scanf(" %c", &userInput) == 1) {
            switch (userInput) {
                case 'w':
                    current_x--;
                    world[current_y][current_x] = *generateMap(X_BOUND, Y_BOUND, &world[current_y][current_x], world,
                                                               current_x, current_y);
                    break;
                case 'e':
                    current_x++;
                    world[current_y][current_x] = *generateMap(X_BOUND, Y_BOUND, &world[current_y][current_x], world,
                                                               current_x, current_y);
                    break;
                case 'n':
                    current_y--;
                    world[current_y][current_x] = *generateMap(X_BOUND, Y_BOUND, &world[current_y][current_x], world,
                                                               current_x, current_y);
                    break;
                case 's':
                    current_y++;
                    world[current_y][current_x] = *generateMap(X_BOUND, Y_BOUND, &world[current_y][current_x], world,
                                                               current_x, current_y);
                    break;
                case 'q':
                    printf("Now quitting\n");
                    break;
                default:
                    printf("Bad input, commands are n, s, e, w, 'f x y', and q\n");
                    break;
            }
            if (userInput == 'f' && scanf("%d %d", &fly_x, &fly_y) == 2) {
                if ((-200 <= fly_x && fly_x < 201) && (-200 <= fly_y && fly_y < 201)) {
                    current_x = fly_x + 200;
                    current_y = fly_y + 200;
                    world[current_y][current_x] = *generateMap(X_BOUND, Y_BOUND, &world[current_y][current_x], world, current_x, current_y);
                } else {
                    printf("Invalid coordinates, must be between -200 and 200 inclusive for both x and y");
                }
            }
        } else {
            printf("Bad input, commands are n, s, e, w, 'f x y', and q\n");
        }
    }
    free(world);

    return 0;
}