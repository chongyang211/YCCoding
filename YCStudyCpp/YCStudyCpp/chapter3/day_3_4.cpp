//
// Created by 杨充 on 2025/3/13.
//
#include <iostream>
using namespace std;
int main() {
    float f1 = 3.14;
    float f2 = 3.14f;
    double d1 = 3.14;
    double d2 = 3.14f;
    cout << f1 << endl;
    cout << f2 << endl;
    cout << d1<< endl;
    cout << d2<< endl;
    cout << "float  sizeof = " << sizeof(f1) << endl;
    cout << "float  sizeof = " << sizeof(f2) << endl;
    cout << "double sizeof = " << sizeof(d1) << endl;
    cout << "double sizeof = " << sizeof(d2) << endl;

    //科学计数法
    float f3 = 3e2; // 3 * 10 ^ 2
    cout << "f3 = " << f2 << endl;
    float f4 = 3e-2;  // 3 * 0.1 ^ 2
    cout << "f3 = " << f3 << endl;
    return 0;
}
//3.14
//3.14
//3.14
//3.14
//float  sizeof = 4
//float  sizeof = 4
//double sizeof = 8
//double sizeof = 8
//f3 = 3.14
//f3 = 300
