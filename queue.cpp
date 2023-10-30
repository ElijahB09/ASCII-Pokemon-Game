#include "queue.h"
#include <iostream>

Queue::Queue(unsigned capacity) {
    this->capacity = capacity;
    this->front = this->size = 0;
    this->array = new terrainCell[this->capacity];
}

bool Queue::isFull() const {
    return (size == capacity);
}

bool Queue::isEmpty() const {
    return (size == 0);
}

void Queue::enqueue(const terrainCell& item) {
    if (isFull())
        return;
    rear = (rear + 1) % capacity;
    array[rear] = item;
    size = size + 1;
}

terrainCell Queue::dequeue() {
    if (isEmpty()) {
        std::cout << "Queue is empty" << std::endl;
    }

    terrainCell item = array[front];
    front = (front + 1) % capacity;
    size = size - 1;
    return item;
}
