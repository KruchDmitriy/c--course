#pragma once

#include "shared_buffer.h"
#include "vector.h"

namespace apa {
    struct wrong_format : public std::exception {
        virtual const char* what() const throw() {
            return "Wrong number representation";
        }
    };

    class big_int {
    public:
        explicit big_int();
        explicit big_int(uint32_t n);
        explicit big_int(const std::string& str);
        big_int(const big_int& bi);
        big_int(big_int&& bi);
        big_int& swap(big_int& bi);
        big_int& operator =  (big_int bi);
        big_int& operator += (const big_int& bi);
        big_int& operator *= (const big_int& bi);
        big_int& operator -= (const big_int& bi);
        big_int& operator /= (uint32_t n);
        big_int& operator /= (const big_int& bi);
        friend std::ostream& operator<<(std::ostream& os, const big_int& num);
        friend bool operator == (const big_int& bi, const big_int& bi2);
        friend bool operator >  (const big_int& bi, const big_int& bi2);
    private:
        size_t act_size() const;
        shared_buffer buf_;
    };

    std::ostream& operator<<(std::ostream& os, const big_int& bi);
    std::istream& operator>>(std::istream& is, big_int& bi);

    big_int operator + (big_int bi, const big_int& bi2);
    big_int operator * (big_int bi, const big_int& bi2);
    big_int operator - (big_int bi, const big_int& bi2);
    big_int operator / (big_int bi, uint32_t n);
    big_int operator / (big_int bi, const big_int& bi2);

    bool operator == (const big_int& bi, const big_int& bi2);
    bool operator != (const big_int& bi, const big_int& bi2);
    bool operator >= (const big_int& bi, const big_int& bi2);
    bool operator <= (const big_int& bi, const big_int& bi2);
    bool operator  > (const big_int& bi, const big_int& bi2);
    bool operator  < (const big_int& bi, const big_int& bi2);

    big_int gcd(const big_int& bi, const big_int& bi2);
} // namespace apa