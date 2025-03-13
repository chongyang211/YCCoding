//
// Created by 杨充 on 2025/3/13.
//
#include <iostream>
using namespace std;

//1.宏常量
#define day = 7;

int main() {
    //cout << "一周里总共有 %d" << day << " 天" << endl;
    //printf("一周里总共有:%d\n",day);
    //day = 8;  //报错，宏常量不可以修改
    //2、const修饰变量
    const int month = 12;
    cout << "一年里总共有 " << month << " 个月份" << endl;
    //month = 24; //报错，常量是不可以修改的
    return 0;
}