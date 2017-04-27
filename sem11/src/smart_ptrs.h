#pragma once

template <class T>
class scoped_ptr {
public:
    scoped_ptr(T* ptr = nullptr)
        : ptr_(ptr)
    {}

    T* operator -> () {
        return ptr_;
    }

    explicit operator bool() {
        return ptr_ != nullptr;
    }

    T& operator *() {
        return *ptr_;
    }

    T* get() const {
        return ptr_;
    }

    void reset() {
        if (ptr_ != nullptr) {
            delete ptr_;
        }
        ptr_ = nullptr;
    }

    void reset(T* ptr) {
        if (ptr_ != nullptr) {
            delete ptr_;
        }
        ptr_ = ptr;
    }

    ~scoped_ptr() {
        delete ptr_;
    }
private:
    scoped_ptr(scoped_ptr& p);
    scoped_ptr(scoped_ptr&& p);
    scoped_ptr& operator = (const scoped_ptr& p);

    T* ptr_;
};
