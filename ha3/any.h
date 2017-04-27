#pragma once

#include <string>
#include <type_traits>
#include <typeinfo>
#include <stdexcept>
#include <iostream>
#include <memory>

namespace utils
{
   class any
    {
    public:
        any();
        any(const any& other);
        ~any();

        bool empty();
        any& swap(any& other);
        any& operator = (any other);

        template<class T>
        any(const T& t) : content(new holder<T>(t)) {}

        template<class T>
        friend T* any_cast(any* operand);

        template<class T>
        friend const T* any_cast(const any* operand);

        template<class T>
        friend T any_cast(any& operand);

    private:
        struct placeholder
        {
            virtual ~placeholder() {}
            virtual const std::type_info& type_info() const = 0;
            virtual placeholder* copy() = 0;
        };

        template<class T>
        struct holder : placeholder
        {
            holder(const T& held) : held(held) {}

            virtual const std::type_info& type_info() const {
                return typeid(T);
            }

            virtual placeholder* copy()
            {
                return new holder(held);
            }

            T held;
        };

        placeholder *content;
    };

    class bad_any_cast : public std::exception
    {
    public:
        bad_any_cast(const std::string& name_bad_type)
        {
            this->name_bad_type = "bad_any_cast: Failed conversion to ";
            this->name_bad_type += name_bad_type;
        }

        virtual const char* what() const throw()
        {
            return name_bad_type.c_str();
        }
    private:
        std::string name_bad_type;
    };

    template<class T>
    T* any_cast(any* operand)
    {
        if (operand->content->type_info() == typeid(T)) {
            return &static_cast<any::holder<T> *>(operand->content)->held;
        } else {
            return nullptr;
        }
    }

    template<class T>
    const T* any_cast(const any* operand)
    {
        if (operand->content->type_info() == typeid(T)) {
            return &static_cast<any::holder<T> *>(operand->content)->held;
        } else {
            return nullptr;
        }
    }

    template<class T>
    T any_cast(any& operand)
    {
        typedef typename std::remove_reference<T>::type base_type;
        base_type* obj = any_cast<base_type>(&operand);
        if (!obj)
        {
            throw bad_any_cast(typeid(T).name());
        }

        return *obj;
    }

    template<class T>
    const T any_cast(const any& operand)
    {
        typedef typename std::remove_reference<T>::type base_type;
        typedef typename std::remove_cv<base_type>::type base_without_const;
        const base_type* obj = any_cast<base_without_const>(&operand);
        if (!obj)
        {
            throw bad_any_cast(typeid(T).name());
        }

        return *obj;
    }

    void swap(any& left, any& right);
}
