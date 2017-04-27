#ifndef SERIALIZE_ARGS_HPP
#define SERIALIZE_ARGS_HPP

#include <vector>
#include <string>
#include <tuple>
#include <iterator>

std::vector<char> store_args() {
    return std::vector<char>();
}

template <class First, class ...Rest>
std::vector<char> store_args(First value, Rest... rest) {
    char* chars = (char *)(&value);
    std::vector<char> result;

    for (size_t i = 0; i < sizeof(value); i++) {
        result.push_back(chars[i]);
    }

    std::vector<char> rests = store_args(rest...);
    result.insert(result.end(), rests.begin(), rests.end());

    return result;
}

template <class ...Rest>
std::vector<char> store_args(std::string value, Rest... rest) {
    std::vector<char> result = store_args(value.size());

    for (unsigned int i = 0; i < value.size(); i++) {
        result.push_back(value[i]);
    }

    std::vector<char> rests = store_args(rest...);
    result.insert(result.end(), rests.begin(), rests.end());
    return result;
}

template <class ...Rest>
std::tuple<Rest...> restore_args(std::vector<char>& bytes);

template <class Last>
std::tuple<Last> restore_args_(std::vector<char>& bytes, size_t pos = 0) {
    char cur_bytes[sizeof(Last)];

    for (size_t i = 0; i < sizeof(Last); i++) {
        cur_bytes[i] = bytes[pos + i];
    }

    Last value(*(Last *)static_cast<void *>(cur_bytes));

    return std::tuple<Last>(value);
}

template <class First, class Second, class ...Rest>
std::tuple<First, Second, Rest...> restore_args_(std::vector<char>& bytes, size_t pos = 0) {
    char cur_bytes[sizeof(First)];

    for (size_t i = 0; i < sizeof(First); i++) {
        cur_bytes[i] = bytes[pos + i];
    }

    First value = *(First *)static_cast<void *>(cur_bytes);
    std::tuple<Second, Rest...> rest =
            restore_args_<Second, Rest ...>(bytes, pos + sizeof(First));

    return std::tuple_cat(std::make_tuple(value), rest);
}

template<class Second, class ...Rest>
std::tuple<std::string, Second, Rest...> restore_args_(std::vector<char> &bytes, size_t pos) {
    std::vector<char> new_bytes();
    new_bytes.insert(new_bytes.begin(), bytes.begin() + pos, bytes.end());

    std::tuple<size_t> size_ = restore_args<size_t>(new_bytes);
    size_t size = std::get<0>(size_);

    std::string str(size, '0');
    for (size_t i = 0; i < size; i++) {
        str[i] = bytes[i + pos + sizeof(size_t)];
    }

    std::tuple<Second, Rest...> rest =
            restore_args_<Second, Rest ...>(bytes, pos + sizeof(size_t) + size);

    return std::tuple_cat<size_t, Rest...>(std::make_tuple(str), rest);
}

template <class ...Rest>
std::tuple<Rest...> restore_args(std::vector<char>& bytes) {
    return restore_args_<Rest...>(bytes);
}

template<>
std::tuple<> restore_args(std::vector<char>& bytes) {
    return std::tuple<>();
}

#endif // SERIALIZE_ARGS_HPP
