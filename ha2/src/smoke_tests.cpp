#include "catch.hpp"
#include "rational.h"
#include <sstream>


const apa::rational ONE(1);
const apa::rational ZERO(0);


TEST_CASE("construction")
{
    apa::rational default_rational;
    CHECK(default_rational == apa::rational());

    CHECK(ONE == apa::rational(1, 1));
    CHECK(ONE == apa::rational(ONE));
    CHECK(ONE == apa::rational(987, 987));
    CHECK(ONE == apa::rational("1"));
    CHECK(ONE == apa::rational("1", "1"));
    {
        apa::rational one_copy;
        one_copy = ONE;
        CHECK(ONE == one_copy);
    }
}


TEST_CASE("string IO")
{
    std::string const ONE_THIRD_STR("1 / 3");
    apa::rational ONE_THIRD(1, 3);
    CHECK(ONE_THIRD.str() == ONE_THIRD_STR);
    {
        std::stringstream ss(ONE_THIRD_STR);
        apa::rational rational_number;
        ss >> rational_number;
        CHECK(rational_number == ONE_THIRD);
    }
    {
        std::stringstream ss;
        ss << ONE_THIRD;
        CHECK(ss.str() == ONE_THIRD_STR);
    }
}


TEST_CASE("converting to bool")
{
    CHECK(ONE);
    CHECK_FALSE(ZERO);
}


TEST_CASE("comparison operations")
{
    apa::rational two_third(2, 3);
    CHECK(ZERO < two_third);
    CHECK(ZERO <= two_third);
    CHECK(ZERO != two_third);
    CHECK(ONE > two_third);
    CHECK(ONE >= two_third);
}


TEST_CASE("simple arithmetic operations")
{
    CHECK(ONE + ONE == apa::rational(2));
    CHECK(ONE == apa::rational(0) + ONE);
    CHECK(ONE / ONE == ONE);
    CHECK(ONE * ONE == ONE);

    {
        apa::rational num(1);
        num += num * (num + ONE);
        CHECK(num == apa::rational(3));
    }
    {
        apa::rational num(5, 2);
        num /= num *= apa::rational(212, 321);
        CHECK(num == ONE);
    }
}
