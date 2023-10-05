#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <limits.h>
#include <string.h>
#include "queue.h"
#include "minHeap.h"
#define QUEUE_SIZE 2048
#define HEAP_SIZE 8192
#define NUM_RAN_COORDS 30
#define MAX_ELEVATION 9999
#define X_BOUND 80
#define Y_BOUND 21

typedef struct {
    int x_coord, y_coord;
    char symbol;
} PlayerCharacter;

typedef struct {
    int x_coord, y_coord;
    char symbol;
} Hiker;

typedef struct {
    int x_coord, y_coord;
    char symbol;
} Rival;

typedef struct {
    int x_coord, y_coord;
    char symbol;
} Wanderer;

typedef struct {
    int x_coord, y_coord;
    char symbol;
} Pacer;

typedef struct {
    int x_coord, y_coord;
    char symbol;
} Sentry;

typedef struct {
    int x_coord, y_coord;
    char symbol;
} Explorers;

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

int getNeighbors8Directions(int x, int y, terrainCell map[y][x], terrainCell cell, terrainCell* neighbors) {
    int numNeighbors;
    if (cell.x_coord == 0 && cell.y_coord == 0) {
        neighbors[0] = map[cell.y_coord][cell.x_coord + 1];
        neighbors[1] = map[cell.y_coord + 1][cell.x_coord];
        neighbors[2] = map[cell.y_coord + 1][cell.x_coord + 1];
        numNeighbors = 3;
    } else if (cell.x_coord == X_BOUND - 1 && cell.y_coord == 0) {
        neighbors[0] = map[cell.y_coord][cell.x_coord - 1];
        neighbors[1] = map[cell.y_coord + 1][cell.x_coord];
        neighbors[2] = map[cell.y_coord + 1][cell.x_coord - 1];
        numNeighbors = 3;
    } else if (cell.x_coord == 0 && cell.y_coord == Y_BOUND - 1) {
        neighbors[0] = map[cell.y_coord][cell.x_coord + 1];
        neighbors[1] = map[cell.y_coord - 1][cell.x_coord];
        neighbors[2] = map[cell.y_coord - 1][cell.x_coord + 1];
        numNeighbors = 3;
    } else if (cell.x_coord == X_BOUND - 1 && cell.y_coord == Y_BOUND - 1) {
        neighbors[0] = map[cell.y_coord][cell.x_coord - 1];
        neighbors[1] = map[cell.y_coord - 1][cell.x_coord];
        neighbors[2] = map[cell.y_coord - 1][cell.x_coord - 1];
        numNeighbors = 3;
    } else if (cell.x_coord == 0) {
        neighbors[0] = map[cell.y_coord][cell.x_coord + 1];
        neighbors[1] = map[cell.y_coord - 1][cell.x_coord];
        neighbors[2] = map[cell.y_coord + 1][cell.x_coord];
        neighbors[3] = map[cell.y_coord - 1][cell.x_coord + 1];
        neighbors[4] = map[cell.y_coord + 1][cell.x_coord + 1];
        numNeighbors = 5;
    } else if (cell.y_coord == 0) {
        neighbors[0] = map[cell.y_coord + 1][cell.x_coord];
        neighbors[1] = map[cell.y_coord][cell.x_coord + 1];
        neighbors[2] = map[cell.y_coord][cell.x_coord - 1];
        neighbors[3] = map[cell.y_coord + 1][cell.x_coord + 1];
        neighbors[4] = map[cell.y_coord + 1][cell.x_coord - 1];
        numNeighbors = 5;
    } else if (cell.x_coord == X_BOUND - 1) {
        neighbors[0] = map[cell.y_coord][cell.x_coord - 1];
        neighbors[1] = map[cell.y_coord - 1][cell.x_coord];
        neighbors[2] = map[cell.y_coord + 1][cell.x_coord];
        neighbors[3] = map[cell.y_coord - 1][cell.x_coord - 1];
        neighbors[4] = map[cell.y_coord + 1][cell.x_coord - 1];
        numNeighbors = 5;
    } else if (cell.y_coord == Y_BOUND - 1) {
        neighbors[0] = map[cell.y_coord - 1][cell.x_coord];
        neighbors[1] = map[cell.y_coord][cell.x_coord + 1];
        neighbors[2] = map[cell.y_coord][cell.x_coord - 1];
        neighbors[3] = map[cell.y_coord - 1][cell.x_coord + 1];
        neighbors[4] = map[cell.y_coord - 1][cell.x_coord - 1];
        numNeighbors = 5;
    } else {
        neighbors[0] = map[cell.y_coord][cell.x_coord + 1];
        neighbors[1] = map[cell.y_coord][cell.x_coord - 1];
        neighbors[2] = map[cell.y_coord + 1][cell.x_coord];
        neighbors[3] = map[cell.y_coord - 1][cell.x_coord];
        neighbors[4] = map[cell.y_coord + 1][cell.x_coord + 1];
        neighbors[5] = map[cell.y_coord + 1][cell.x_coord - 1];
        neighbors[6] = map[cell.y_coord - 1][cell.x_coord + 1];
        neighbors[7] = map[cell.y_coord - 1][cell.x_coord - 1];
        numNeighbors = 8;
    }
    return numNeighbors;
}

