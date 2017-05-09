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


    template <class ...Args>
    class alignas(get_max_alignof<Args...>())
    variant {
    public:
        variant()
            : cur_type(typeid(void))
            , cur_type_idx(-1)
        {}

        template <class T>
        variant(T arg)
            : cur_type(typeid(void))
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

        variant(const variant& v)
            : cur_type(v.cur_type)
            , cur_type_idx(v.cur_type_idx)
        {
            std::copy(v.content, v.content + v.max_size, content);
        }

        variant(variant&& other)
            : variant()
        {
            swap(*this, other);
        }

        variant& operator =(variant other) {
            swap(*this, other);
            return *this;
        }

        template <class T>
        variant& operator =(T arg)
        {
            variant tmp(arg);
            swap(*this, tmp);
            return *this;
        }

        bool empty() const {
            return cur_type_idx == -1;
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
        friend auto get(variant<A...>* v) -> typename std::tuple_element<idx, std::tuple<A...>>::type*;

        template <size_t idx, class ...A>
        friend auto get(const variant<A...>* v) -> const typename std::tuple_element<idx, std::tuple<A...>>::type*;

        template <size_t idx, class ...A>
        friend auto get(variant<A...>& v) -> typename std::tuple_element<idx, std::tuple<A...>>::type&;

        template <size_t idx, class ...A>
        friend auto get(const variant<A...>& v) -> const typename std::tuple_element<idx, std::tuple<A...>>::type&;
    private:
        static constexpr size_t max_size = get_max_sizeof<Args...>();
        using types = std::tuple<Args...>;
        std::type_index cur_type;
        int cur_type_idx;
        char content[max_size];

        friend void swap(variant& v1, variant& v2) {
            const size_t size = v1.max_size;

            char tmp[size];

            std::copy(v1.content, v1.content + size, tmp);
            std::copy(v2.content, v2.content + size, v1.content);
            std::copy(tmp, tmp + size, v2.content);

            std::swap(v1.cur_type, v2.cur_type);
            std::swap(v1.cur_type_idx, v2.cur_type_idx);
        }
    };

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
