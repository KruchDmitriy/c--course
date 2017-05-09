#include "imstring.h"
#include <string>
#include <cassert>
#include <cstring>
#include <iterator>
#include <algorithm>
#include <iostream>

template<class STR1, class STR2>
void test_strings_equal(STR1& str1, STR2 &str2)
{
    std::cout << str1 << std::endl << str2 << std::endl;
    assert(str1.size() == str2.size());

    assert(std::equal(str1.begin(), str1.end(), str2.begin()));
    assert(std::distance(str1.begin(), str1.end()) ==
            std::distance(str2.begin(), str2.end()));

    // call operator << before we force strings to be not lazy
    assert(strcmp(str1.c_str(), str2.c_str()) == 0);
}

void test() {
    std::list<int> list = {1, 1, 2, 3, 4};

    for (auto it = list.begin(); it != list.end(); ++it) {
        int cur_node = *it;
        if (cur_node != 0) {
            list.insert(it, cur_node - 1);
            list.insert(it, cur_node - 1);
            auto prev_it = it;
            --prev_it;
            --prev_it;
            --prev_it;
            list.erase(it);
            it = prev_it;
        }
    }

    std::copy(list.begin(), list.end(),
              std::ostream_iterator<int>(std::cout, " "));
    std::cout << std::endl;
}

int main()
{
    std::string str1("foo bar buzz");
    std::string str2("braz bar buzz");

    // Test creation from implicit c string
    imstring imstr1 = str1.c_str();
    auto it = imstr1.begin();
    auto end = imstr1.end();
    std::cout << std::distance(imstr1.begin(), imstr1.end()) << std::endl;

    test_strings_equal(str1, imstr1);
    test_strings_equal(imstr1, str1);

    imstring imstr2 = str2.c_str();
    imstring imstr_concat = imstr1 + imstr2;
    std::string str_concat = str1 + str2;
    test_strings_equal(str_concat, imstr_concat);

    // Test copying of shared immutable string buffer
    imstring imstr1_cp(imstr1);
    test_strings_equal(str1, imstr1);
    test_strings_equal(imstr1, imstr1_cp);
    assert(imstr1.begin() == imstr1.begin());
    assert(imstr1.end() == imstr1.end());
    // check laziness
    assert(&(*imstr1.begin()) == &(*imstr1_cp.begin()));

    // Test lazy concatenation
    imstring imstr_concat1 = imstr1 + imstr1_cp;
    imstring imstr_concat2 = imstr_concat1 + imstr_concat1 +
        imstr1 + imstr1_cp + imstr_concat1;
    std::string str_concat1 = str1 + str1;
    std::string str_concat2 = str_concat1 + str_concat1 +
        str1 + str1 + str_concat1;
    test_strings_equal(str_concat1, imstr_concat1);
    test_strings_equal(str_concat2, imstr_concat2);
    test_strings_equal(imstr_concat2, str_concat2);

    return 0;
}

