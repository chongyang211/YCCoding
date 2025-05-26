//
// Created by 杨充 on 2025/4/28.
//
#include <iostream>
using namespace std;

//函数定义
int add(int num1, int num2) {
    int sum = num1 + num2;
    return sum;
}

int main() {
    int num = add(1,2);
    printf("num %d",num);
    return 0;
}


