//
// Created by 杨充 on 2025/5/12.
//
#include "iostream"
using namespace std;

int main() {
    int a = 10;
    int * p;
    p = &a; //指针指向数据a的地址
    cout << *p << endl; //* 解引用
    cout << sizeof(p) << endl;
    cout << sizeof(char *) << endl;
    cout << sizeof(float *) << endl;
    cout << sizeof(double *) << endl;
    return 0;
}