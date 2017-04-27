#include <iostream>

int main() {
	int n = 10;
	int m = 10;

	int** matrix = new int*[m];
	int* buf = new int[m * n];
	for (int i = 0; i < m; i++) {
		matrix[i] = buf + i*n;
	}

	return 0;
}