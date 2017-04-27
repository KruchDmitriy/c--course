#include "big_int.h"

#include <sstream>
#include <iostream>
#include <algorithm>
#include <iomanip>

#define BIGINT_BASE 1000000000 // 10^9
#define BIGINT_NUMBERS 9

namespace apa {
    big_int::big_int(uint32_t n)
        : buf_(2)
    {
        if (n >= BIGINT_BASE) {
            buf_.get_data()[0] = n % BIGINT_BASE;
            buf_.get_data()[1] = n / BIGINT_BASE;
        } else {
            buf_ = shared_buffer(1);
            buf_.get_data()[0] = n;
        }
    }

    big_int::big_int()
        : buf_(1) {
        big_int(1);
    }

    big_int::big_int(const std::string& str)
        : buf_((str.size() + BIGINT_NUMBERS - 1) / BIGINT_NUMBERS)
    {
        if (str.find_first_not_of("0123456789") != std::string::npos) {
            throw wrong_format();
        }

        size_t str_size = str.size();
        size_t num_size = (str_size + BIGINT_NUMBERS - 1) / BIGINT_NUMBERS;

        size_t tail_size = str_size % BIGINT_NUMBERS;
        int tail;
        if (tail_size > 0) {
            tail = 1;
        } else {
            tail = 0;
        }
        std::stringstream ss;
        for (size_t i = 0; i < num_size - tail; ++i) {
            ss.clear();
            ss << str.substr(str_size - (i + 1) * BIGINT_NUMBERS, BIGINT_NUMBERS);
            ss >> buf_.get_data()[i];
        }

        if (tail_size > 0) {
            ss.clear();
            ss << str.substr(0, tail_size);
            ss >> buf_.get_data()[num_size - 1];
        }
    }

    big_int::big_int(const big_int& bi)
        : buf_(bi.buf_) {}

    big_int::big_int(big_int&& bi)
        : buf_(bi.buf_) {}

    big_int& big_int::swap(big_int& bi) {
        buf_.swap(bi.buf_);
        return *this;
    }

    big_int& big_int::operator = (big_int bi) {
        swap(bi);
        return *this;
    }

    size_t big_int::act_size() const {
        return buf_.get_size();
    }

    big_int& big_int::operator += (const big_int& bi) {
        const size_t com_size = std::min(act_size(), bi.act_size());
        const size_t res_size = std::max(act_size(), bi.act_size());

        const uint32_t* left = buf_.get_data();
        const uint32_t* right = bi.buf_.get_data();

        uint32_t* res_data = new uint32_t[res_size + 1];

        uint64_t removal = 0;
        for (size_t i = 0; i < com_size; ++i) {
            uint64_t res = (uint64_t)left[i] + (uint64_t)right[i] + removal;
            removal = static_cast<uint32_t>(res / BIGINT_BASE);
            res_data[i] = static_cast<uint32_t>(res % BIGINT_BASE);
        }

        const uint32_t* max_num;
        if (act_size() == res_size) {
            max_num = left;
        } else {
            max_num = right;
        }

        for (size_t i = com_size; i < res_size; ++i) {
            uint64_t res = (uint64_t)max_num[i] + removal;
            removal = static_cast<uint32_t>(res / BIGINT_BASE);
            res_data[i] = static_cast<uint32_t>(res % BIGINT_BASE);
        }

        if (removal > 0) {
            res_data[res_size] = static_cast<uint32_t>(removal);
            buf_ = shared_buffer(res_size + 1);
            memcpy(buf_.get_data(), res_data, sizeof(uint32_t) * (res_size + 1));
        } else {
            buf_ = shared_buffer(res_size);
            memcpy(buf_.get_data(), res_data, sizeof(uint32_t) * (res_size));
        }

        delete []res_data;

        return *this;
    }

    big_int& big_int::operator *= (const big_int& bi) {
        const uint32_t* left;
        const uint32_t* right;

        const size_t size_left  = std::max(act_size(), bi.act_size());
        const size_t size_right = std::min(act_size(), bi.act_size());

        if (act_size() > bi.act_size()) {
            left = buf_.get_data();
            right = bi.buf_.get_data();
        } else {
            left = bi.buf_.get_data();
            right = buf_.get_data();
        }

        uint32_t* data = new uint32_t[size_left + size_right];
        memset(data, 0, sizeof(uint32_t) * (size_left + size_right));
        vector<uint32_t> v(data, size_left + size_right);
        for (size_t i = 0; i < size_right; ++i) {
            for (size_t j = 0, removal = 0; j < size_left || removal; ++j) {
                if (j > size_left) {
                    v.push_back(0);
                }
                uint64_t cur = v[i + j] +
                        (uint64_t)right[i] * (j < size_left ? left[j] : 0) +
                        removal;
                v[i + j] = static_cast<uint32_t>(cur % BIGINT_BASE);
                removal = static_cast<uint32_t>(cur / BIGINT_BASE);
            }
        }

        while (v.size() > 1 && v.back() == 0) {
            v.pop_back();
        }

        buf_ = shared_buffer(v.size());
        memcpy(buf_.get_data(), v.get_data(), sizeof(uint32_t) * v.size());

        delete []data;
        return *this;
    }

