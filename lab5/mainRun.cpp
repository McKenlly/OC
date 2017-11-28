#include <iostream>
#include "usr/vector.h"
const int SIZE_STRING = 256;

int main() {
    TVector<char *> vec;
    char *s = new char[SIZE_STRING];
    while (scanf("%s", s) != EOF) {

        vec.Push_back(s);
        
        s = new char[SIZE_STRING];
    }
    delete [] s;

    for (auto i:vec) {
        std::cout << **i  <<  i << std::endl;
    }

    return 0;
}
