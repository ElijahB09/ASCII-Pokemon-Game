#include "minHeap.h"
#include <iostream>

MinHeap::MinHeap(unsigned capacity) {
    this->capacity = capacity;
    this->size = 0;
    this->array = new terrainCell[capacity];

    if (this->array == nullptr) {
        std::cerr << "Uh oh, computer broke" << std::endl;
    }
}

void MinHeap::insertHelper(int index) {
    int parent = (index - 1) / 2;

    if (array[parent].distance > array[index].distance) {
        terrainCell temp = array[parent];
        array[parent] = array[index];
        array[index] = temp;

        insertHelper(parent);
    }
}

void MinHeap::insert(const terrainCell& element) {
    if (size < capacity) {
        array[size] = element;
        insertHelper(size);
        size++;
    }
}

void MinHeap::minHeapify(int index) {
    int left = index * 2 + 1;
    int right = index * 2 + 2;
    int min = index;

    if (left >= size || left < 0)
        left = -1;
    if (right >= size || right < 0)
        right = -1;

    if (left != -1 && array[left].distance < array[min].distance)
        min = left;
    if (right != -1 && array[right].distance < array[min].distance)
        min = right;

    if (min != index) {
        terrainCell temp = array[min];
        array[min] = array[index];
        array[index] = temp;

        minHeapify(min);
    }
}

terrainCell MinHeap::extractMin() {
    terrainCell deleteItem;
    deleteItem.x_coord = deleteItem.y_coord = -1;

    if (size == 0) {
        std::cerr << "Heap is empty." << std::endl;
        return deleteItem;
    }

    deleteItem = array[0];
    array[0] = array[size - 1];
    size--;

    minHeapify(0);
    return deleteItem;
}

int MinHeap::getSize() {
    return this->size;
}
