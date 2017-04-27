#include <iostream>
#include <algorithm>
#include <assert.h>
#include <queue>

#define NDEBUG 1

template <class T, class RndIt, class Comp = std::less<T> >
int elem_num_in_sorted(RndIt begin, RndIt end, T def_val) {
    auto low = std::lower_bound(begin, end, def_val);
    auto up  = std::upper_bound(begin, end, def_val);
    int value = up - low;
    return value;
}

// assert(elem_num_in_sorted(sorted_ar, sorted_ar + ARRAY_SIZE(sorted_ar), 7) == 3);

template <class T, class Cont>
typename Cont::iterator set_add(Cont & cont, T value) {
    if (!std::binary_search(cont.begin(), cont.end(), value)) {
        auto it = std::lower_bound(cont.begin(), cont.end(), value);
        return cont.insert(it--, value);
    }
    return cont.end();
}

template<class Cont, class Pred>
Cont erase_if(Cont &cont, Pred pred) {
    // auto begin = cont.begin();
    // auto end = cont.end();
    // for (; begin!=end; ){
    //     if (pred(*begin)){
    //         std::remove_if(begin)
    //     }
    // }

    auto end  = std::remove_if(cont.begin(), cont.end(), pred);
    cont.erase(end, cont.end());
    return cont;
}

template<class Iter, class T>
void merge_sort(Iter begin, Iter end) {
    std::queue<std::vector<T>> queue;

    Iter begin_copy = begin;
    while(begin!=end){
        queue.push(std::vector<T>(1,begin++));
    }

    while(queue.size() != 1){
        auto first = queue.top();
        auto second = queue.top();

        queue.pop();
        queue.pop();

        std::vector<int> res;

        std::merge(first.begin(), first.end(), second.begin(), second.end(),
            std::back_inserter(res));

    }
}

int main() {
    int sorted_ar[] = { 1, 2, 3, 4, 5, 6, 7, 7, 7, 8, 9 , 10, 11};
    std::cout<<elem_num_in_sorted(sorted_ar, sorted_ar + 13,
                            7) << std::endl;


    // std::vector<int> set;// set_add(set, 10);
    // assert(*set_add(set, 10) == 10);
    // assert(set_add(set, 10) == set.end());
    // assert(*set_add(set, 5) == 5);
    // assert((set[0] == 5) && (set[1] == 10));

    std::vector<int> set = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };
    erase_if(set, [](int val) { return val > 6; });
    // set = { 1, 2, 3, 4, 5, 6 }
    for (auto item : set){
        std::cout<<item<<" ";
    }
    std::cout<<std::endl;

    return 0;
}
