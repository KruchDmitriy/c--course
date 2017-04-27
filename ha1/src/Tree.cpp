#include <cstring>
#include "Tree.h"

Tree::Tree(__uint32_t key, char value, Tree *left, Tree *right) {
    this->key   = key;
    this->value = value;
    this->left  = left;
    this->right = right;
}

Tree::Tree(std::ifstream& in) {
    char leaf  = 0;
    char buf;

    key = 0;
    value = '0';

    in.read(&buf, 1);
    if (buf == leaf) {
        in.read(&value, 1);
        left  = nullptr;
        right = nullptr;
        return;
    }

    left  = new Tree(in);

    in.read(&buf, 1);
    right = new Tree(in);
}

Tree::Tree(Tree *left, Tree *right) {
    key   = left->key + right->key;
    this->left  = left;
    this->right = right;
}

Tree::~Tree() {
    if (left != nullptr) {
        delete left;
    }
    if (right != nullptr) {
        delete right;
    }
}

namespace {
    void write_code_table(Tree* tree, TableOfCodes* table, char* code, uchar depth) {
        if (tree->left == nullptr && tree->right == nullptr) {
            uchar symbol = static_cast<uchar>(tree->value);
            memcpy(table->codes[symbol], code, depth);
            table->len[symbol] = depth;
            return;
        }

        if (tree->left != nullptr) {
            code[depth] = 0;
            write_code_table(tree->left, table, code, depth + 1);
        }

        if (tree->right != nullptr) {
            code[depth] = 1;
            write_code_table(tree->right, table, code, depth + 1);
        }
    }

    void serialize_tree(Tree *root, std::ofstream& out) {
        char leaf  = 0;
        char left  = 1;
        char right = 2;

        if (root->left == nullptr && root->right == nullptr) {
            out.write(&leaf, 1);
            out.write(&root->value, 1);
        }

        if (root->left != nullptr) {
            out.write(&left, 1);
            serialize_tree(root->left, out);
        }

        if (root->right != nullptr) {
            out.write(&right, 1);
            serialize_tree(root->right, out);
        }
    }
}

void Tree::serialize(std::ofstream& out) {
    serialize_tree(this, out);
}

TableOfCodes* Tree::getCodes() {
    TableOfCodes* table = new TableOfCodes;
    char code[256];
    uchar depth = 0;

    if (left == nullptr && right == nullptr) {
        uchar symbol = static_cast<uchar>(value);
        table->codes[symbol][0] = 0;
        table->len[symbol] = 1;
    }

    if (left != nullptr) {
        code[depth] = 0;
        write_code_table(left, table, code, depth + 1);
    }

    if (right != nullptr) {
        code[depth] = 1;
        write_code_table(right, table, code, depth + 1);
    }

    return table;
}