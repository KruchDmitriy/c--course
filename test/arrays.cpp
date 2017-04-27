#include <iostream>
#include <typeinfo>
#include <memory>

class Base
{
    int a;
};

class Derived : public Base
{
    int b;
};

int main() {
    using namespace std;

    auto pb = new Derived();

    cout << typeid(pb).name() << endl;

    return 0;
}
