#pragma once

struct shared_buffer {
    explicit shared_buffer(size_t size);
    shared_buffer(const shared_buffer &src);
    shared_buffer& operator=(shared_buffer src);
    ~shared_buffer();
    char* get_data();
    const char* get_data() const;
    size_t get_size() const;
private:
    char* data;
    size_t* count;
    size_t size;
    void swap(shared_buffer& buf);
};

void shared_buffer::swap(shared_buffer& buf) {
    shared_buffer tmp(buf);
    buf.data = data;
    buf.size = size;
    buf.count = count;

    data = tmp.data;
    size = tmp.size;
    count = tmp.count;
}

shared_buffer::shared_buffer(size_t size) {
    data = new char[size];
    this->size = size;
    count = new size_t(1);
}

shared_buffer::shared_buffer(const shared_buffer &src) {
    data = src.data;
    size = src.size;
    count = src.count;
    *count += 1;
}

shared_buffer& shared_buffer::operator=(shared_buffer src) {
    swap(src);
    return *this;
}

shared_buffer::~shared_buffer() {
    *count -= 1;
    if (*count == 0) {
        delete []data;
        delete count;
    }
}

char* shared_buffer::get_data() {
    return data;
}

const char* shared_buffer::get_data() const {
    return data;
}

size_t shared_buffer::get_size() const {
    return size;
}
