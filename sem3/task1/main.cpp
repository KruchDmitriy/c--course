#include <iostream>
using namespace std;

void foo_stack(int a, int b) {
	cout << "foo_stack\n";
	cout << &a << endl;
	cout << &b << endl;
}

void foo_heap() {
	int *a = new int(1);
	int *b = new int(2);

	cout << "foo_heap\n";
	cout << a << endl;
	cout << b << endl;

	delete a;
	delete b;
}

int main() {
	foo_stack(1, 2);
	foo_heap();
	return 0;
}