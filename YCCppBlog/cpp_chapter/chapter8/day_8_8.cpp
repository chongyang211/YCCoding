//
// Created by 杨充 on 2025/6/12.
//

#include <iostream>
using namespace std;

int& badFunction() {
    int a = 10;
    return a; // 错误：返回局部变量的引用
}

int main() {
    int num = badFunction();
    cout << num << endl;
    int b = 10;
    int& ref = b;
    int* ptr = &b;
    // ref 和 *ptr 是等价的
    cout << ref << endl;
    cout << ptr << endl;
    return 0;
}

