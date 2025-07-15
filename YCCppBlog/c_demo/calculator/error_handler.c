//
// Created by 杨充 on 2025/7/15.
//
#include "error_handler.h"

// 初始化错误处理器
void error_handler_init(ErrorHandler* eh) {
    eh->last_error = ERROR_NONE;
    eh->error_message[0] = '\0';
}

// 设置错误
void error_handler_set(ErrorHandler* eh, CalculatorError error, const char* message) {
    eh->last_error = error;
    if (message) {
        strncpy(eh->error_message, message, sizeof(eh->error_message) - 1);
        eh->error_message[sizeof(eh->error_message) - 1] = '\0';
    } else {
        switch (error) {
            case ERROR_DIV_ZERO:
                strcpy(eh->error_message, "除以零错误");
            break;
            case ERROR_SYNTAX:
                strcpy(eh->error_message, "语法错误");
            break;
            case ERROR_OVERFLOW:
                strcpy(eh->error_message, "溢出错误");
            break;
            case ERROR_UNDEFINED_VAR:
                strcpy(eh->error_message, "未定义的变量");
            break;
            case ERROR_PARENTHESIS:
                strcpy(eh->error_message, "括号不匹配");
            break;
            case ERROR_DOMAIN:
                strcpy(eh->error_message, "定义域错误");
            break;
            case ERROR_ARGUMENTS:
                strcpy(eh->error_message, "参数错误");
            break;
            case ERROR_NONE:
                default:
                    eh->error_message[0] = '\0';
            break;
        }
    }
}

// 获取错误信息
const char* error_handler_get_message(ErrorHandler* eh) {
    return eh->error_message;
}

// 检查是否有错误
bool error_handler_has_error(ErrorHandler* eh) {
    return eh->last_error != ERROR_NONE;
}