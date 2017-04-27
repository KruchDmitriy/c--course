#pragma once

#include <string>
#include <cstddef>
#include <stdint.h>

struct shared_buffer {
    explicit shared_buffer(size_t size);
    shared_buffer(const shared_buffer &src);
    shared_buffer& operator=(shared_buffer src);
    ~shared_buffer();

    uint32_t* get_data();
    const uint32_t* get_data() const;
    size_t get_size() const;

    void swap(shared_buffer& buf);
private:
    uint32_t* data_;
    size_t*   count_;
    size_t    size_;
};
