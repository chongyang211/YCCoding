//
// Created by 杨充 on 2025/7/15.
//

#ifndef ERROR_HANDLER_H
#define ERROR_HANDLER_H

#include <stdio.h>
#include <string.h>

// 计算器错误类型
typedef enum {
    ERROR_NONE,
    ERROR_DIV_ZERO,
    ERROR_SYNTAX,
    ERROR_OVERFLOW,
    ERROR_UNDEFINED_VAR,
    ERROR_PARENTHESIS,
    ERROR_DOMAIN,
    ERROR_ARGUMENTS
} CalculatorError;

// 错误处理器
typedef struct {
    CalculatorError last_error;
    char error_message[128];
} ErrorHandler;

// 初始化错误处理器
void error_handler_init(ErrorHandler* eh);

// 设置错误
void error_handler_set(ErrorHandler* eh, CalculatorError error, const char* message);

// 获取错误信息
const char* error_handler_get_message(ErrorHandler* eh);

// 检查是否有错误
bool error_handler_has_error(ErrorHandler* eh);

#endif // ERROR_HANDLER_H
