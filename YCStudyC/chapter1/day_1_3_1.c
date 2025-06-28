//
// Created by 杨充 on 2025/6/17.
//
#include <stdio.h>

int main(void) {
    printf("hello world");

    int x = 10;
    int *p = 1; // 错误
    int *l = &x; // 将 p 初始化为 x 的地址
    return 0;
}