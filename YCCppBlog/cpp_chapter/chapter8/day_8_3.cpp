//
// Created by 杨充 on 2025/6/12.
//

#include <iostream>
using namespace std;

//**函数参数传递** 引用常用于函数参数传递，可以避免拷贝开销，同时允许函数修改实参。
void increment(int& num) {
    num++;
}

void test1() {
    int a = 10;
    increment(a); // 传递引用
    cout << "a = " << a << endl; // 输出 11
}

//**函数返回值** 引用可以作为函数的返回值，但必须确保返回的引用指向有效的内存。
int& getMax(int& a, int& b) {
    return (a > b) ? a : b;
}

void test2() {
    int x = 10, y = 20;
    cout << "y = " << getMax(x,y) << endl; // 输出 20
    getMax(x, y) = 30; // 修改较大的值
    cout << "y = " << y << endl; // 输出 30
}

int main() {
    test1();
    test2();
    return 0;
}

