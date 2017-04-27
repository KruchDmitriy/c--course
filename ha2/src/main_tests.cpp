#include "catch.hpp"
#include "rational.h"
#include <math.h>
#include <string>
#include <cstdlib>
#include <algorithm>


TEST_CASE("factorial calculation")
{
    auto factorial = [](size_t n)
    {
        apa::rational result(1);
        for (size_t i = 2; i <= n; ++i)
        {
            result *= apa::rational(i);
        }
        return result;
    };

    CHECK(factorial(4) == apa::rational(2 * 3 * 4));
    CHECK((factorial(60) / factorial(59)) == apa::rational(60));
    CHECK(factorial(95) == apa::rational(
              "10329978488239059262599702099394727095397746340117"
              "37286921225057123429398759470312487176537538542446"
              "8563282236864226607350415360000000000000000000000"));
}


TEST_CASE("long chained calculations")
{
    int const CHAIN_LEN = 400;
    SECTION("division")
    {
        apa::rational result(1);
        for (int i = 1; i <= CHAIN_LEN; ++i) {
            result /= apa::rational(i, i + 1);
        }
        CHECK(result == apa::rational(CHAIN_LEN + 1));
    }
    SECTION("multiplication")
    {
        apa::rational result(1);
        for (int i = 1; i <= CHAIN_LEN; ++i) {
            result *= apa::rational(i, i + 1);
        }
        CHECK(result == apa::rational(1, CHAIN_LEN + 1));
    }
    SECTION("sum")
    {
        apa::rational result(0);
        apa::rational two_pow_minus_i(1, CHAIN_LEN);
        for (int i = 1; i <= CHAIN_LEN; ++i) {
            result += apa::rational(1, CHAIN_LEN);
        }
        CHECK(result == apa::rational(1));
    }
}


TEST_CASE("harmonic numbers calculation")
{
    auto harmonic_number = [](int n)
    {
        apa::rational result(1);
        for (int i = 2; i <= n; ++i)
        {
            result += apa::rational(1, i);
        }
        return result;
    };

    CHECK((harmonic_number(21) / (harmonic_number(21)) == apa::rational(1)));
    CHECK(harmonic_number(29) ==
          apa::rational("9227046511387") / apa::rational("2329089562800"));
    CHECK(harmonic_number(54) == harmonic_number(53) + apa::rational(1, 54));
}


TEST_CASE("Taylor series approximation")
{
    auto test_approx_exp_for_x = [](int numerator, int denominator)
    {
        auto exp_approx = [](const apa::rational &x, int accuracy = 10)
        {
            apa::rational result(1);
            apa::rational factorial_i(1);
            apa::rational x_pow_i(1);
            for (int i = 1; i < accuracy; ++i) {
                x_pow_i *= x;
                factorial_i *= apa::rational(i);
                result += x_pow_i / factorial_i;
            }

            return result;
        };
        double expd = exp(static_cast<double>(numerator) / denominator);

        int const PRECISION = static_cast<int>(1e8);
        apa::rational expected(static_cast<int>(expd * PRECISION), PRECISION);
        apa::rational real(exp_approx(apa::rational(numerator, denominator)));

        apa::rational const DELTA(1, PRECISION);
        CHECK(real < expected + DELTA);
        CHECK(real + DELTA > expected);
    };

    test_approx_exp_for_x(0, 2);
    test_approx_exp_for_x(2, 123456);
    test_approx_exp_for_x(9, 321456);
    test_approx_exp_for_x(4, 9);
    test_approx_exp_for_x(123, 193);
}


TEST_CASE("construction from string and string IO")
{
    std::string const LONG_STR_NUM(
                "7892387943789432623167213124871342712643909432"
                "1234567899876543112345678975433456787128132789");
    CHECK(apa::rational(LONG_STR_NUM, LONG_STR_NUM + "000") ==
          apa::rational("1", "1000"));
    CHECK(apa::rational("1", LONG_STR_NUM).str() ==
          ("1 / " + LONG_STR_NUM));
    CHECK((apa::rational("1", LONG_STR_NUM) *
           apa::rational(LONG_STR_NUM)) == apa::rational(1));
    CHECK_THROWS_AS(apa::rational("Hello world", "2"), apa::wrong_format);
    CHECK_THROWS_AS(apa::rational("1", "oops"), apa::wrong_format);
	CHECK_THROWS_AS(apa::rational("12f", "4"), apa::wrong_format);
	CHECK_THROWS_AS(apa::rational("12", "4a"), apa::wrong_format);
	CHECK_THROWS_AS(apa::rational("12y", "4a"), apa::wrong_format);
	CHECK(apa::rational("6", "4").str() == "3 / 2");
	CHECK(apa::rational("60000000", "40000000").str() == "3 / 2");
	CHECK(apa::rational("123456", "2").str() == "61728 / 1");
	CHECK(apa::rational("21", "42").str() == "1 / 2");

    {
        std::stringstream ss(LONG_STR_NUM + " / " + LONG_STR_NUM);
        apa::rational number_red;
        ss >> number_red;
        CHECK(number_red == apa::rational(1));
    }
	{
		std::stringstream ss(LONG_STR_NUM + "/ " + LONG_STR_NUM);
		apa::rational number_red;
		CHECK_THROWS_AS(ss >> number_red, apa::wrong_format);
	}
	{
		std::stringstream ss(LONG_STR_NUM + " /" + LONG_STR_NUM);
		apa::rational number_red;
		CHECK_THROWS_AS(ss >> number_red, apa::wrong_format);
	}
	{
		std::stringstream ss(LONG_STR_NUM + " / " + "ooops");
		apa::rational number_red;
		CHECK_THROWS_AS(ss >> number_red, apa::wrong_format);
	}
	{
		std::stringstream ss("nope / " + LONG_STR_NUM);
		apa::rational number_red;
		CHECK_THROWS_AS(ss >> number_red, apa::wrong_format);
	}
    {
        std::stringstream ss(LONG_STR_NUM);
        apa::rational number_red;
        CHECK_THROWS_AS(ss >> number_red, apa::wrong_format);
    }
    {
        apa::rational number_written("1", LONG_STR_NUM);
        std::stringstream ss;
        ss << number_written;
        CHECK(ss.str() == ("1 / " + LONG_STR_NUM));
    }
}


TEST_CASE("sorting rational numbers")
{
    std::vector<apa::rational> rationals(1000);
    std::generate(rationals.begin(), rationals.end(),
                  []() { return apa::rational(rand(), rand() + 1);});
    std::sort(rationals.begin(), rationals.end());
    CHECK(std::is_sorted(rationals.begin(), rationals.end()));
}
