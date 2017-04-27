#include <cstdlib>
#include <memory.h>

#include "PriorityQueue.h"

PriorityQueue::PriorityQueue() {
    nodes = new Node[10];
    act_size = 10;
    size = 0;
}

PriorityQueue::PriorityQueue(Node* const array_, size_t size_) {
    nodes = new Node[size_];
    memcpy(nodes, array_, size_ * sizeof(Node));
    size = size_;
    act_size = size_;

    for (size_t i = size; i != 0; i--) {
        sift_down(i - 1);
    }
}

PriorityQueue::~PriorityQueue() {
    delete []nodes;
}

void PriorityQueue::sift_up(size_t i) {
    size_t son = i;
    size_t par = (i - 1) / 2;
    while ((son != 0) && (nodes[son]->key < nodes[par]->key)) {
        Node tmp = nodes[son];
        nodes[son] = nodes[par];
        nodes[par] = tmp;
        son = par;
        par = (par - 1) / 2;
    }
}

void PriorityQueue::sift_down(size_t i) {
    while (2 * i + 1 < size - 1) {
        size_t left = 2 * i + 1;
        size_t right = 2 * i + 2;
        size_t j = left;
        if (right < size - 1 && nodes[right]->key < nodes[left]->key) {
            j = right;
        }
        if (nodes[i]->key <= nodes[j]->key) {
            break;
        }

        Node tmp = nodes[i];
        nodes[i] = nodes[j];
        nodes[j] = tmp;

        i = j;
    }
}

PriorityQueue::Node PriorityQueue::extract_min() {
    Node min = nodes[0];
    nodes[0] = nodes[size - 1];
    size--;
    if (size != 0) {
        sift_down(0);
    }
    return min;
}

void PriorityQueue::insert(Node n) {
    if (size + 1 > act_size) {
        Node* new_mem = new Node[size * 2];
        memcpy(new_mem, nodes, size * sizeof(Node));
        act_size = size * 2;
    }
    size++;
    nodes[size - 1] = n;

    sift_up(size - 1);
}

size_t PriorityQueue::get_size() {
    return size;
}