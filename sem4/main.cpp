#include <iostream>
#include <string.h>
#include <stdio.h>

enum Operation {
    add = 0,
    sub,
    read,
    floating,
    google,
    palindrom
};

Operation char_to_op(char c) {
    switch (c) {
        case 'a': return add;
        case 's': return sub;
        case 'r': return read;
        case 'f': return floating;
        case 'g': return google;
        case 'p': return palindrom;
    }
}

char* make_op(Operation o, int& a) {
    using namespace std;
    int i;
    double d;
    char* str = new char[1000];
    char* buf = new char[500];
    switch (o) {
        case add:
            cin >> i;
            a = a + i;
            sprintf(str, "%d\n", a);
            break;
        case sub:
            cin >> i;
            a = a - i;
            sprintf(str, "%d\n", a);
            break;
        case floating:
            cin >> d;
            a = a + static_cast<int>(d + 0.5);
            sprintf(str, "%d\n", a);
            break;
        case google: {
            cin.getline(buf, 1023);
            strcat(str, "https://www.google.ru/search?q=");
            int k = strlen(str);
            for (int i = 1; i < strlen(buf); i++) {
                if (buf[i] == ' ') {
                    str[k] = '%';
                    str[k+1] = '2';
                    str[k+2] = '0';
                    k += 3;
                }
                else {
                    str[k] = buf[i];
                    k++;
                }
            }
            str[k] = '\n';
            break; }
        case palindrom:
            cin >> buf;
            str[0] = '\0';

            for (int i = 0; i < strlen(buf); i++) {
                for (int j = i + 1; j < strlen(buf); j++) {
                    for (int k = i; k < j; k++) {
                        if (buf[k] != buf[j-k]) {
                            //break;
                        }
                        if (k == j && (k-j) > strlen(str)) {
                            strncpy(str, buf + i, j - i);
                        }
                    }
                }
            }
            break;
    }

    delete  []buf;
    return str;
}

int main() {
    using namespace std;

    int result = 0;

    char c;
    while (true) {
        cin >> c;
        Operation o = char_to_op(c);
        char* str = make_op(o, result);

        if (o == read || o == google) {
            cout << str;
        }

        delete []str;
    }

    return 0;
}