    big_int& big_int::operator -= (const big_int& bi) {
        int removal = 0;
        vector<uint32_t> v(buf_.get_data(), buf_.get_size());
        const size_t right_size = bi.buf_.get_size();
        const uint32_t* right_data = bi.buf_.get_data();
        for (size_t i = 0; i < right_size || removal; ++i) {
            v[i] -= removal + (i < right_size ? right_data[i] : 0);
            removal = v[i] > BIGINT_BASE;
            if (removal) {
                v[i] += BIGINT_BASE;
            }
        }

        while (v.size() > 1 && v.back() == 0) {
            v.pop_back();
        }

        buf_ = shared_buffer(v.size());
        memcpy(buf_.get_data(), v.get_data(), sizeof(uint32_t) * v.size());

        return *this;
    }

    big_int& big_int::operator /= (uint32_t n) {
        if (n == 0) {
            throw std::invalid_argument("Division by zero");
        }

        vector<uint32_t> v(buf_.get_data(), act_size());

        uint64_t removal = 0;
        for (size_t i = v.size(); i > 0; --i) {
            uint64_t res = (uint64_t)(v[i - 1]) + removal * BIGINT_BASE;
            v[i - 1] = static_cast<uint32_t>(res / n);
            removal = res % n;
        }
        while (v.size() > 1 && v.back() == 0)
            v.pop_back();

        buf_ = shared_buffer(v.size());
        memcpy(buf_.get_data(), v.get_data(), sizeof(uint32_t) * v.size());

        return *this;
    }

    big_int& big_int::operator /= (const big_int& bi) {
        if (bi == big_int(0)) {
            throw std::invalid_argument("Division by zero");
        }

        big_int l(0);
        big_int r(*this);

        while (l < r) {
            big_int m = (l + r) / 2;
            if ((m * bi) > *this) {
                r = m;
            } else
                if ((m * bi) < *this) {
                    l = m + big_int(1);
                } else {
                    swap(m);
                    return *this;
                }
        }

        if (r * bi == *this) {
            swap(r);
            return *this;
        }

        if (l * bi > *this) {
            l -= big_int(1);
        }

        swap(l);
        return *this;
    }

    std::ostream& operator << (std::ostream& os, const big_int& bi) {
        const uint32_t* data = bi.buf_.get_data();
        size_t num_size = bi.buf_.get_size();
        os << data[num_size - 1];
        for (size_t i = num_size - 1; i > 0; --i) {
            os << std::setw(9) << std::setfill('0') << data[i - 1];
        }
        return os;
    }

    std::istream& operator >> (std::istream& is, big_int& bi) {
        std::string str;
        is >> str;
        bi = big_int(str);
        return is;
    }

    big_int operator + (big_int bi, const big_int& bi2) {
        bi += bi2;
        return bi;
    }

    big_int operator * (big_int bi, const big_int& bi2) {
        bi *= bi2;
        return bi;
    }

    big_int operator - (big_int bi, const big_int& bi2) {
        bi -= bi2;
        return bi;
    }

    big_int operator / (big_int bi, uint32_t n) {
        bi /= n;
        return bi;
    }

    big_int operator / (big_int bi, const big_int& bi2) {
        bi /= bi2;
        return bi;
    }

    bool operator == (const big_int& bi, const big_int& bi2) {
        if (bi.act_size() != bi2.act_size()) {
            return false;
        }

        size_t size = bi.act_size();
        const uint32_t* left  = bi.buf_.get_data();
        const uint32_t* right = bi2.buf_.get_data();

        for (size_t i = 0; i < size; ++i) {
            if (left[i] != right[i]) {
                return false;
            }
        }

        return true;
    }

    bool operator != (const big_int& bi, const big_int& bi2) {
        return !(bi == bi2);
    }

    bool operator > (const big_int& bi, const big_int& bi2) {
        if (bi.act_size() > bi2.act_size()) {
            return true;
        }
        if (bi.act_size() < bi2.act_size()) {
            return false;
        }

        size_t size = bi.act_size();
        const uint32_t* left  = bi.buf_.get_data();
        const uint32_t* right = bi2.buf_.get_data();

        for (size_t i = size; i > 0; --i) {
            if (left[i - 1] > right[i - 1]) {
                return true;
            }
            if (left[i - 1] < right[i - 1]) {
                return false;
            }
        }

        return false;
    }

    bool operator  < (const big_int& bi, const big_int& bi2) {
        return bi2 > bi;
    }

    bool operator >= (const big_int& bi, const big_int& bi2) {
        return (bi == bi2) || (bi > bi2);
    }

    bool operator <= (const big_int& bi, const big_int& bi2) {
        return (bi == bi2) || (bi < bi2);
    }

    big_int gcd(const big_int& bi, const big_int& bi2) {
        big_int l(bi);
        big_int r(bi2);

        while (l != r) {
            if (l == big_int(0)) {
                return r;
            }
            if (r == big_int(0)) {
                return l;
            }

            if (l > r) {
                big_int removal = l - (l / r) * r;
                l = removal;
            } else if (l < r) {
                big_int removal = r - (r / l) * l;
                r = removal;
            } else {
                return l;
            }
        }

        return l;
    }

} // namespace apa
