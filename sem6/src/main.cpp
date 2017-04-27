#include <iostream>

#include "buffer.h"
#include "lazy.h"

int main() {
    shared_buffer str(5);
    shared_buffer str2(str);

    str = str2;

    return 0;
}
