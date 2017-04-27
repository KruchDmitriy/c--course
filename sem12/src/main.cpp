#include <iostream>
#include "linq.h"

int main() {
    using namespace std;

    vector<int> v =
    { 1, 2, 3, 7, 8, 9 };

    copy(v.begin(), v.end(),
          ostream_iterator<int>(cout, "\n"));

    auto result_vector = from(v)
       .where([](int i){return i < 5;})
       .select<double>([](int i) {return sqrt((double)i);})
       .toVector();

    cout << endl;

    copy(result_vector.begin(), result_vector.end(),
          ostream_iterator<double>(cout, "\n"));

    cout << endl;

    cout << from(result_vector).count() << endl;

    return 0;
}
