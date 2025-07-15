//
// Created by 杨充 on 2025/7/15.
//
#include <stdbool.h>
#ifndef STACK_H
#define STACK_H

#define STACK_SIZE 128

// 栈数据结构
typedef struct {
    double data[STACK_SIZE];
    int top;
} Stack;

// 初始化栈
void stack_init(Stack* s);

// 压入栈
bool stack_push(Stack* s, double value);

// 弹出栈
bool stack_pop(Stack* s, double* value);

// 查看栈顶
bool stack_peek(Stack* s, double* value);

// 检查栈是否为空
bool stack_is_empty(Stack* s);

#endif // STACK_H
