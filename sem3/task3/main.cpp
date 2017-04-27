#include <iostream>
#include <string.h>

const char* concat(const char a[], 
				   const size_t a_size,
				   const char b[],
				   const size_t b_size,
				   size_t &concat_size) {
	concat_size = a_size + b_size;
	char *c = new char[concat_size];
	
	memcpy(c, a, a_size);
	memcpy(c + a_size, b, b_size);

	return c;
}

int main() {
	using namespace std;
	const char* a = "maka";
	const char* b = "ka";

	size_t size;

	const char* c = concat(a, strlen(a),
		   b, strlen(b) + 1,
		   size);

	cout << c << endl;
	delete []c;
	return 0;
}