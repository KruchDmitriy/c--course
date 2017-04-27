#include <iostream>
#include <string>
#include <iterator>
#include "fifo.h"

int gen() {
    return 3;
}

int gen2(int x) {
    return x + 1;
}

int main() {
    std::cout << "Hello, World!" << std::endl;
    std::string strings[2];
    my_utils::fill(strings, &strings[2], std::string("hello"));
    for (int i = 0; i < 2; ++i) {
        std::cout << strings[i] << std::endl;
    }

    int array[2] = {};
    for (int i = 0; i < 2; ++i) {
        std::cout << array[i] << std::endl;
    }

    my_utils::for_each(array, &array[2], gen2);
    my_utils::copy(array, &array[2], std::ostream_iterator<int>(std::cout, "\n"));

    my_utils::generate(array, &array[2], gen);
    my_utils::copy(array, &array[2], std::ostream_iterator<int>(std::cout, "\n"));

    fifo::queue<int>* q = new fifo::queue<int>();
    q->push(2);
    q->push(3);
    q->push(4);
    for (int i = 0; i < 3; i++) {
        std::cout << q->front() << std::endl;
        q->pop();
    }
    delete q;
    return 0;
}