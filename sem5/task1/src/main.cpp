#include <iostream>
#include <vector>
#include <functional>
#include <cstdio>

#include "event.h"

void for_each(std::vector<int>& v,
			  std::function<int(int x)> f) {
	for (uint i = 0; i < v.size(); i++) {
		v[i] = f(v[i]);
	}
}

int sum(std::vector<int> const& v,
		std::function<int (int x)> f) {
	int res = 0;

	for (uint i = 0; i < v.size(); i++) {
		res += f(v[i]);
	}

	return res;
}

int ix(int x) { return x; }

int square(int x) {
	return x * x;
}

template <typename T>
void read(FILE* f, T &val) {
	T *buf = new T;
	fread(buf, sizeof(T), 1, f);
	val = *buf;
}

template <typename T>
void write(FILE* f, T val) {
	fwrite(&val, sizeof(T), 1, f);
}

int main() {
	using namespace std;

	vector<int> v = { 1, 2, 3, 4, 5 };

	for_each(v, square);

	for (uint i = 0; i < v.size(); i++) {
		cout << v[i] << " ";
	}
	cout << endl;

	int sum_of_squares = sum(v, ix);
	cout << sum_of_squares << endl;

	FILE *f_in = fopen("input.txt", "rb");
	FILE *f_out = fopen("output.txt", "wb");
	for (uint i = 0; i < v.size(); i++) {
		write(f_out, v[i]);
	}

	int* from_file = new int[v.size()];
	for (uint i = 0; i < v.size(); i++) {
		read(f_in, from_file[i]);
		cout << from_file[i] << " ";
	}
	cout << endl;

	event ev;
	//ev.subscribe(std::bind(foobar, 1, 2, 3, _1));
	ev.subscribe([](const char *arg) { std::cout << arg; });
	// вызываются foobar(“hello!”), lambda(“hello!”)
	ev.fire("hello!\n");

	return 0;
}
