#include <iostream>
#include <fstream>
#include <string.h>

#include "huffman.h"

struct Args
{
    enum type { ENCODE = 0, DECODE = 1 };
    type type_coding;
    std::ifstream in;
    std::ofstream out;
};

bool parser(int argc, char *argv[], Args& args)
{
    if (argc < 6)
    {
        return false;
    }

    for (int i = 1; i < 6; i++)
    {
        if (strcmp(argv[i], "-c") == 0)
        {
            args.type_coding = Args::ENCODE;
        }
        else if (strcmp(argv[i], "-u") == 0)
        {
            args.type_coding = Args::DECODE;
        }
        else if (strcmp(argv[i], "-f") == 0 ||
                 strcmp(argv[i], "--file") == 0)
        {
            i++;
            args.in.open(argv[i], std::ios::binary | std::ios::in);
        }
        else if (strcmp(argv[i], "-o") == 0 ||
                 strcmp(argv[i], "--output") == 0)
        {
            i++;
            args.out.open(argv[i], std::ios::binary | std::ios::out);
        }
    }

    return true;
}

int main(int argc, char *argv[]) {
    using namespace std;

    Args args;
    if (!parser(argc, argv, args)) {
        cout << "Bad input" << endl;
        return 1;
    }

    if (args.type_coding == Args::ENCODE) {
        huffman_encode(args.in, args.out);
    } else {
        huffman_decode(args.in, args.out);
    }

    return 0;
}