void DijkstraTrainers(int x, int y, terrainCell map[y][x], terrainCell start) {
    int i, j, numNeighbors, tempHikerDistance, tempRivalDistance;
    MinHeap *heapHiker, *heapRival;
    terrainCell temp;
    terrainCell neighbors[8];

    i = j = numNeighbors = tempHikerDistance = tempRivalDistance = 0;

    for (i = 0; i < y; i++) {
        for (j = 0; j < x; j++) {
            map[i][j].rival_total_distance = INT_MAX;
            map[i][j].hiker_total_distance = INT_MAX;
            map[i][j].rival_visited = 0;
            map[i][j].hiker_visited = 0;
        }
    }
    map[start.y_coord][start.x_coord].rival_total_distance = 0;
    map[start.y_coord][start.x_coord].hiker_total_distance = 0;

    heapRival = buildMinHeap(HEAP_SIZE);
    insert(heapRival, map[start.y_coord][start.x_coord]);
    while (heapRival->size != 0) {
        temp = extractMin(heapRival);
        map[temp.y_coord][temp.x_coord].rival_visited = 1;
        numNeighbors = getNeighbors8Directions(x, y, map, temp, neighbors);
        for (i = 0; i < numNeighbors; i++) {
            if (map[neighbors[i].y_coord][neighbors[i].x_coord].rival_visited == 0) {
                if (neighbors[i].rival_distance != INT_MAX) {
                    tempRivalDistance = temp.rival_total_distance + temp.rival_distance;
                    if (tempRivalDistance < neighbors[i].rival_total_distance) {
                        map[neighbors[i].y_coord][neighbors[i].x_coord].rival_total_distance = tempRivalDistance;
                        insert(heapRival, map[neighbors[i].y_coord][neighbors[i].x_coord]);
                    }
                }
            }
        }
    }
    free(heapRival->arr);
    free(heapRival);

    heapHiker = buildMinHeap(HEAP_SIZE);
    insert(heapHiker, map[start.y_coord][start.x_coord]);
    while (heapHiker->size != 0) {
        temp = extractMin(heapHiker);
        map[temp.y_coord][temp.x_coord].hiker_visited = 1;
        numNeighbors = getNeighbors8Directions(x, y, map, temp, neighbors);
        for (i = 0; i < numNeighbors; i++) {
            if (map[neighbors[i].y_coord][neighbors[i].x_coord].hiker_visited == 0) {
                if (neighbors[i].hiker_distance != INT_MAX) {
                    tempHikerDistance = temp.hiker_total_distance + temp.hiker_distance;
                    if (tempHikerDistance < neighbors[i].hiker_total_distance) {
                        map[neighbors[i].y_coord][neighbors[i].x_coord].hiker_total_distance = tempHikerDistance;
                        insert(heapHiker, map[neighbors[i].y_coord][neighbors[i].x_coord]);
                    }
                }
            }
        }
    }
    free(heapHiker->arr);
    free(heapHiker);
}

