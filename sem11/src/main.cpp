#include <iostream>
#include "smart_ptrs.h"

struct foo {
    int field1;
    int field2;
};

void bool_1st_arg_func(bool b) {
    return;
}

class A {
public:
    A() {
        bar();
    }
    void bar() {
        foo();
    }
    virtual void foo() = 0;
};

class B : public A {
public:
    void foo() {
        std::cout << "B" << std::endl;
    }
};

void bar(A& a) {
    a.foo();
}

int main() {
    B* ptr = new B();
    ptr->foo();

    A* ptr1 = reinterpret_cast<A*>(ptr);
    ptr1->foo();

    bar(*ptr);

    using namespace std;

    cout << "hello" << endl;

    scoped_ptr<foo> foo_ptr(new foo {0, 111});

    cout << (*foo_ptr).field2 << " "
         << foo_ptr->field2 << " "
         << foo_ptr.get() << std::endl;

    if (foo_ptr) {
        foo_ptr->field1 += 1;
    }
    cout << foo_ptr->field1 << endl;

    // foo_ptr = std::move(foo_ptr); // - compilation fails
    // auto foo_ptr2(foo_ptr);// - compilation fails
    // foo_ptr = foo_ptr; // - compilation fails
    // bool_1st_arg_func(foo_ptr); // - compilation fails
    // foo_ptr.reset(); //now foo_ptr->field1 fails in runtime
    // foo_ptr.reset(new foo {0, 111}); //now ok
    // scoped_ptr<foo> foo_ptr2; //now foo_ptr2->field1 fails in runtime
    // int a = foo_ptr2->field1;
    // a = a;

    return 0;
}
