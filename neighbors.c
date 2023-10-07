#include "neighbors.h"
#define X_BOUND 80
#define Y_BOUND 21

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