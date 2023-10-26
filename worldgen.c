#include <stdlib.h>
#include "worldgen.h"
#include <limits.h>
#include <stdio.h>

void initNPCS(PokeMap *map, int num_npcs, NPC *npcs[num_npcs]) {
    int i, j, rand_trainer, test_x, test_y;

    // No characters are on the map
    for (i = 0; i < Y_BOUND; i++) {
        for (j = 0; j < X_BOUND; j++) {
            map->arr[i][j].character_present = 0;
        }
    }

    // Generate npcs
    if (num_npcs == 1) {
        npcs[0] = malloc(sizeof (NPC));
        npcs[0]->symbol = 'r';
	npcs[0]->defeated = 0;
    } else if (num_npcs >= 2) {
        npcs[0] = malloc(sizeof (NPC));
        npcs[0]->symbol = 'r';
	npcs[0]->defeated = 0;

        npcs[1] = malloc(sizeof (NPC));
        npcs[1]->symbol = 'h';
	npcs[1]->defeated = 0;

        for (i = 2; i < num_npcs; i++) {
            rand_trainer = (rand() % 6);
            switch (rand_trainer) {
                case 0:
                    npcs[i] = malloc(sizeof (NPC));
                    npcs[i]->symbol = 'r';
                    break;
                case 1:
                    npcs[i] = malloc(sizeof (NPC));
                    npcs[i]->symbol = 'h';
                    break;
                case 2:
                    npcs[i] = malloc(sizeof (NPC));
                    npcs[i]->symbol = 'p';
                    break;
                case 3:
                    npcs[i] = malloc(sizeof (NPC));
                    npcs[i]->symbol = 'w';
                    break;
                case 4:
                    npcs[i] = malloc(sizeof (NPC));
                    npcs[i]->symbol = 's';
                    break;
                case 5:
                    npcs[i] = malloc(sizeof (NPC));
                    npcs[i]->symbol = 'e';
                    break;
            }
	    npcs[i]->defeated = 0;
        }
    }

    for (i = 0; i < num_npcs; i++) {
        test_x = (rand() % 77) + 1;
        test_y = (rand() % 18) + 1;

        if (npcs[i]->symbol == 'r' || npcs[i]->symbol == 'p' || npcs[i]->symbol == 'w' || npcs[i]->symbol == 'e' || npcs[i]->symbol == 's') {
            while (map->arr[test_y][test_x].character_present == 1 || map->arr[test_y][test_x].rival_distance == INT_MAX) {
                test_x = (rand() % 77) + 1;
                test_y = (rand() % 18) + 1;
            }
        }
        else if (npcs[i]->symbol == 'h') {
            while (map->arr[test_y][test_x].character_present == 1 || map->arr[test_y][test_x].hiker_distance == INT_MAX) {
                test_x = (rand() % 77) + 1;
                test_y = (rand() % 18) + 1;
            }
        }

        npcs[i]->x_coord = test_x; npcs[i]->y_coord = test_y;
        map->arr[test_y][test_x].character_present = 1;
        map->arr[test_y][test_x].npc = npcs[i];
    }
}

PokeMap* generateMap(PokeMap *map, PokeMap* world[401][401], int map_x, int map_y, int num_npcs, NPC *npcs[num_npcs]) {
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
    initNPCS(map, num_npcs, npcs);

    map->is_created = 1;
    world[map_y][map_x] = map;

    free(randomCells);
    free(currentCell);
    free(seeding_queue->array);
    free(seeding_queue);

    return world[map_y][map_x];
}
