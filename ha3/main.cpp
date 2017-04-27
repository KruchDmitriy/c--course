#include <iostream>
#include <cassert>
#include <memory>
#include "any.h"

using utils::any;
using utils::any_cast;
using utils::bad_any_cast;

using std::string;
using std::cout;
using std::cerr;
using std::endl;

void construct_test()
{
    any def;
    any copy_on_type(42);

    def = 3.14;
    def = string("2.71");
    any def_copy(def);

    def = copy_on_type;
    any e;

    assert(e.empty());
}

template<class T>
void check_cast(any& a, bool should_throw)
{
    bool thrown = false;

    try
    {
        T res = any_cast<T>(a);
        std::cout << res << endl;
    }

    catch (bad_any_cast const& err)
    {
        thrown = true;
        std::cerr << err.what() << std::endl;
    }

    assert(should_throw == thrown);
}

void retrieve_value_test()
{
    any ia(42);
    auto res = any_cast<double>(&ia);

    assert(res == nullptr);

    check_cast<double>(ia, true);
    check_cast<int> (ia, false);
}

void check_all_types()
{
    any i_any(42);

    int i   = any_cast<int>(i_any);
    int* pi = any_cast<int>(&i_any);

    assert(i == 42);
    assert(*pi == 42);

    int& ri  = any_cast<int&>(i_any);
    int& ri2 = any_cast<int&>(i_any);

    assert(ri == 42);
    assert(ri2 == 42);

    ri = 15;
    assert(ri2 == 15);

    const any ci_any(48);

    int ci         = any_cast<int>(ci_any);
    const int* cpi = any_cast<int>(&ci_any);
    const int& cri = any_cast<const int&>(ci_any);

    assert(ci   == 48);
    assert(*cpi == 48);
    assert(cri  == 48);

    /*
     * What is forbidden to do?
     */
    int a = 5;
    const int* pa = &a;
    any fbd_any(pa);
    bool thrown = false;
    try {
        any_cast<int>(fbd_any);
    }
    catch (bad_any_cast const& err) {
        thrown = true;
        std::cerr << err.what() << std::endl;
    }
    assert(thrown);

    int* fbd_pi = any_cast<int>(&fbd_any);
    assert(fbd_pi == nullptr);

    try {
        any_cast<int&>(fbd_any);
    }
    catch (bad_any_cast const& err) {
        thrown = true;
        std::cerr << err.what() << std::endl;
    }
    assert(thrown);

    /*
     * Also this code won't compile
     * const any const_any(5);
     * int& ref_i = any_cast<int&>(const_any);
     */
}

void swap_test(any& a, any& b)
{
    swap(a, b);
}

int main()
{
    using namespace std;

    construct_test();
    retrieve_value_test();

    any a(5), b(string("6"));
    swap_test(a, b);

    check_all_types();

    return 0;

}
