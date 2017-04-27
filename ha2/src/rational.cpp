#include <sstream>

#include "rational.h"

namespace apa {

    rational::rational()
        : num_(0)
        , den_(1) {}

    rational::rational(uint32_t num, uint32_t den)
        : num_(num)
        , den_(den)
    {
        if (den == 0) {
            throw std::invalid_argument("Division by zero");
        }

        big_int gcd_ = gcd(num_, den_);
        num_ /= gcd_;
        den_ /= gcd_;
    }

    rational::rational(const std::string& num, const std::string& den)
        : num_(num)
        , den_(den)
    {
        if (den_ == big_int(0)) {
            throw std::invalid_argument("Division by zero");
        }

        big_int gcd_ = gcd(num_, den_);
        num_ /= gcd_;
        den_ /= gcd_;
    }

    rational::rational(const big_int& num, const big_int& den)
        : num_(num)
        , den_(den)
    {
        if (den_ == big_int(0)) {
            throw std::invalid_argument("Division by zero");
        }

        big_int gcd_ = gcd(num_, den_);
        num_ /= gcd_;
        den_ /= gcd_;
    }

    rational::rational(const rational& r)
        : num_(r.num_)
        , den_(r.den_) {}

    rational& rational::swap(rational& r) {
        num_.swap(r.num_);
        den_.swap(r.den_);
        return *this;
    }

    rational::rational(rational&& r)
        : num_(r.num_)
        , den_(r.den_) {}

    rational& rational::operator = (rational r) {
        swap(r);
        return *this;
    }

    rational& rational::operator += (const rational& r) {
        rational res = rational(num_ * r.den_ + r.num_ * den_, den_ * r.den_);
        big_int gcd_ = gcd(res.num_, res.den_);
        res.num_ /= gcd_;
        res.den_ /= gcd_;
        num_ = res.num_;
        den_ = res.den_;
        return *this;
    }

    rational& rational::operator *= (const rational& r) {
        big_int num = num_ * r.num_;
        big_int den = den_ * r.den_;
        big_int gcd_ = gcd(num, den);
        num /= gcd_;
        den /= gcd_;
        num_ = num;
        den_ = den;
        return *this;
    }

    rational& rational::operator /= (const rational& r) {
        if (r.num_ == big_int(0)) {
            throw std::invalid_argument("Division by zero");
        }

        big_int num = num_ * r.den_;
        big_int den = den_ * r.num_;
        big_int gcd_ = gcd(num, den);
        num /= gcd_;
        den /= gcd_;
        num_ = num;
        den_ = den;
        return *this;
    }

    rational operator + (rational l, const rational& r) {
        l += r;
        return l;
    }

    rational operator * (rational l, const rational& r) {
        l *= r;
        return l;
    }

    rational operator / (rational l, const rational& r) {
        l /= r;
        return l;
    }

    std::ostream& operator<<(std::ostream& os, const rational& r) {
        os << r.num_ << " / " << r.den_;
        return os;
    }

    std::istream& operator>>(std::istream& is, rational& r) {
        std::string num, delim, den;

        is >> num >> delim >> den;

        if (delim != "/") {
            throw wrong_format();
        }

        r = rational(num, den);

        return is;
    }

    std::string rational::str() const {
        std::stringstream ss;
        std::string res[3];
        ss << num_ << " / " << den_;
        ss >> res[0] >> res[1] >> res[2];
        return res[0] + " / " + res[2];
    }

    rational::operator bool() const {
        return num_ != big_int(0);
    }

    bool operator == (const rational& l, const rational& r) {
        return (l.num_ == r.num_) && (l.den_ == r.den_);
    }

    bool operator != (const rational& l, const rational& r) {
        return !(l == r);
    }

    bool operator > (const rational& l, const rational& r) {
        rational left;
        left.num_ = l.num_ * r.den_;
        left.den_ = l.den_ * r.den_;

        rational right;
        right.num_ = r.num_ * l.den_;
        right.den_ = r.den_ * l.den_;

        return (left.num_ > right.num_);
    }

    bool operator < (const rational& l, const rational& r) {
        return r > l;
    }

    bool operator >= (const rational& l, const rational& r) {
        return (l > r) || (l == r);
    }

    bool operator <= (const rational& l, const rational& r) {
        return (l < r) || (l == r);
    }

} // namespace apa
