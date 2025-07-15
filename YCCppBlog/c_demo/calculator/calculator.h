//
// Created by 杨充 on 2025/7/15.
//
#ifndef CALCULATOR_H
#define CALCULATOR_H

#include <stdio.h>
#include <stdbool.h>
#include "error_handler.h"

#define MAX_EXPR_LEN 256

// 初始化计算器
void calculator_init(void);

// 处理用户输入
void calculator_process_input(const char* input);

// 获取当前结果
double calculator_get_result(void);

// 获取当前表达式
const char* calculator_get_expression(void);

// 清除计算器状态
void calculator_clear(void);

// 清除最后输入
void calculator_backspace(void);

// 设置角度单位
void calculator_set_angle_unit(bool radians);

// 历史记录管理
void calculator_show_history(void);
void calculator_clear_history(void);

#endif // CALCULATOR_H
