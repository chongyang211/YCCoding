//
// Created by 杨充 on 2025/7/15.
//
#include "stack.h"

// 初始化栈
void stack_init(Stack* s) {
    s->top = -1;
}

// 压入栈
bool stack_push(Stack* s, double value) {
    if (s->top >= STACK_SIZE - 1) {
        return false; // 栈满
    }
    s->data[++s->top] = value;
    return true;
}

// 弹出栈
bool stack_pop(Stack* s, double* value) {
    if (s->top < 0) {
        return false; // 栈空
    }
    *value = s->data[s->top--];
    return true;
}

// 查看栈顶
bool stack_peek(Stack* s, double* value) {
    if (s->top < 0) {
        return false; // 栈空
    }
    *value = s->data[s->top];
    return true;
}

// 检查栈是否为空
bool stack_is_empty(Stack* s) {
    return s->top == -1;
}