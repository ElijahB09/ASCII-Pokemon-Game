#include <limits>
#include <cstdlib>
#include <iostream>
#include "dijkstra.h"
#define HEAP_SIZE 8192

void DijkstraTrainers(int x, int y, PokeMap *map, terrainCell start) {
    int numNeighbors, tempHikerDistance, tempRivalDistance;
    MinHeap* heapHiker;
    MinHeap* heapRival;
    terrainCell temp;
    terrainCell neighbors[8];

    for (int i = 0; i < y; i++) {
        for (int j = 0; j < x; j++) {
            map->arr[i][j].rival_total_distance = std::numeric_limits<int>::max();
            map->arr[i][j].hiker_total_distance = std::numeric_limits<int>::max();
            map->arr[i][j].rival_visited = 0;
            map->arr[i][j].hiker_visited = 0;
        }
    }
    map->arr[start.y_coord][start.x_coord].rival_total_distance = 0;
    map->arr[start.y_coord][start.x_coord].hiker_total_distance = 0;

    heapRival = new MinHeap(HEAP_SIZE);
    heapRival->insert(map->arr[start.y_coord][start.x_coord]);
    while (heapRival->getSize() != 0) {
        temp = heapRival->extractMin();
        map->arr[temp.y_coord][temp.x_coord].rival_visited = 1;
        numNeighbors = getNeighbors8Directions(x, y, map, temp, neighbors);
        for (int i = 0; i < numNeighbors; i++) {
            if (map->arr[neighbors[i].y_coord][neighbors[i].x_coord].rival_visited == 0) {
                if (neighbors[i].rival_distance != std::numeric_limits<int>::max()) {
                    tempRivalDistance = temp.rival_total_distance + temp.rival_distance;
                    if (tempRivalDistance < neighbors[i].rival_total_distance) {
                        map->arr[neighbors[i].y_coord][neighbors[i].x_coord].rival_total_distance = tempRivalDistance;
                        heapRival->insert(map->arr[neighbors[i].y_coord][neighbors[i].x_coord]);
                    }
                }
            }
        }
    }
    delete heapRival;

    heapHiker = new MinHeap(HEAP_SIZE);
    heapHiker->insert(map->arr[start.y_coord][start.x_coord]);
    while (heapHiker->getSize() != 0) {
        temp = heapHiker->extractMin();
        map->arr[temp.y_coord][temp.x_coord].hiker_visited = 1;
        numNeighbors = getNeighbors8Directions(x, y, map, temp, neighbors);
        for (int i = 0; i < numNeighbors; i++) {
            if (map->arr[neighbors[i].y_coord][neighbors[i].x_coord].hiker_visited == 0) {
                if (neighbors[i].hiker_distance != std::numeric_limits<int>::max()) {
                    tempHikerDistance = temp.hiker_total_distance + temp.hiker_distance;
                    if (tempHikerDistance < neighbors[i].hiker_total_distance) {
                        map->arr[neighbors[i].y_coord][neighbors[i].x_coord].hiker_total_distance = tempHikerDistance;
                        heapHiker->insert(map->arr[neighbors[i].y_coord][neighbors[i].x_coord]);
                    }
                }
            }
        }
    }
    delete heapHiker;
}

void Dijkstra(int x, int y, PokeMap *map, terrainCell start, terrainCell end) {
    int numNeighbors, tempDistance;
    MinHeap* heap;
    terrainCell temp;
    terrainCell neighbors[4];

    for (int i = 0; i < y; i++) {
        for (int j = 0; j < x; j++) {
            map->arr[i][j].distance = std::numeric_limits<int>::max();
            map->arr[i][j].visited = 0;
            map->arr[i][j].previous_x = -1;
            map->arr[i][j].previous_y = -1;
        }
    }
    map->arr[start.y_coord][start.x_coord].distance = 0;

    heap = new MinHeap(HEAP_SIZE);
    heap->insert(map->arr[start.y_coord][start.x_coord]);
    while (heap->getSize() != 0) {
        temp = heap->extractMin();
        if ((temp.x_coord == end.x_coord) && (temp.y_coord == end.y_coord)) {
            break;
        }
        map->arr[temp.y_coord][temp.x_coord].visited = 1;
        numNeighbors = getNeighbors(x, y, map, temp, neighbors);
        for (int i = 0; i < numNeighbors; i++) {
            if (map->arr[neighbors[i].y_coord][neighbors[i].x_coord].visited == 0) {
                tempDistance = temp.distance + neighbors[i].elevation;
                if (tempDistance < neighbors[i].distance) {
                    map->arr[neighbors[i].y_coord][neighbors[i].x_coord].distance = tempDistance;
                    map->arr[neighbors[i].y_coord][neighbors[i].x_coord].previous_x = temp.x_coord;
                    map->arr[neighbors[i].y_coord][neighbors[i].x_coord].previous_y = temp.y_coord;
                    heap->insert(map->arr[neighbors[i].y_coord][neighbors[i].x_coord]);
                }
            }
        }
    }
    temp = map->arr[end.y_coord][end.x_coord];
    while ((temp.x_coord != start.x_coord) || (temp.y_coord != start.y_coord)) {
        map->arr[temp.previous_y][temp.previous_x].elevation = 0;
        map->arr[temp.previous_y][temp.previous_x].terrainPiece = '#';
        map->arr[temp.previous_y][temp.previous_x].buildable = 0;
        map->arr[temp.previous_y][temp.previous_x].hiker_distance = 10;
        map->arr[temp.previous_y][temp.previous_x].rival_distance = 10;
        temp = map->arr[temp.previous_y][temp.previous_x];
    }
    map->arr[start.y_coord][start.x_coord].hiker_distance = std::numeric_limits<int>::max();
    map->arr[start.y_coord][start.x_coord].rival_distance = std::numeric_limits<int>::max();
    delete heap;
}
