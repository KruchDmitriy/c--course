#ifndef IMSTRING_H
#define IMSTRING_H

#include <string>
#include <string.h>
#include <memory>
#include <ostream>

class imstring {
public:
    imstring(const char* str)
        : left_cat(nullptr)
        , right_cat(nullptr)
        , buffer(std::make_shared<char>(str))
        , size_(strlen(str))
    {}

    imstring(const imstring&) = default;
    imstring(imstring&&) = default;

    friend std::ostream& operator <<(std::ostream& os, imstring& istr) {
        os << *istr.buffer;
        return os;
    }

    size_t size() {
        return size_;
    }

    char* begin() {

    }

    char* end() {

    }

private:
    std::shared_ptr<char> left_cat;
    std::shared_ptr<char> right_cat;
    std::shared_ptr<char> buffer;
    const size_t size_;
};

#endif // IMSTRING_H
