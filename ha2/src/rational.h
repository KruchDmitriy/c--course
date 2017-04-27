#pragma once

#include <string>
#include "big_int.h"

namespace apa {
    class rational {
    public:
        rational();
        explicit rational(uint32_t num, uint32_t den = 1);
        explicit rational(const std::string& num, const std::string& den = "1");

        rational(const rational& r);
        rational(rational&& r);
        rational& swap(rational& r);
        rational& operator = (rational r);

        explicit operator bool() const;

        rational& operator += (const rational& r);
        rational& operator *= (const rational& r);
        rational& operator /= (const rational& r);

        std::string str() const;
        friend std::ostream& operator<<(std::ostream& os, const rational& r);

        friend bool operator == (const rational& l, const rational& r);
        friend bool operator  > (const rational& l, const rational& r);
    private:
        explicit rational(const big_int& num, const big_int& den);
        big_int num_;
        big_int den_;
    };

    rational operator + (rational l, const rational& r);
    rational operator * (rational l, const rational& r);
    rational operator / (rational l, const rational& r);

    std::ostream& operator<<(std::ostream& os, const rational& r);
    std::istream& operator>>(std::istream& is, rational& r);

    bool operator == (const rational& l, const rational& r);
    bool operator != (const rational& l, const rational& r);
    bool operator >= (const rational& l, const rational& r);
    bool operator <= (const rational& l, const rational& r);
    bool operator  > (const rational& l, const rational& r);
    bool operator  < (const rational& l, const rational& r);
} // namespace apa
