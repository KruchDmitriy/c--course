#ifndef TREE_H
#define TREE_H

#include <fstream>

#define MAX_CHAR 256

typedef __uint8_t uchar;

typedef struct {
    char codes[MAX_CHAR][MAX_CHAR];
    uchar len[MAX_CHAR];
} TableOfCodes;

struct Tree {
    Tree(__uint32_t key = 0, char value = 0,
         Tree* left = nullptr,
         Tree* right = nullptr);
    Tree(std::ifstream& in);
    Tree(Tree* left, Tree* right);
    ~Tree();

    void serialize(std::ofstream& out);
    TableOfCodes* getCodes();

    __uint32_t key;
    char value;
    Tree* left;
    Tree* right;
};

#endif //TREE_H
