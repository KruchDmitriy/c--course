#ifndef PRIORITY_QUEUE_H
#define PRIORITY_QUEUE_H

#include <cstdlib>

#include "Tree.h"

class PriorityQueue {
public:
    typedef Tree* Node;

    PriorityQueue();
    PriorityQueue(Node* const array, size_t size);
    ~PriorityQueue();

    Node extract_min();
    void insert(Node n);
    size_t get_size();
private:
    size_t size;
    size_t act_size;
    Node* nodes;

    void sift_up(size_t idx);
    void sift_down(size_t idx);
};

#endif //PRIORITY_QUEUE_H
