#include <iostream>

#include "lazy_string.h"

int main() {
	using namespace std;

	lazy::lazy_string str("abracadabra");
	cout << find(str, "cadabr") << endl;

	cout << str << endl;

	return 0;
}
