#include <memory.h>

#include "shared_buffer.h"

void shared_buffer::swap(shared_buffer& buf) {
    shared_buffer tmp(buf);
    buf.data_ = data_;
    buf.size_ = size_;
    buf.count_ = count_;

    data_ = tmp.data_;
    size_ = tmp.size_;
    count_ = tmp.count_;
}

shared_buffer::shared_buffer(size_t size) {
    data_ = new uint32_t[size];
    this->size_ = size;
    count_ = new size_t(1);
}

shared_buffer::shared_buffer(const shared_buffer &src) {
    data_ = src.data_;
    size_ = src.size_;
    count_ = src.count_;
    *count_ += 1;
}

shared_buffer& shared_buffer::operator=(shared_buffer src) {
    swap(src);
    return *this;
}

shared_buffer::~shared_buffer() {
    *count_ -= 1;
    if (*count_ == 0) {
        delete []data_;
        delete count_;
    }
}

uint32_t* shared_buffer::get_data() {
    return data_;
}

const uint32_t* shared_buffer::get_data() const {
    return data_;
}

size_t shared_buffer::get_size() const {
    return size_;
}