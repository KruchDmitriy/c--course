#ifndef SEM9_FIFO_H
#define SEM9_FIFO_H

#include <iostream>
#include <algorithm>
#include <memory>

namespace my_utils {
    template <class T>
    void fill(T* begin, T* end, const T& value) {
        for (T* it = begin; it != end; it++) {
            *it = value;
        }
    }

    template <class T, class Gen>
    void generate(T* begin, T* end, Gen gen) {
        for (T* it = begin; it != end; it++) {
            *it = gen();
        }
    }

    template <class T, class F>
    F for_each(T begin, T end, F func) {
        for (T it = begin; it != end; ++it) {
            func(*it);
        }
        return func;
    };

    template<class InputIt, class OutputIt>
    OutputIt copy(InputIt first, InputIt last, OutputIt d_first) {
        while (first != last) {
            *d_first++ = *first++;
        }
        return d_first;
    };

    template <class T>
    struct Deleter
    {
        void operator()(T* obj) {delete obj;}
    };
}

namespace fifo {
    template <class T>
    struct node {
        node *next;
        T value;
    };

    template <class T>
    struct queue {
        queue() {
            size_ = 0;
            first = nullptr;
            last = nullptr;
        }

        queue(const queue& q) {
            my_utils::copy(first, last, this);
        }

        ~queue() {
            // TODO!!!
        }

        void push(const T& value) {
            node<T>* node_ = new node<T>();
            node_->next  = nullptr;
            node_->value = value;

            if (last == nullptr) {
                first = node_;
                last = node_;
            } else {
                last->next  = node_;
                last = node_;
            }
            size_++;
        }

        void pop() {
            if (size_ == 0) {
                throw "fuck you queue is empty!";
            }

            node<T>* n = first;
            first = first->next;
            delete n;
            size_--;
        }

        T& front() {
            return first->value;
        }

        const T& front() const {
            return first->value;
        }

        size_t size() const {
            return size_;
        }

    private:
        node<T> *first;
        node<T> *last;
        size_t size_;
    };
}

#endif //SEM9_FIFO_H
