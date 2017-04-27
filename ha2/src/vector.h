#pragma once

#include "memory.h"

template <typename T>
class vector {
public:
    explicit vector();
    explicit vector(size_t size);
    vector(const T data[], size_t size);
    vector(const vector& src);
    vector& operator=(vector src);
    vector(vector &&src);
    ~vector();

    T* get_data();
    vector& swap(vector& other);
    size_t size() const;
    T& operator[](size_t i);
    const T& operator[](size_t i) const;
    void push_back(const T& value);
    T& back();
    void pop_back();
private:
    T* data_;
    size_t size_;
    size_t act_size_;
};

template <typename T>
vector<T>::vector() {
    data_ = new T[1];
    size_ = 1;
    act_size_ = 0;
}

template <typename T>
vector<T>::vector(size_t size) {
    data_ = new T[size];
    size_ = size;
    act_size_ = 0;
}

template <typename T>
vector<T>::vector(const T data[], size_t size) {
    data_ = new T[size];
    size_ = size;
    memcpy(data_, data, sizeof(T) * size);
    act_size_ = size;
}

template <typename T>
vector<T>::vector(const vector& src) {
    data_ = new T[src.act_size_];
    memcpy(data_, src.data_, sizeof(T) * src.act_size_);
    size_ = src.act_size_;
    act_size_ = src.act_size_;
}

template <typename T>
vector<T>& vector<T>::swap(vector& other) {
    T* tmp_data = other.data_;
    size_t tmp_size = other.size_;
    size_t tmp_act_size = other.act_size_;

    other.data_ = data_;
    other.size_ = size_;
    other.act_size_ = act_size_;

    data_ = tmp_data;
    size_ = tmp_size;
    act_size_ = tmp_act_size;

    return *this;
}

template <typename T>
vector<T>::vector(vector &&src) {
    swap(src);
}

template <typename T>
vector<T>::~vector() {
    delete []data_;
}

template <typename T>
vector<T>& vector<T>::operator=(vector src) {
    swap(src);
    return *this;
}

template <typename T>
T* vector<T>::get_data() {
    return data_;
}

template <typename T>
size_t vector<T>::size() const {
    return act_size_;
}

template <typename T>
T& vector<T>::operator[](size_t i) {
    if (i >= 0 && i < act_size_) {
        return data_[i];
    } else {
        throw "Index vector out of bound";
    }
}

template <typename T>
const T& vector<T>::operator[](size_t i) const {
    if (i >= 0 && i < act_size_) {
        return data_[i];
    } else {
        throw "Index vector out of bound";
    }
}

template <typename T>
void vector<T>::push_back(const T& value) {
    if (act_size_ + 1 == size_) {
        T* new_data = new T[size_ * 2];
        size_ *= 2;
        memcpy(new_data, data_, sizeof(T) * act_size_);
        data_[act_size_] = value;
        act_size_++;
    } else {
        data_[act_size_] = value;
        act_size_++;
    }
}

template <typename T>
T& vector<T>::back() {
    if (act_size_ > 0) {
        return data_[act_size_ - 1];
    } else {
        throw "Vector is empty";
    }
}

template <typename T>
void vector<T>::pop_back() {
    if (act_size_ > 0) {
        act_size_--;
    } else {
        throw "Vector is empty";
    }
}