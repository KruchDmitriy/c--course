#pragma once

#include "buffer.h"

struct lazy_string {
    lazy_string(const char *src);
    lazy_string(const lazy_string &src);

    bool empty() const;
    size_t get_size() const;
    char get_at(size_t ix) const;
private:
    friend void print(std::ostream& os, const lazy_string& str);
    friend lazy_string concat(const lazy_string &str1, const lazy_string &str2);
    shared_buffer buf_;
};

lazy_string::lazy_string(const char *src)
    : buf_(strlen(src) + 1) {
    char* dst = buf_.get_data();
    memcpy(dst, src, 1, buf_.get_size());
}

lazy_string::lazy_string(const lazy_string &src)
    : buf_(src.buf_) {}

bool lazy_string::empty const {
    return get_size() == 0;
}

size_t lazy_string::get_size() const {
    return buf_.get_size();
}

char get_at(size_t ix) const {
    return buf_.get_data()[ix];
}

void print(std::ostream& os, const lazy_string& str) {
    if (get_size() == 0) return 0;

    os << str.buf_.get_data() << std::endl;
}

lazy_string concat(const lazy_string &str1, const lazy_string &str2) {
    char* dst = new char[str1.size() + str2.size() - 1];
    memcpy(dst, str1.buf_.get_data(), 1, str1.get_size() - 1);
    memcpy(dst + str1.get_size(), str2.buf_.get_data(), 1, str2.get_size());

    return lazy_string(dst);
}
