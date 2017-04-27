#include <iostream>

int num_steps(int n) {
	int steps[21];
	steps[0] = 1;
	steps[1] = 1;

	for (int i = 2; i <= n; i++) {
		steps[i] = steps[i-1] + steps[i-2];
	}
	
	return steps[n];
}

int main() {
	using namespace std;
	int n;
	cin >> n;
	cout << num_steps(n) << endl;
	return 0;
}