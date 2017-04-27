#pragma once

#include <functional>
#include <vector>
#include <algorithm>
#include <iterator>


template <class T>
class enumerable;

template <class T>
enumerable<T> from(std::vector<T> data)
{
    return enumerable<T>(data);
}

template <class T>
class enumerable
{
public:
    enumerable where(std::function <bool(T)> pred) const
    {
        std::vector<T> result;
        std::copy_if(data.begin(), data.end(),
            std::back_inserter(result), pred);
        return enumerable(result);
    }

    size_t count() const
    {
        return data.size();
    }

    size_t count(std::function<bool(T)> pred)
    {
        size_t cnt = 0;
        std::for_each(data.begin(), data.end(),
            [&cnt, pred](T t) -> void {
                if (pred(t)) cnt++;
            }
        );

        return cnt;
    }

    template <class V>
    const enumerable<V> select(std::function<V(T)> trans) const
    {
        std::vector<V> result;
        std::transform(data.begin(), data.end(),
            std::back_inserter(result), trans);
        return from<V>(result);
    }

    const std::vector<T>& toVector() const
    {
        return data;
    }

    template <class U>
    friend enumerable<U> from(std::vector<U> data);
private:
    enumerable(const std::vector<T>& data)
        : data(data) {}
    std::vector<T> data;
};
