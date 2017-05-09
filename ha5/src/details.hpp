#pragma once

template <class T>
constexpr int get_convertible_type(int) {
    return -1;
}

template <class T, class First, class ...Args>
constexpr int get_convertible_type(int n) {
    if (std::is_convertible<T, First>::value) {
        return n;
    }
    return get_convertible_type<T, Args...>(n + 1);
}

template <class T, class ...Args>
constexpr int get_first_convertible_type() {
    return get_convertible_type<T, Args...>(0);
}

template <class T>
constexpr int get_type(int) {
    return -1;
}

template <class T, class First, class ...Args>
constexpr int get_type(int n) {
    if (std::is_same<T, First>::value) {
        return n;
    }

    return  get_type<T, Args...>(n + 1);
}

template <class T, class ...Args>
constexpr int get_same_type() {
    return get_type<T, Args...>(0);
}


template <class ...Args>
constexpr int get_max_alignof() {
    return std::max({alignof(Args) ...});
}

template <class ...Args>
constexpr int get_max_sizeof() {
    return std::max({sizeof(Args) ...});
}

template <class First>
constexpr void copy(int idx, const char* src, char* dst) {
    if (idx == 0) {
        First* dst_ = reinterpret_cast<First*>(dst);
        const First* src_ = reinterpret_cast<const First*>(src);

        new (dst_) First(*src_);
    }
}

template <class First, class Second, class ...Args>
constexpr void copy(int idx, const char* src, char* dst) {
    if (idx == 0) {
        First* dst_ = reinterpret_cast<First*>(dst);
        const First* src_ = reinterpret_cast<const First*>(src);

        new (dst_) First(*src_);
        return;
    }

    copy<Second, Args...>(idx - 1, src, dst);
}

template <class First>
constexpr void move(int idx, char* src, char* dst) {
    if (idx == 0) {
        First* dst_ = reinterpret_cast<First*>(dst);
        First* src_ = reinterpret_cast<First*>(src);

        new (dst_) First(std::move(*src_));
    }
}

template <class First, class Second, class ...Args>
constexpr void move(int idx, char* src, char* dst) {
    if (idx == 0) {
        First* dst_ = reinterpret_cast<First*>(dst);
        const First* src_ = reinterpret_cast<First*>(src);

        new (dst_) First(std::move(*src_));
        return;
    }

    move<Second, Args...>(idx - 1, src, dst);
}

template <class First>
constexpr void destroy(int idx, char* data) {
    if (idx == 0) {
        First* ptr = reinterpret_cast<First*>(data);
        ptr->~First();
    }
}

template <class First, class Second, class ...Args>
constexpr void destroy(int idx, char* data) {
    if (idx == 0) {
        First* ptr = reinterpret_cast<First*>(data);
        ptr->~First();

        return;
    }

    destroy<Second, Args...>(idx - 1, data);
}
