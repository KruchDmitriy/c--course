#include <string.h>
#include <utility>

#include "lazy_string.h"
namespace lazy {

lazy_string::lazy_string(size_t buf_size)
    : buf_(buf_size)
{}

lazy_string::lazy_string(const char *src)
    : lazy_string(strlen(src) + 1)
{
    memcpy(buf_.get_data(), src, buf_.get_size());
}

lazy_string::lazy_string(const lazy_string &src)
    : buf_(src.buf_)
{}

lazy_string::lazy_string(lazy_string &&src)
    : buf_(src.buf_)
{}

bool lazy_string::empty() const
{
    return buf_.get_size() == 1;
}

size_t lazy_string::get_size() const
{
    return buf_.get_size() - 1;
}

char lazy_string::get_at(size_t ix) const
{
    return buf_.get_data()[ix];
}

const char* lazy_string::c_str() const
{
    return buf_.get_data();
}

lazy_string& lazy_string::operator=(lazy_string src)
{
    std::swap(buf_, src.buf_);
    return *this;
}

void lazy_string::set_at(size_t ix, char value)
{
    shared_buffer changed_buf(buf_.get_size());
    memcpy(changed_buf.get_data(), buf_.get_data(), buf_.get_size());
    changed_buf.get_data()[ix] = value;
    buf_ = changed_buf;
}

lazy_string& lazy_string::operator+=(const lazy_string& str) {
    shared_buffer dst(buf_.get_size() + str.get_size());
    memcpy(dst.get_data(), buf_.get_data(), buf_.get_size() - 1);
    memcpy(dst.get_data() + buf_.get_size(), str.c_str(), str.get_size() + 1);
    buf_ = dst;

    return *this;
}

size_t find(const lazy_string& in, const char* what,
            size_t start_ix)
{
    const char* substr = strstr(in.c_str() + start_ix, what);

    if (substr == nullptr) {
        return in.npos;
    }

    return substr - in.c_str() + 1;
}

lazy_string operator+(lazy_string str1, const lazy_string& str2)
{
    return (str1 += str2);
}

bool operator <(const lazy_string& str1, const lazy_string& str2)
{
    return strcmp(str1.c_str(), str2.c_str()) < 0;
}

std::ostream& operator<<(std::ostream& os, const lazy_string& str)
{
    os << str.c_str();
    return os;
}

} //namespace lazy