void buildPokeStuffFancy(int x, int y, PokeMap *map) {
    int coin_flip, rand_mart_x, rand_center_x, rand_mart_y, rand_center_y, i, j, k, road_start_x, road_end_x, road_start_y, road_end_y, started_x, ended_x, road_in_column, road_in_row, started_y, ended_y;
    int mart_placed, center_placed, num_neighbors, size, manhattan_distance;
    int valid_mart_y_coords[y];
    int valid_center_y_coords[y];
    int valid_mart_x_coords[x];
    int valid_center_x_coords[x];
    int chance_mart, chance_center;
    int chance_buildings;
    terrainCell neighbors[4];

    road_in_column = road_in_row = ended_x = started_x = ended_y = started_y = road_start_x = road_end_x = road_start_y = road_end_y = i = j = k = rand_center_x = rand_center_y = rand_mart_x = rand_mart_y = mart_placed = center_placed = 0;

    if (!(map->world_x == 200 && map->world_y == 200)) {
        manhattan_distance = abs(0 - map->world_x) + abs(0 - map->world_y);
        chance_buildings = (((manhattan_distance * -45) / 200) + 40) / -1;
        chance_mart = rand() % 100;
        if (chance_mart > chance_buildings) {
            mart_placed = 1;
        }
        chance_center = rand() % 100;
        if (chance_center > chance_buildings) {
            center_placed = 1;
        }
    }

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
        if (ended_y == 0 && (started_y == 1 && (road_in_row == 0 || i == y - 1))) {
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
                for (i = 0; i < num_neighbors && mart_placed == 0; i++) {
                    if (map->arr[neighbors[i].y_coord][neighbors[i].x_coord].buildable == 1) {
                        map->arr[neighbors[i].y_coord][neighbors[i].x_coord].terrainPiece = 'M';
                        map->arr[neighbors[i].y_coord][neighbors[i].x_coord].buildable = 0;
                        map->arr[neighbors[i].y_coord][neighbors[i].x_coord].hiker_distance = map->arr[neighbors[i].y_coord][neighbors[i].x_coord].rival_distance = 50;
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
            while (rand_center_x == rand_mart_x || rand_center_x == rand_mart_x + 1 || rand_center_x == rand_mart_x - 1 || rand_center_x == rand_mart_x + 2 || rand_center_x == rand_mart_x - 2) {
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
                        map->arr[neighbors[i].y_coord][neighbors[i].x_coord].hiker_distance = map->arr[neighbors[i].y_coord][neighbors[i].x_coord].rival_distance = 50;
                        center_placed = 1;
                    }
                }
                k++;
            }
        }
    } else {
        // use y axis for placement
        while (mart_placed == 0) {
            rand_mart_y = rand() % (road_end_y - road_start_y + 1) + road_start_y;

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
                        map->arr[neighbors[i].y_coord][neighbors[i].x_coord].hiker_distance = map->arr[neighbors[i].y_coord][neighbors[i].x_coord].rival_distance = 50;
                        mart_placed = 1;
                    }
                }
                k++;
            }
        }
        while (center_placed == 0) {
            rand_center_y = rand() % (road_end_y - road_start_y + 1) + road_start_y;

            // Prevent building overlap
            while (rand_center_y == rand_mart_y || rand_center_y == rand_mart_y + 1 || rand_center_y == rand_mart_y - 1 || rand_center_y == rand_mart_y + 2 || rand_center_y == rand_mart_y - 2) {
                rand_center_y = rand() % (road_end_y - road_start_y + 1) + road_start_y;
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
                num_neighbors = getNeighbors(x, y, map->arr, map->arr[rand_center_y][valid_center_x_coords[k]], neighbors);
                for (i = 0; i < num_neighbors && center_placed == 0; i++) {
                    if (map->arr[neighbors[i].y_coord][neighbors[i].x_coord].buildable == 1) {
                        map->arr[neighbors[i].y_coord][neighbors[i].x_coord].terrainPiece = 'C';
                        map->arr[neighbors[i].y_coord][neighbors[i].x_coord].buildable = 0;
                        map->arr[neighbors[i].y_coord][neighbors[i].x_coord].hiker_distance = map->arr[neighbors[i].y_coord][neighbors[i].x_coord].rival_distance = 50;
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
            map[i][j].distance = INT_MAX;
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
        if (map[temp.previous_y][temp.previous_x].x_coord != start.x_coord && map[temp.previous_y][temp.previous_x].y_coord != start.y_coord) {
            map[temp.previous_y][temp.previous_x].hiker_distance = map[temp.previous_y][temp.previous_x].rival_distance = 10;
        }
        temp = map[temp.previous_y][temp.previous_x];
    }

    free(heap->arr);
    free(heap);
}

PokeMap* generateMap(PokeMap *map, PokeMap* world[401][401], int map_x, int map_y, PlayerCharacter *player) {
    if (world[map_y][map_x]) {
        return world[map_y][map_x];
    }

    map = (PokeMap *) malloc(sizeof (PokeMap));

    terrainCell (*randomCells) = malloc(sizeof (terrainCell[NUM_RAN_COORDS]));
    terrainCell (*currentCell) = malloc(sizeof (terrainCell));
    int currentCellXCoord, currentCellYCoord, i, j, k, rand_x_coords[NUM_RAN_COORDS], rand_y_coords[NUM_RAN_COORDS], rand_path_left, rand_path_right, rand_path_up, rand_path_down, rand_offset;
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
                map->arr[i][j].hiker_distance = INT_MAX;
                map->arr[i][j].rival_distance = INT_MAX;
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
                                map->arr[i][j].hiker_distance = 15;
                                map->arr[i][j].rival_distance = 20;
                                randomCells[k] = map->arr[i][j];
                                break;
                            case 1:
                                map->arr[i][j].terrainPiece = '^';
                                map->arr[i][j].elevation = 3;
                                map->arr[i][j].buildable = 1;
                                map->arr[i][j].hiker_distance = 15;
                                map->arr[i][j].rival_distance = INT_MAX;
                                randomCells[k] = map->arr[i][j];
                                break;
                            case 2:
                            case 3:
                                map->arr[i][j].terrainPiece = '.';
                                map->arr[i][j].elevation = 1;
                                map->arr[i][j].buildable = 1;
                                map->arr[i][j].hiker_distance = map->arr[i][j].rival_distance = 10;
                                randomCells[k] = map->arr[i][j];
                                break;
                            case 4:
                                map->arr[i][j].terrainPiece = '~';
                                map->arr[i][j].elevation = 3;
                                map->arr[i][j].buildable = 1;
                                map->arr[i][j].hiker_distance = INT_MAX;
                                map->arr[i][j].rival_distance = INT_MAX;
                                randomCells[k] = map->arr[i][j];
                                break;
                            case 5:
                                map->arr[i][j].terrainPiece = '%';
                                map->arr[i][j].elevation = 4;
                                map->arr[i][j].buildable = 1;
                                map->arr[i][j].hiker_distance = 15;
                                map->arr[i][j].rival_distance = INT_MAX;
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
                    map->arr[currentCellYCoord + (i - 1)][currentCellXCoord + (j - 1)].hiker_distance = currentCell->hiker_distance;
                    map->arr[currentCellYCoord + (i - 1)][currentCellXCoord + (j - 1)].rival_distance = currentCell->rival_distance;
                    enqueue(seeding_queue, map->arr[currentCellYCoord + (i - 1)][currentCellXCoord + (j - 1)]);
                }
            }
        }
    }
    map->world_x = map_x;
    map->world_y = map_y;
    if (map_x == 400 && map_y == 400) {
        // lower right
        if (world[map_y][map_x - 1]) {
            rand_path_left = world[map_y][map_x - 1]->right_exit;
        } else {
            rand_path_left = (rand() % 18) + 1;
        }
        if (world[map_y - 1][map_x]) {
            rand_path_up = world[map_y - 1][map_x]->down_exit;
        } else {
            rand_path_up = (rand() % 77) + 1;
        }
        map->left_exit = rand_path_left;
        map->up_exit = rand_path_up;

        map->arr[rand_path_left][0].terrainPiece = '#';
        map->arr[rand_path_left][0].elevation = 0;
        map->arr[0][rand_path_up].terrainPiece = '#';
        map->arr[0][rand_path_up].elevation = 0;

        Dijkstra(X_BOUND, Y_BOUND, map->arr, map->arr[rand_path_left][0], map->arr[0][rand_path_up]);

    } else if (map_x == 400 && map_y == 0) {
        // Upper right
        if (world[map_y][map_x - 1]) {
            rand_path_left = world[map_y][map_x - 1]->right_exit;
        } else {
            rand_path_left = (rand() % 18) + 1;
        }
        if (world[map_y + 1][map_x]) {
            rand_path_down = world[map_y + 1][map_x]->up_exit;
        } else {
            rand_path_down = (rand() % 77) + 1;
        }
        map->left_exit = rand_path_left;
        map->down_exit = rand_path_down;

        map->arr[rand_path_left][0].terrainPiece = '#';
        map->arr[rand_path_left][0].elevation = 0;
        map->arr[Y_BOUND - 1][rand_path_down].terrainPiece = '#';
        map->arr[Y_BOUND - 1][rand_path_down].elevation = 0;

        Dijkstra(X_BOUND, Y_BOUND, map->arr, map->arr[rand_path_left][0], map->arr[Y_BOUND - 1][rand_path_down]);

    } else if (map_x == 0 && map_y == 400) {
        // Lower left
        if (world[map_y][map_x + 1]) {
            rand_path_right = world[map_y][map_x + 1]->left_exit;
        } else {
            rand_path_right = (rand() % 18) + 1;
        }
        if (world[map_y - 1][map_x]) {
            rand_path_up = world[map_y - 1][map_x]->down_exit;
        } else {
            rand_path_up = (rand() % 77) + 1;
        }
        map->right_exit = rand_path_right;
        map->up_exit = rand_path_up;

        map->arr[rand_path_right][X_BOUND - 1].terrainPiece = '#';
        map->arr[rand_path_right][X_BOUND - 1].elevation = 0;
        map->arr[0][rand_path_up].terrainPiece = '#';
        map->arr[0][rand_path_up].elevation = 0;

        Dijkstra(X_BOUND, Y_BOUND, map->arr, map->arr[0][rand_path_up], map->arr[rand_path_right][X_BOUND - 1]);

    } else if (map_x == 0 && map_y == 0) {
        // Upper left
        if (world[map_y][map_x + 1]) {
            rand_path_right = world[map_y][map_x + 1]->left_exit;
        } else {
            rand_path_right = (rand() % 18) + 1;
        }
        if (world[map_y + 1][map_x]) {
            rand_path_down = world[map_y + 1][map_x]->up_exit;
        } else {
            rand_path_down = (rand() % 77) + 1;
        }
        map->right_exit = rand_path_right;
        map->down_exit = rand_path_down;

        map->arr[rand_path_right][X_BOUND - 1].terrainPiece = '#';
        map->arr[rand_path_right][X_BOUND - 1].elevation = 0;
        map->arr[Y_BOUND - 1][rand_path_down].terrainPiece = '#';
        map->arr[Y_BOUND - 1][rand_path_down].elevation = 0;
        Dijkstra(X_BOUND, Y_BOUND, map->arr, map->arr[Y_BOUND - 1][rand_path_down], map->arr[rand_path_right][X_BOUND - 1]);
    } else if (map_x == 400) {
        if (world[map_y][map_x - 1]) {
            rand_path_left = world[map_y][map_x - 1]->right_exit;
        } else {
            rand_path_left = (rand() % 18) + 1;
        }
        if (world[map_y - 1][map_x]) {
            rand_path_up = world[map_y - 1][map_x]->down_exit;
        } else {
            rand_path_up = (rand() % 77) + 1;
        }
        if (world[map_y + 1][map_x]) {
            rand_path_down = world[map_y + 1][map_x]->up_exit;
        } else {
            rand_path_down = (rand() % 77) + 1;
        }
        map->left_exit = rand_path_left;
        map->up_exit = rand_path_up;
        map->down_exit = rand_path_down;

        map->arr[rand_path_left][0].terrainPiece = '#';
        map->arr[rand_path_left][0].elevation = 0;
        map->arr[0][rand_path_up].terrainPiece = '#';
        map->arr[0][rand_path_up].elevation = 0;
        map->arr[Y_BOUND - 1][rand_path_down].terrainPiece = '#';
        map->arr[Y_BOUND - 1][rand_path_down].elevation = 0;
        Dijkstra(X_BOUND, Y_BOUND, map->arr, map->arr[0][rand_path_up], map->arr[Y_BOUND - 1][rand_path_down]);

        rand_offset = (rand() % 18) + 1;
        for (i = 0; i < X_BOUND; i++) {
            if (map->arr[rand_offset][i].terrainPiece == '#') {
                Dijkstra(X_BOUND, Y_BOUND, map->arr, map->arr[rand_path_left][0], map->arr[rand_offset][i]);
                break;
            }
        }
    } else if (map_x == 0) {
        if (world[map_y][map_x + 1]) {
            rand_path_right = world[map_y][map_x + 1]->left_exit;
        } else {
            rand_path_right = (rand() % 18) + 1;
        }
        if (world[map_y - 1][map_x]) {
            rand_path_up = world[map_y - 1][map_x]->down_exit;
        } else {
            rand_path_up = (rand() % 77) + 1;
        }
        if (world[map_y + 1][map_x]) {
            rand_path_down = world[map_y + 1][map_x]->up_exit;
        } else {
            rand_path_down = (rand() % 77) + 1;
        }
        map->right_exit = rand_path_right;
        map->up_exit = rand_path_up;
        map->down_exit = rand_path_down;

        map->arr[rand_path_right][X_BOUND - 1].terrainPiece = '#';
        map->arr[rand_path_right][X_BOUND - 1].elevation = 0;
        map->arr[0][rand_path_up].terrainPiece = '#';
        map->arr[0][rand_path_up].elevation = 0;
        map->arr[Y_BOUND - 1][rand_path_down].terrainPiece = '#';
        map->arr[Y_BOUND - 1][rand_path_down].elevation = 0;
        Dijkstra(X_BOUND, Y_BOUND, map->arr, map->arr[0][rand_path_up], map->arr[Y_BOUND - 1][rand_path_down]);

        rand_offset = (rand() % 18) + 1;
        for (i = 0; i < X_BOUND; i++) {
            if (map->arr[rand_offset][i].terrainPiece == '#') {
                Dijkstra(X_BOUND, Y_BOUND, map->arr, map->arr[rand_path_right][X_BOUND - 1], map->arr[rand_offset][i]);
                break;
            }
        }

    } else if (map_y == 400) {
        if (world[map_y][map_x + 1]) {
            rand_path_right = world[map_y][map_x + 1]->left_exit;
        } else {
            rand_path_right = (rand() % 18) + 1;
        }
        if (world[map_y][map_x - 1]) {
            rand_path_left = world[map_y][map_x - 1]->right_exit;
        } else {
            rand_path_left = (rand() % 18) + 1;
        }
        if (world[map_y - 1][map_x]) {
            rand_path_up = world[map_y - 1][map_x]->down_exit;
        } else {
            rand_path_up = (rand() % 77) + 1;
        }
        map->left_exit = rand_path_left;
        map->right_exit = rand_path_right;
        map->up_exit = rand_path_up;

        map->arr[rand_path_left][0].terrainPiece = '#';
        map->arr[rand_path_left][0].elevation = 0;
        map->arr[rand_path_right][X_BOUND - 1].terrainPiece = '#';
        map->arr[rand_path_right][X_BOUND - 1].elevation = 0;
        map->arr[0][rand_path_up].terrainPiece = '#';
        map->arr[0][rand_path_up].elevation = 0;
        Dijkstra(X_BOUND, Y_BOUND, map->arr, map->arr[rand_path_left][0], map->arr[rand_path_right][X_BOUND - 1]);

        rand_offset = (rand() % 77) + 1;
        for (i = 0; i < Y_BOUND; i++) {
            if (map->arr[i][rand_offset].terrainPiece == '#') {
                Dijkstra(X_BOUND, Y_BOUND, map->arr, map->arr[0][rand_path_up], map->arr[i][rand_offset]);
                break;
            }
        }
    } else if (map_y == 0) {
        if (world[map_y][map_x + 1]) {
            rand_path_right = world[map_y][map_x + 1]->left_exit;
        } else {
            rand_path_right = (rand() % 18) + 1;
        }
        if (world[map_y][map_x - 1]) {
            rand_path_left = world[map_y][map_x - 1]->right_exit;
        } else {
            rand_path_left = (rand() % 18) + 1;
        }
        if (world[map_y + 1][map_x]) {
            rand_path_down = world[map_y + 1][map_x]->up_exit;
        } else {
            rand_path_down = (rand() % 77) + 1;
        }
        map->left_exit = rand_path_left;
        map->right_exit = rand_path_right;
        map->down_exit = rand_path_down;

        map->arr[rand_path_left][0].terrainPiece = '#';
        map->arr[rand_path_left][0].elevation = 0;
        map->arr[rand_path_right][X_BOUND - 1].terrainPiece = '#';
        map->arr[rand_path_right][X_BOUND - 1].elevation = 0;
        map->arr[Y_BOUND - 1][rand_path_down].terrainPiece = '#';
        map->arr[Y_BOUND - 1][rand_path_down].elevation = 0;
        Dijkstra(X_BOUND, Y_BOUND, map->arr, map->arr[rand_path_left][0], map->arr[rand_path_right][X_BOUND - 1]);

        rand_offset = (rand() % 77) + 1;
        for (i = 0; i < Y_BOUND; i++) {
            if (map->arr[i][rand_offset].terrainPiece == '#') {
                Dijkstra(X_BOUND, Y_BOUND, map->arr, map->arr[Y_BOUND - 1][rand_path_down], map->arr[i][rand_offset]);
                break;
            }
        }
    } else {
        if (world[map_y][map_x + 1]) {
            rand_path_right = world[map_y][map_x + 1]->left_exit;
        } else {
            rand_path_right = (rand() % 18) + 1;
        }
        if (world[map_y][map_x - 1]) {
            rand_path_left = world[map_y][map_x - 1]->right_exit;
        } else {
            rand_path_left = (rand() % 18) + 1;
        }
        if (world[map_y + 1][map_x]) {
            rand_path_down = world[map_y + 1][map_x]->up_exit;
        } else {
            rand_path_down = (rand() % 77) + 1;
        }
        if (world[map_y - 1][map_x]) {
            rand_path_up = world[map_y - 1][map_x]->down_exit;
        } else {
            rand_path_up = (rand() % 77) + 1;
        }

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

    int tempx, tempy, found;
    found = 0;
    for (i = 0; i < Y_BOUND; i++) {
        for (j = 0; j < X_BOUND; j++) {
            if (map->arr[i][j].terrainPiece == '#' && i > 0 && j > 0 && found == 0) {
                player->x_coord = j; player->y_coord = i;
                map->arr[i][j].present_character = &player->symbol;
                tempx = j;
                tempy = i;
                found = 1;
            }
        }
    }

    DijkstraTrainers(X_BOUND, Y_BOUND, map->arr, map->arr[tempy][tempx]);
    map->is_created = 1;
    world[map_y][map_x] = map;

    free(randomCells);
    free(currentCell);
    free(seeding_queue->array);
    free(seeding_queue);

    return world[map_y][map_x];
}

