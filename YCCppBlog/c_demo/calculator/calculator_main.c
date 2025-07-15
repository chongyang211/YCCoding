//
// Created by 杨充 on 2025/7/15.
//
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <readline/readline.h>
#include <readline/history.h>
#include "calculator.h"

// 打印帮助信息
void print_help() {
    printf("=== 命令行计算器 ===\n");
    printf("基本操作: +, -, *, /, ^ (幂运算)\n");
    printf("括号: ( )\n");
    printf("函数: sin, cos, tan, log, sqrt\n");
    printf("常量: pi, e\n");
    printf("命令: \n");
    printf("  clear   - 清除当前计算\n");
    printf("  history - 显示历史记录\n");
    printf("  clrhis  - 清除历史记录\n");
    printf("  exit    - 退出计算器\n");
    printf("  help    - 显示此帮助\n");
    printf("示例: (3 + 4) * 2, sin(pi/2), 2^10\n");
}

// 主函数
int main() {
    calculator_init();

    printf("命令行计算器 (输入'help'获取帮助)\n");

    while (1) {
        char* input = readline("> ");
        if (!input) break;

        // 忽略空输入
        if (strlen(input) == 0) {
            free(input);
            continue;
        }

        // 处理内置命令
        if (strcmp(input, "exit") == 0) {
            free(input);
            break;
        } else if (strcmp(input, "clear") == 0) {
            calculator_clear();
            free(input);
            continue;
        } else if (strcmp(input, "history") == 0) {
            calculator_show_history();
            free(input);
            continue;
        } else if (strcmp(input, "clrhis") == 0) {
            calculator_clear_history();
            free(input);
            continue;
        } else if (strcmp(input, "help") == 0) {
            print_help();
            free(input);
            continue;
        }

        // 添加到历史记录
        add_history(input);

        // 处理计算
        calculator_process_input(input);

        // 显示结果或错误
        if (error_handler_has_error(&global_error_handler)) {
            printf("错误: %s\n", error_handler_get_message(&global_error_handler));
        } else {
            printf("结果: %.10g\n", calculator_get_result());
        }

        free(input);
    }

    return 0;
}