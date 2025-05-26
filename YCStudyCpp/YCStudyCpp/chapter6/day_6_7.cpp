//
// Created by 杨充 on 2025/4/28.
//
#include<iostream>
using namespace std;

int add(int a, int b) {
    return a + b;
}

double add(double a, double b) {
    return a + b;
}

int add(int a, int b, int c) {
    return a + b + c;
}

int main() {
    int result1 = add(3, 5); // 调用第一个add函数
    double result2 = add(2.5, 3.7); // 调用第二个add函数
    int result3 = add(1, 2, 3); // 调用第三个add函数
    return 0;
}
