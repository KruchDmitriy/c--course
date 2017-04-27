#include <cstring>

#include "huffman.h"

Buffer::Buffer(char *symbols, size_t num_bytes) {
    this->symbols = symbols;
    size_in_bytes = num_bytes;
    size = (num_bytes + 7) / 8;
}

Buffer::Buffer(std::ifstream& in) {
    in.read((char *)&size_in_bytes, sizeof(size_t));
    size = (size_in_bytes + 7) / 8;
    symbols = new char[size];
    in.read(symbols, size);
}

void Buffer::serialize(std::ofstream& out) {
    out.write(reinterpret_cast<char *>(&size_in_bytes), sizeof(size_t));
    out.write(symbols, size);
}

Buffer::~Buffer() {
    delete []symbols;
}

char* const Buffer::get_symbols() {
    return symbols;
}

size_t Buffer::get_size_in_bytes() {
    return size_in_bytes;
}

namespace {
    size_t count_freq(uchar* str, size_t size, __uint32_t * freq) {
        size_t non_zero = 0;
        for (uint i = 0; i < 256; i++) {
            freq[i] = 0;
            for (size_t j = 0; j < size; j++) {
                if (str[j] == i) {
                    freq[i]++;
                }
            }
            if (freq[i] != 0) {
                non_zero++;
            }
        }

        return non_zero;
    }

    size_t encode(TableOfCodes* table, char* str, size_t size, char*& result) {
        size_t result_size = 0;
        for (size_t i = 0; i < size; i++) {
            uchar cur_ch = static_cast<uchar>(str[i]);
            result_size += table->len[cur_ch];
        }
        result_size++;
        result = new char[result_size];

        for (size_t i = 0; i < result_size; i++) {
            result[i] = static_cast<char>(0xFF);
        }

        size_t res_idx = 0;
        uchar offset = 0;
        for (size_t i = 0; i < size; i++) {
            uchar cur_ch = static_cast<uchar>(str[i]);
            uchar len  = table->len[cur_ch];
            char* code = table->codes[cur_ch];

            for (int j = 0; j < len; ++j) {
                if (code[j] == 0) {
                    result[res_idx] &= ~(1 << (7 - offset));
                } else {
                    result[res_idx] |= (1 << (7 - offset));
                }

                offset = (offset + 1) % 8;
                if (offset == 0) {
                    res_idx++;
                }
            }
        }

        if (offset == 0) {
            result[res_idx] = '\0';
            return res_idx * 8;
        } else {
            result[res_idx + 1] = '\0';
            return res_idx * 8 + offset;
        }
    }
}

void huffman_encode(std::ifstream& in, std::ofstream& out) {
    typedef PriorityQueue::Node Node;

    in.seekg(0, in.end);
    size_t in_size = static_cast<size_t >(in.tellg());
    in.seekg(0, in.beg);

    if (in_size == 0) {
        return;
    }

    char* str = new char[in_size];
    in.read(str, in_size);


    __uint32_t freq[256];
    size_t count_nonzero = count_freq(reinterpret_cast<uchar*>(str), in_size, freq);

    Node* nodes = new Node[count_nonzero];
    uchar j = 0;
    for (uint i = 0; i < count_nonzero; i++) {
        while (freq[j] == 0) j++;

        char value = static_cast<char>(j);
        nodes[i] = new Tree(freq[j], value);
        j++;
    }


    PriorityQueue pq(nodes, count_nonzero);
    delete []nodes;

    while (pq.get_size() > 1) {
        Tree* left  = pq.extract_min();
        Tree* right = pq.extract_min();

        Tree* root = new Tree(left, right);
        pq.insert(root);
    }

    Tree* tree = pq.extract_min();

    TableOfCodes* table = tree->getCodes();

    char* result;
    size_t num_bytes = encode(table, str, in_size, result);

    tree->serialize(out);

    Buffer buffer(result, num_bytes);
    buffer.serialize(out);

    delete []str;
    delete table;
    delete tree;
}

void huffman_decode(std::ifstream& in, std::ofstream& out) {
    if (in.peek() == std::ifstream::traits_type::eof()) {
        return;
    }

    Tree*  tree = new Tree(in);
    Buffer buf(in);

    Tree*  cur_tree = tree;
    char*  code     = buf.get_symbols();
    char*  str      = new char[10];
    size_t str_idx  = 0;
    size_t str_size = 10;

    for (size_t i = 0; i < buf.get_size_in_bytes(); i++) {
        if (cur_tree->left != nullptr || cur_tree->right != nullptr) {
            // get i-th bit
            char byte = code[i / 8];
            char bit  = byte & ((char) 0x01 << (7 - i % 8));
            if (bit == 0) {
                cur_tree = cur_tree->left;
            } else {
                cur_tree = cur_tree->right;
            }
        }

        if (cur_tree->left == nullptr && cur_tree->right == nullptr) {
            str[str_idx] = cur_tree->value;
            str_idx++;
            cur_tree = tree;
            if (str_idx == str_size) {
                str_size *= 2;
                char *tmp = new char [str_size];
                memcpy(tmp, str, str_idx);
                delete []str;
                str = tmp;
            }
        }
    }

    out.write(str, str_idx);

    delete tree;
    delete []str;
}