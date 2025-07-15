//
// Created by 杨充 on 2025/7/15.
//
#include "calculator.h"
#include "parser.h"
#include "error_handler.h"
#include "history.h"
#include "stack.h"
#include <ctype.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>

// 计算器状态
typedef struct {
    double result;
    char expression[MAX_EXPR_LEN];
    bool is_radians;
    History history;
    Stack memory_stack;
} CalculatorState;

static CalculatorState calculator;

// 全局错误处理器
ErrorHandler global_error_handler;

// 初始化计算器
void calculator_init(void) {
    calculator.result = 0.0;
    calculator.expression[0] = '\0';
    calculator.is_radians = true;
    history_init(&calculator.history);
    stack_init(&calculator.memory_stack);
    error_handler_init(&global_error_handler);
}

// 处理用户输入
void calculator_process_input(const char* input) {
    if (strlen(input) >= MAX_EXPR_LEN) {
        error_handler_set(&global_error_handler, ERROR_OVERFLOW, "表达式过长");
        return;
    }

    // 清除之前的错误
    error_handler_set(&global_error_handler, ERROR_NONE, "");

    // 更新当前表达式
    strncpy(calculator.expression, input, MAX_EXPR_LEN);
    calculator.expression[MAX_EXPR_LEN - 1] = '\0';

    // 解析和计算表达式
    Parser parser;
    parser_init(&parser, calculator.expression, &global_error_handler);

    CalculatorError error = ERROR_NONE;
    double result = parser_parse_expression(&parser, &error);

    if (error != ERROR_NONE) {
        if (error_handler_has_error(&global_error_handler)) {
            // 错误消息已在parser中设置
            return;
        }
        // 设置默认错误消息
        error_handler_set(&global_error_handler, error, "计算错误");
        return;
    }

    calculator.result = result;

    // 添加到历史记录
    history_add_entry(&calculator.history, calculator.expression, result);
}

// 获取当前结果
double calculator_get_result(void) {
    return calculator.result;
}

// 获取当前表达式
const char* calculator_get_expression(void) {
    return calculator.expression;
}

// 清除计算器状态
void calculator_clear(void) {
    calculator.result = 0.0;
    calculator.expression[0] = '\0';
    stack_init(&calculator.memory_stack);
    error_handler_set(&global_error_handler, ERROR_NONE, "");
}

// 清除最后输入
void calculator_backspace(void) {
    size_t len = strlen(calculator.expression);
    if (len > 0) {
        calculator.expression[len - 1] = '\0';
    }
}

// 设置角度单位
void calculator_set_angle_unit(bool radians) {
    calculator.is_radians = radians;
}

// 显示历史记录
void calculator_show_history(void) {
    printf("=== 计算历史 ===\n");
    HistoryEntry* entries;
    int count;
    history_get_entries(&calculator.history, &entries, &count);

    for (int i = 0; i < count; i++) {
        printf("%d. %s = %.10g\n", i+1, entries[i].expression, entries[i].result);
    }
    if (count == 0) {
        printf("无历史记录\n");
    }
}

// 清除历史记录
void calculator_clear_history(void) {
    history_clear(&calculator.history);
    printf("历史记录已清除\n");
}