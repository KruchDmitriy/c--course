#pragma once

#include <unordered_map>
#include <typeinfo>
#include <typeindex>
#include <functional>

template <class T>
class double_dispatcher {
public:
    using key_type = std::pair<std::type_index, std::type_index>;
    using func_type = std::function<void(T&, T&)>;

    template <class Arg1, class Arg2>
    static void reg(std::function<void(Arg1, Arg2)> func) {
        key_type pair_types = key_type(std::type_index(typeid(Arg1)),
                                       std::type_index(typeid(Arg2)));

        using real_type = std::function<void(Arg1, Arg2)>;
        overloaded_funcs[pair_types] = std::make_shared<real_type>((func));
    }

    static void call(T& arg1, T& arg2) {
        key_type key(std::type_index(typeid(arg1)), std::type_index(typeid(arg2)));
        std::shared_ptr<void> func = overloaded_funcs[key];
        if (func) {
            (*static_cast<func_type*>(func.get()))(arg1, arg2);
            return;
        }

        key = key_type(std::type_index(typeid(arg2)), std::type_index(typeid(arg1)));
        func = overloaded_funcs[key];
        if (func) {
            (*static_cast<func_type*>(func.get()))(arg1, arg2);
        }
    }

    struct hasher {
        size_t operator() (const key_type& key) const noexcept {
            return std::hash<std::type_index>()(key.first)
                    + std::hash<std::type_index>()(key.second);
        }
    };

private:
    static std::unordered_map<key_type, std::shared_ptr<void>, hasher> overloaded_funcs;
};

template <class T>
std::unordered_map<typename double_dispatcher<T>::key_type,
                   std::shared_ptr<void>,
                   typename double_dispatcher<T>::hasher>
double_dispatcher<T>::overloaded_funcs = {};
