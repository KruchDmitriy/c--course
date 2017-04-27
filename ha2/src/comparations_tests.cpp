#include "catch.hpp"
#include "rational.h"


void test_comparisons_for(int n1, int n2, int d1 = 1, int d2 = 1)
{
	const apa::rational r1(n1, d1);
	const apa::rational r2(n2, d2);
	const apa::rational one(1);
	INFO("Running comparisons test for ");
	INFO("r1");
	INFO(r1);
	INFO("r2");
	INFO(r2);

	CHECK((r1 == r2) == (n1 * d2 == n2 * d1));
	CHECK((r1 != r2) == (n1 * d2 != n2 * d1));
	CHECK((r1 <= r2) == (n1 * d2 <= n2 * d1));
	CHECK((r1 < r2) == (n1 * d2 < n2 * d1));
	CHECK((r1 >= r2) == (n1 * d2 >= n2 * d1));
	CHECK((r1 > r2) == (n1 * d2 > n2 * d1));

	CHECK((r1 == apa::rational(n2, d2)) == (n1 * d2 == n2 * d1));
	CHECK((r1 != apa::rational(n2, d2)) == (n1 * d2 != n2 * d1));
	CHECK((r1 <= apa::rational(n2, d2)) == (n1 * d2 <= n2 * d1));
	CHECK((r1 < apa::rational(n2, d2)) == (n1 * d2 < n2 * d1));
	CHECK((r1 >= apa::rational(n2, d2)) == (n1 * d2 >= n2 * d1));
	CHECK((r1 > apa::rational(n2, d2)) == (n1 * d2 > n2 * d1));

	CHECK((apa::rational(n1, d1) == r2) == (n1 * d2 == n2 * d1));
	CHECK((apa::rational(n1, d1) != r2) == (n1 * d2 != n2 * d1));
	CHECK((apa::rational(n1, d1) <= r2) == (n1 * d2 <= n2 * d1));
	CHECK((apa::rational(n1, d1) < r2) == (n1 * d2 < n2 * d1));
	CHECK((apa::rational(n1, d1) >= r2) == (n1 * d2 >= n2 * d1));
	CHECK((apa::rational(n1, d1) > r2) == (n1 * d2 > n2 * d1));

	CHECK((r1*one == r2) == (n1 * d2 == n2 * d1));
	CHECK((r1*one != r2) == (n1 * d2 != n2 * d1));
	CHECK((r1*one <= r2) == (n1 * d2 <= n2 * d1));
	CHECK((r1*one < r2) == (n1 * d2 < n2 * d1));
	CHECK((r1*one >= r2) == (n1 * d2 >= n2 * d1));
	CHECK((r1*one > r2) == (n1 * d2 > n2 * d1));

	CHECK((r1 == r2*one) == (n1 * d2 == n2 * d1));
	CHECK((r1 != r2*one) == (n1 * d2 != n2 * d1));
	CHECK((r1 <= r2*one) == (n1 * d2 <= n2 * d1));
	CHECK((r1 < r2*one) == (n1 * d2 < n2 * d1));
	CHECK((r1 >= r2*one) == (n1 * d2 >= n2 * d1));
	CHECK((r1 > r2*one) == (n1 * d2 > n2 * d1));

	CHECK((r1*one == r2*one) == (n1 * d2 == n2 * d1));
	CHECK((r1*one != r2*one) == (n1 * d2 != n2 * d1));
	CHECK((r1*one <= r2*one) == (n1 * d2 <= n2 * d1));
	CHECK((r1*one < r2*one) == (n1 * d2 < n2 * d1));
	CHECK((r1*one >= r2*one) == (n1 * d2 >= n2 * d1));
	CHECK((r1*one > r2*one) == (n1 * d2 > n2 * d1));

	CHECK((r1*one == apa::rational(n2, d2)) == (n1 * d2 == n2 * d1));
	CHECK((r1*one != apa::rational(n2, d2)) == (n1 * d2 != n2 * d1));
	CHECK((r1*one <= apa::rational(n2, d2)) == (n1 * d2 <= n2 * d1));
	CHECK((r1*one < apa::rational(n2, d2)) == (n1 * d2 < n2 * d1));
	CHECK((r1*one >= apa::rational(n2, d2)) == (n1 * d2 >= n2 * d1));
	CHECK((r1*one > apa::rational(n2, d2)) == (n1 * d2 > n2 * d1));

	CHECK((apa::rational(n1, d1) == r2*one) == (n1 * d2 == n2 * d1));
	CHECK((apa::rational(n1, d1) != r2*one) == (n1 * d2 != n2 * d1));
	CHECK((apa::rational(n1, d1) <= r2*one) == (n1 * d2 <= n2 * d1));
	CHECK((apa::rational(n1, d1) < r2*one) == (n1 * d2 < n2 * d1));
	CHECK((apa::rational(n1, d1) >= r2*one) == (n1 * d2 >= n2 * d1));
	CHECK((apa::rational(n1, d1) > r2*one) == (n1 * d2 > n2 * d1));
}



TEST_CASE("comparison tests")
{
	test_comparisons_for(0, 0);
	test_comparisons_for(7, 0);
	test_comparisons_for(99, 0);
	test_comparisons_for(79, 98);
	test_comparisons_for(98, 98);
	test_comparisons_for(98, 98);
	test_comparisons_for(0, 0, 89, 92);
	test_comparisons_for(7, 0, 98, 1);
	test_comparisons_for(99, 0, 28, 9);
	test_comparisons_for(79, 98, 87, 123);
	test_comparisons_for(98, 98, 2, 980);
	test_comparisons_for(98, 98, 12, 2);
	test_comparisons_for(98, 98, 1, 13);
	test_comparisons_for(99, 99, 122, 122);
	test_comparisons_for(99, 99, 1812, 122);
	test_comparisons_for(99, 99, 1812, 12345);
	test_comparisons_for(1299, 992, 12211, 124212);
	test_comparisons_for(919, 199, 12232, 12132);
	test_comparisons_for(9, 1, 9, 1);
	test_comparisons_for(9, 1, 9, 2);
	test_comparisons_for(9, 1, 1, 2);
	test_comparisons_for(9, 45, 9, 1);
	test_comparisons_for(9, 912, 9, 2);
	test_comparisons_for(9, 213, 1, 2);
}