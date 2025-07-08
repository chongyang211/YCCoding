//
// Created by 杨充 on 2025/6/12.
//

#include <iostream>
using namespace std;


void test1() {
    int a = 10;
    int &b = a;
    cout << "a = " << a << endl;
    cout << "b = " << b << endl;
    b = 100;
    cout << "a = " << a << endl;
    cout << "b = " << b << endl;
}

int main() {
    test1();
    return 0;
}

//a = 10
// b = 10
// a = 100
// b = 100

