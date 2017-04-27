#include "catch.hpp"
#include "rational.h"


bool operator ==(const apa::rational &l, int r)
{
	return l == apa::rational(r);
}


TEST_CASE("arithmetic operations")
{
	apa::rational a(8);
	apa::rational b(64);
	apa::rational c(500);
	apa::rational d(1024);
	CHECK((a + b) == (72));
	a += b;
	CHECK((a) == (72));
	a = apa::rational(8);
	CHECK((a * b) == (8 * 64));
	a *= b;
	CHECK((a) == (8 * 64));
	CHECK((a / b) == (8));
	a /= b;
	CHECK((a) == (8));
	apa::rational ac(a);
	CHECK((ac) == (a));
	ac = a * c;
	CHECK((ac) == (8 * 500));
	ac = apa::rational(8 * 500);
	ac = ac + b + c;
	CHECK((ac) == (8 * 500 + 64 + 500));
	ac = a;
	ac = b + c + ac;
	CHECK((ac) == (8 + 64 + 500));
	ac = a;
	ac = ac * b;
	CHECK((ac) == (8 * 64));
	ac = a;
	ac *= b * ac;
	CHECK((ac) == (8 * 8 * 64));
	ac = b;
	ac = ac / a;
	CHECK((ac) == (64 / 8));
	ac = b;
	ac /= ac / a;
	CHECK((ac) == (64 / (64 / 8)));
	ac = a;
	ac = b + ac * a;
	CHECK((ac) == (64 * 2));
	ac = a;
	ac = b * (ac + a);
	CHECK((ac) == (64 * (16)));
	ac = a;
	ac = b / (ac);
	CHECK((ac) == (64 / 8));
	ac = a;
	ac = ac + b;
	CHECK((ac) == (8 + 64));
	ac = a;
	ac = a + ac;
	CHECK((ac) == (16));
	ac = a;
	ac += a + b;
	CHECK((ac) == (80));
	ac = a;
	ac += b + a;
	CHECK((ac) == (80));
	ac = a;
	CHECK((ac) == (8));
	ac = apa::rational(8);
	ac = a * ac;
	CHECK((ac) == (8 * 8));
	ac = a;
	ac += a;
	CHECK((ac) == (16));
	ac = a;
	ac += b*c;
	CHECK((ac) == (8 + 64 * 500));
	ac = a;
	ac += ac * b;
	CHECK((ac) == (8 + 8 * 64));
	ac = a * apa::rational(8);
	ac *= a;
	CHECK((ac) == (64 * 8));
	ac = a;
	ac *= b * c;
	CHECK((ac) == (8 * 64 * 500));
	ac = a;
	ac *= b / a;
	CHECK((ac) == (8 * 64 / 8));
	ac = a;
	ac *= b + c;
	CHECK((ac) == (8 * (64 + 500)));
	ac = b;
	ac /= a;
	CHECK((ac) == (8));
	ac = b;
	ac /= b / a;
	CHECK((ac) == (64 / (64 / 8)));
	ac = b;
	ac /= a + apa::rational(0);
	CHECK((ac) == (8));

	ac = a + b;
	CHECK((ac) == (72));
	ac = a + b;
	CHECK((ac) == (72));
	ac = a + b;
	CHECK((ac) == (72));
	ac = a + b;
	CHECK((ac) == (72));
	ac = a;
	ac = b / ac;
	CHECK((ac) == (b / a));
}