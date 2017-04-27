#include <iostream>
#include <memory>
#include <vector>
#include <functional>

#define TEST3

#include "shapes.h"
#include "dispatcher.h"

struct B {
    virtual void foo() {}
};
struct C : public B {};
struct D : public B {};

void func(C& c, D& d) {
    std::cout << "I was called" << std::endl;
    return;
}

#ifdef TEST3
double_dispatcher<shape> dp;
#endif

int main(int argc, char *argv[]) {
    using namespace std;

    std::vector<std::shared_ptr<shape>> shapes = {
        std::make_shared<point>(0, 0),
        std::make_shared<rectangle>(0, 0, 0, 0),
        std::make_shared<circle>(0, 0, 0)
    };

#ifdef TEST3
    shape_init_intersect_dd();
#endif

    for(auto shpptr1 : shapes)
        for(auto shpptr2 : shapes)
            intersect(*shpptr1, *shpptr2);

    double_dispatcher<B> disp;
    auto f = std::function<void(C&, D&)>(func);

    disp.reg(f);
    C c;
    D d;
    B& b1 = d;
    B& b2 = c;
    disp.call(b1, b2);

    cout << "Hello World!" << endl;
    return 0;
}
