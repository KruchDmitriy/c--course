#include <iostream>

#include "my_ostream.h"

my_ostream& operator<<(my_ostream& os, const std::string& str) {
	os.print(str);
	return os;
}

my_ostream& operator<<(my_ostream& os, int n) {
	os.print(n);
	return os;
}

my_ostream& operator<<(my_ostream& os, double d) {
	os.print(d);
	return os;
}

void my_ostream_con::print(const std::string& str) {
	std::cout << str;
}

void my_ostream_con::print(int n) {
	std::cout << n;
}

void my_ostream_con::print(double d) {
	std::cout << d;
}

my_ostream_file::my_ostream_file(std::string filename)
	: fout(filename) {}

void my_ostream_file::print(const std::string& str) {
	fout << str;
}

void my_ostream_file::print(int n) {
	fout << n;
}

void my_ostream_file::print(double d) {
	fout << d;
}
