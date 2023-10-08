#include <limits.h>
#include <stdlib.h>
#include <stdio.h>
#include "dijkstra.h"
#include "neighbors.h"
#define HEAP_SIZE 8192

void DijkstraTrainers(int x, int y, terrainCell map[y][x], terrainCell start) {
    int i, j, numNeighbors, tempHikerDistance, tempRivalDistance;
    MinHeap *heapHiker, *heapRival;
    terrainCell temp;
    terrainCell neighbors[8];

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
        map[temp.previous_y][temp.previous_x].hiker_distance = 10;
        map[temp.previous_y][temp.previous_x].rival_distance = 10;
        temp = map[temp.previous_y][temp.previous_x];
    }
    map[start.y_coord][start.x_coord].hiker_distance = INT_MAX;
    map[start.y_coord][start.x_coord].rival_distance = INT_MAX;

    free(heap->arr);
    free(heap);
}