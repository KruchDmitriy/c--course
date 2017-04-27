#define CATCH_CONFIG_MAIN
#include "catch.hpp"
#include "serialize_args.hpp"
#include <iostream>

using namespace std;

//#undef TASK2
#undef TASK3
#undef TASK4

//int main() {
//    vector<char> second_bytes = store_args(1, 2, 3.14);
//    cout << second_bytes[0] << " "
//                            << second_bytes[1] << " " << endl;

//    return 0;
//}

namespace {
    template <class T>
    size_t my_tuple_size(T t)
    {
        return std::tuple_size<T>::value;
    }

    template<class T>
    bool check_max_value() {
        T value = std::numeric_limits<T>::max();
        auto data = store_args(value);
        auto args = restore_args<T>(data);
        return get<0>(args) == value;
    }

//    size_t func(int i, string s, double d)
//    {
//        (void)i;
//        (void)s;
//        (void)d;
//        return i + s.size() + static_cast<int>(d);
//    }
}

#ifdef TASK2

TEST_CASE("no args") {
    auto empty_bytes = store_args();
    auto empty_args = restore_args<>(empty_bytes);
    CHECK(my_tuple_size(empty_args) == 0);
}

TEST_CASE("one arg") {
    CHECK(check_max_value<int>());
    CHECK(check_max_value<double>());
    CHECK(check_max_value<float>());
    CHECK(check_max_value<char>());
    CHECK(check_max_value<short>());
}

TEST_CASE("many args") {
    auto data = store_args(1, 2, 3, 999, 20.3, 16, 74.32, 19l);
    auto args = restore_args<int, int, int, int, double, int, double, long>(data);
    CHECK(get<0>(args) == 1);
    CHECK(get<3>(args) == 999);
    CHECK(get<4>(args) == 20.3);
    CHECK(get<6>(args) == 74.32);
    CHECK(get<7>(args) == 19l);
}

TEST_CASE("one string") {
    auto input = string("Helloooooooooo woooorld ") + '\0' + "And null terminator";
    auto data = store_args(input);
    auto args = restore_args<string>(data);
    CHECK(input == get<0>(args));
}

TEST_CASE("combo") {
    string input = string("String between pod types ") + '\0' + "And null terminator";
    auto data = store_args(34, input, 9.09);
    auto args = restore_args<int, string, double>(data);
    CHECK(34 == get<0>(args));
    CHECK(input == get<1>(args));
    CHECK(9.09 == get<2>(args));
}

#endif // TASK2

#ifdef TASK3

TEST_CASE("type conversion") {
    auto data = save_args(func, 5, "hello", 9);
    auto args = restore_args<int, string, double>(data);
    CHECK(5 == get<0>(args));
    CHECK("hello" == get<1>(args));
    CHECK(9.0 == get<2>(args));
}

#endif // TASK3

#ifdef TASK4

TEST_CASE("calling func") {
    auto data = save_args(func, 3, "hello", 9.83);
    // function returns first + string length + last casted to int
    CHECK(17 == call_args(func, data));
}

#endif // TASK4
