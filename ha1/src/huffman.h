#ifndef HUFFMAN_H
#define HUFFMAN_H

#include <fstream>

#include "PriorityQueue.h"

struct Buffer {
public:
    Buffer(char* symbols = nullptr, size_t size_in_bytes = 0);
    Buffer(std::ifstream& in);
    ~Buffer();
    void serialize(std::ofstream& out);
    char* const get_symbols();
    size_t get_size_in_bytes();
private:
    char* symbols;
    size_t size;
    size_t size_in_bytes;
};

void huffman_encode(std::ifstream& in, std::ofstream& out);
void huffman_decode(std::ifstream& in, std::ofstream& out);

#endif //HUFFMAN_H