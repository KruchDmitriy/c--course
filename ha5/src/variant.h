#pragma once

#include <typeinfo>
#include <typeindex>
#include <unordered_set>
#include <vector>
#include <memory>
#include <algorithm>

namespace au {

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

    struct bad_get : std::exception {
        bad_get(const char* message)
            : message(message)
        {}

        bad_get(const std::string& message)
            : message(message)
        {}

        virtual ~bad_get() {}

        virtual const char* what() {
            return message.c_str();
        }

    protected:
        std::string message;
    };

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


    template <class ...Args>
    class alignas(get_max_alignof<Args...>())
    variant {
    public:
        variant()
            : cur_type(INVALID_TYPE)
            , cur_type_idx(INVALID_INDEX)
        {}

        template <class T>
        variant(T arg)
            : cur_type(INVALID_TYPE)
            , cur_type_idx(INVALID_INDEX)
        {
            const int same_type_idx = get_same_type<T, Args...>();
            if (same_type_idx != -1) {
                new (content) T(std::move(arg));
                cur_type = typeid(T);
                cur_type_idx = same_type_idx;
                return;
            }

            const int convert_type_idx = get_first_convertible_type<T, Args...>();
            cur_type_idx = convert_type_idx;
            if (convert_type_idx == -1) {
                throw std::exception();
            }

            using type = typename std::tuple_element<convert_type_idx, types>::type;
            new (content) type(std::move(arg));
            cur_type = typeid(type);
        }

        ~variant() {
            destroy<Args...>(cur_type_idx, content);
        }

        variant(const variant& other)
            : cur_type(other.cur_type)
            , cur_type_idx(other.cur_type_idx)
        {
            copy<Args...>(cur_type_idx, other.content, content);
        }

        variant(variant&& other)
            : cur_type(other.cur_type)
            , cur_type_idx(other.cur_type_idx)
        {
            move<Args...>(other.cur_type_idx, other.content, content);

            other.cur_type = INVALID_TYPE;
            other.cur_type_idx = INVALID_INDEX;
        }


        variant& operator =(const variant& other) {
            cur_type = other.cur_type;
            cur_type_idx = other.cur_type_idx;
            copy<Args...>(cur_type_idx, other.content, content);

            return *this;
        }

        variant& operator =(variant&& other) {
            cur_type = other.cur_type;
            cur_type_idx = other.cur_type_idx;
            move<Args...>(other.cur_type_idx, other.content, content);

            other.cur_type = INVALID_TYPE;
            other.cur_type_idx = INVALID_INDEX;

            return *this;
        }

        template <class T>
        variant& operator =(T arg)
        {
            variant tmp(std::move(arg));
            swap(*this, tmp);
            return *this;
        }

        void clear() {
            destroy<Args...>(cur_type_idx, content);
            cur_type = INVALID_TYPE;
            cur_type_idx = INVALID_INDEX;
        }


        const int INVALID_INDEX = -1;
        const std::type_index INVALID_TYPE = std::type_index(typeid(void));

        bool empty() const {
            return cur_type_idx == INVALID_INDEX;
        }

        int which() const {
            return cur_type_idx;
        }


        template <class T>
        friend T* get(variant* v) {
            if (!v->empty() && std::type_index(typeid(T)) == v->cur_type) {
                return reinterpret_cast<T*>(v->content);
            }

            return nullptr;
        }

        template <class T>
        friend const T* get(variant* v) {
            if (!v->empty() && std::type_index(typeid(T)) == v->cur_type) {
                return reinterpret_cast<T*>(v->content);
            }

            return nullptr;
        }

        template <class T>
        friend T& get(variant& v) {
            if (!v.empty() && std::type_index(typeid(T)) == v.cur_type) {
                return *reinterpret_cast<T*>(v.content);
            }

            throw bad_get(typeid(T).name());
        }

        template <class T>
        friend const T& get(const variant& v) {
            if (!v.empty() && std::type_index(typeid(T)) == v.cur_type) {
                return *reinterpret_cast<const T*>(v.content);
            }

            throw bad_get(typeid(T).name());
        }

        template <size_t idx, class ...A>
        friend auto get(      variant<A...>* v) ->       typename std::tuple_element<idx, std::tuple<A...>>::type*;

        template <size_t idx, class ...A>
        friend auto get(const variant<A...>* v) -> const typename std::tuple_element<idx, std::tuple<A...>>::type*;

        template <size_t idx, class ...A>
        friend auto get(      variant<A...>& v) ->       typename std::tuple_element<idx, std::tuple<A...>>::type&;

        template <size_t idx, class ...A>
        friend auto get(const variant<A...>& v) -> const typename std::tuple_element<idx, std::tuple<A...>>::type&;

        template <class ...A>
        friend void swap(variant<A...>& v1, variant<A...>& v2);

    private:
        static constexpr size_t max_size = get_max_sizeof<Args...>();
        using types = std::tuple<Args...>;
        std::type_index cur_type;
        int cur_type_idx;
        char content[max_size];
    };

    template <class ...A>
    void swap(variant<A...>& v1, variant<A...>& v2) {
        const size_t size = v1.max_size;

        char tmp[size];

        move<A...>(v1.cur_type_idx, v1.content, tmp);
//        std::copy(v1.content, v1.content + size, tmp);
        move<A...>(v2.cur_type_idx, v2.content, v1.content);
//        std::copy(v2.content, v2.content + size, v1.content);
        move<A...>(v1.cur_type_idx, tmp, v2.content);
//        std::copy(tmp, tmp + size, v2.content);

        std::swap(v1.cur_type, v2.cur_type);
        std::swap(v1.cur_type_idx, v2.cur_type_idx);
    }

    template <size_t idx, class ...Args>
    auto get(variant<Args...>* v)
        -> typename std::tuple_element<idx, std::tuple<Args...>>::type*
    {
        using type = typename std::tuple_element<idx, std::tuple<Args...>>::type;

        if (v->cur_type_idx == idx) {
            return reinterpret_cast<type*>(v->content);
        }

        return nullptr;
    }

    template <size_t idx, class ...Args>
    auto get(const variant<Args...>* v)
        -> const typename std::tuple_element<idx, std::tuple<Args...>>::type*
    {
        using type = typename std::tuple_element<idx, std::tuple<Args...>>::type;

        if (v->cur_type_idx == idx) {
            return reinterpret_cast<const type*>(v->content);
        }

        return nullptr;
    }

    template <size_t idx, class ...Args>
    auto get(variant<Args...>& v)
        -> typename std::tuple_element<idx, std::tuple<Args...>>::type&
    {
        using type = typename std::tuple_element<idx, std::tuple<Args...>>::type;

        if (v.cur_type_idx == idx) {
            return *reinterpret_cast<type*>(v.content);
        }

        throw bad_get(typeid(type).name());
    }

    template <size_t idx, class ...Args>
    auto get(const variant<Args...>& v)
        -> const typename std::tuple_element<idx, std::tuple<Args...>>::type&
    {
        using type = typename std::tuple_element<idx, std::tuple<Args...>>::type;

        if (v.cur_type_idx == idx) {
            return *reinterpret_cast<const type*>(v.content);
        }

        throw bad_get(typeid(type).name());
    }
}
