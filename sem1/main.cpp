#include <iostream>
#include <string>

/*int fib(int n) {
    if (n == 0 || n == 1) return 1;
    int res = 1;
    for (int i = 1; i <= n; i++) {
        res *= i;
    }
    
    return res;
}
*/
int main()
{
    using namespace std;
    /*
     * float a[5];
    cin >> a[0] >> a[1] >> a[2] >> a[3] >> a[4];
    float mean = 0;
    for (int i = 0; i < 5; i++) {
        mean += a[i];
    }
    cout << mean / 5.f << endl;
    */

    /*ifstream in("input.txt");
    ofstream out("output.txt");

    int n;
    in >> n;
    out << fib(n);
    */
    
    string str;
    cin >> str;

    int n = str.length() - 1;
    for (int i = 0; i <= n / 2; ++i) {
        char tmp = str[i];
        str[i] = str[n-i];
        str[n-i] = tmp;
        //cout << i << " " << n-i-1 << " "  << tmp << " " << str[i] << " " << str[n-i-1];
    }

    cout << str << endl;

    return 0;
}
