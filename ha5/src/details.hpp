#pragma once


namespace details {

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


    template <class Functor>
    constexpr void traverse(int, char*, char*) {}

    template <class Functor, class First, class ...Args>
    constexpr void traverse(int idx, char* src, char* dst) {
        if (idx == 0) {
            First* dst_ = reinterpret_cast<First*>(dst);
            First* src_ = reinterpret_cast<First*>(src);

            Functor f;
            f(src_, dst_);
        }

        traverse<Functor, Args...>(idx - 1, src, dst);
    }

    class Move {
    public:
        template <class T>
        void operator()(T* src, T* dst) {
            new (dst) T(std::move(*src));
        }
    };

    class Copy {
    public:
        template <class T>
        void operator()(const T* src, T* dst) {
            new (dst) T(*src);
        }
    };

    class Destroy {
    public:
        template <class T>
        void operator()(T* src, T* /* stub */) {
            src->~T();
        }
    };

    template <class ...Args>
    constexpr void move(int idx, char* src, char* dst) {
        traverse<Move, Args...>(idx, src, dst);
    }

    template <class ...Args>
    constexpr void copy(int idx, const char* src, char* dst) {
        traverse<Copy, Args...>(idx, const_cast<char *>(src), dst);
    }

    template <class ...Args>
    constexpr void destroy(int idx, char* data) {
        traverse<Destroy, Args...>(idx, data, nullptr);
    }

} // namespace details

