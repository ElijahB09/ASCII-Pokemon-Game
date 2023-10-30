#include <cstdlib>
#include "buildings.h"

void buildPokeStuffFancy(int x, int y, PokeMap* map) {
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
                num_neighbors = getNeighbors(x, y, map, map->arr[valid_mart_y_coords[k]][rand_mart_x], neighbors);
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
                num_neighbors = getNeighbors(x, y, map, map->arr[valid_center_y_coords[k]][rand_center_x], neighbors);
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
                num_neighbors = getNeighbors(x, y, map, map->arr[rand_mart_y][valid_mart_x_coords[k]], neighbors);
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
                num_neighbors = getNeighbors(x, y, map, map->arr[rand_center_y][valid_center_x_coords[k]], neighbors);
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
