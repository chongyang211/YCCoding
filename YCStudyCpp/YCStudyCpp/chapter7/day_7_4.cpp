//
// Created by 杨充 on 2025/5/12.
//
#include "iostream"
using namespace std;

int test1() {
    //指针变量p指向内存地址编号为0的空间
    int * p = NULL;
    //访问空指针报错
    //内存编号0 ~255为系统占用内存，不允许用户访问
    cout << *p << endl;
    return 0;
}

int test2() {
    //指针变量p指向内存地址编号为0x1100的空间
    int * p = (int *)0x1100;
    //访问野指针报错
    cout << *p << endl;
    return 0;
}

int main() {
    test1();
    test2();
    return 0;
}