int main(int argc, char *argv[]) {
    PokeMap* world[401][401];
    int current_x, current_y, i, j, fly_x, fly_y, num_npcs;
    char userInput;
    num_npcs = 10;

    if (argc == 2) {
        if (strncmp(argv[1], "--numtrainers", 13) == 0) {
            char *num_npcs_str = argv[1] + 14;
            char *endptr;
            num_npcs = (int)strtol(num_npcs_str, &endptr, 10);

            if (*endptr != '\0' || num_npcs <= 0) {
                printf("ERROR: use --numtrainers=x where x >= 0, instead passed: %s\n", num_npcs_str);
                return 1;
            }
        }
    } else if (argc > 2) {
        printf("ERROR: usage ./<assignment_name> --numtrainers=x where x >= 0");
    }
    PlayerCharacter *player = malloc(sizeof (PlayerCharacter));
    player->symbol = '@';

    for (i = 0; i < 401; i++) {
        for (j = 0; j < 401; j++) {
            world[i][j] = NULL;
        }
    }

    srand(time(NULL));

    current_x = 200;
    current_y = 200;
    *world[current_y][current_x] = *generateMap(world[current_y][current_x], world, current_x, current_y, player);
    userInput = 'x';
    fly_x = fly_y = -999;

    while (userInput != 'q') {
        for (i = 0; i < Y_BOUND; i++) {
            for (j = 0; j < X_BOUND; j++) {
                if (world[current_y][current_x]->arr[i][j].present_character) {
                    printf("%c", *world[current_y][current_x]->arr[i][j].present_character);
                } else {
                    printf("%c", world[current_y][current_x]->arr[i][j].terrainPiece);
                }
            }
            printf("\n");
        }

        printf("You are currently at position (%d, %d). Input command:", world[current_y][current_x]->world_x - 200, world[current_y][current_x]->world_y - 200);

        if (scanf(" %c", &userInput) == 1) {
            printf("\n");
            switch (userInput) {
                case 'w':
                    current_x--;
                    if (current_x >= 0) {
                        *world[current_y][current_x] = *generateMap(world[current_y][current_x],world,current_x, current_y, player);
                    } else {
                        current_x++;
                        printf("Error cannot go beyond world limits\n");
                    }
                    break;
                case 'e':
                    current_x++;
                    if (current_x < 401) {
                        *world[current_y][current_x] = *generateMap(world[current_y][current_x],world,current_x, current_y, player);
                    } else {
                        current_x--;
                        printf("Error cannot go beyond world limits\n");
                    }
                    break;
                case 'n':
                    current_y--;
                    if (current_y >= 0) {
                        *world[current_y][current_x] = *generateMap(world[current_y][current_x],world,current_x, current_y, player);
                    } else {
                        current_y++;
                        printf("Error cannot go beyond world limits\n");
                    }
                    break;
                case 's':
                    current_y++;
                    if (current_y < 401) {
                        *world[current_y][current_x] = *generateMap(world[current_y][current_x],world,current_x, current_y, player);
                    } else {
                        current_y--;
                        printf("Error cannot go beyond world limits\n");
                    }
                    break;
                case 'q':
                    printf("Now quitting\n");
                    break;
                case 'f':
                    if (scanf("%d %d", &fly_x, &fly_y) == 2) {
                        if ((-200 <= fly_x && fly_x < 201) && (-200 <= fly_y && fly_y < 201)) {
                            current_x = fly_x + 200;
                            current_y = fly_y + 200;
                            *world[current_y][current_x] = *generateMap(world[current_y][current_x], world, current_x, current_y, player);
                        } else {
                            printf("Invalid coordinates, must be between -200 and 200 inclusive for both x and y");
                        }
                    }
                    break;
                default:
                    printf("Bad input, commands are n, s, e, w, 'f x y', and q\n");
                    break;
            }
        } else {
            printf("Bad input, commands are n, s, e, w, 'f x y', and q\n");
        }
    }
    for(i =0; i < 401; i++) {
        for(j =0; j < 401; j++) {
            if(world[i][j]){
                free(world[i][j]);
                world[i][j] = NULL;
            }
        }
    }
    free(player);

    return 0;
}