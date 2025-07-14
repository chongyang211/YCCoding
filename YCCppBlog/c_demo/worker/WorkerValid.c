//
// Created by 杨充 on 2025/7/14.
//
#include "WorkerValid.h"

int get_valid_int(const char *prompt, int min_val, int max_val) {
    int value;
    char input[50];

    while (1) {
        printf("%s [%d~%d]: ", prompt, min_val, max_val);
        fgets(input, sizeof(input), stdin);

        if (sscanf(input, "%d", &value) == 1) {
            if (value >= min_val && value <= max_val) {
                return value;
            }
        }

        printf("无效输入! 请输入%d~%d之间的整数\n", min_val, max_val);
    }
}

void get_valid_string(const char *prompt, char *buffer, int max_len) {
    while (1) {
        printf("%s: ", prompt);
        fgets(buffer, max_len, stdin);

        // 移除换行符
        size_t len = strlen(buffer);
        if (len > 0 && buffer[len - 1] == '\n') {
            buffer[len - 1] = '\0';
        }

        // 检查是否为空
        if (strlen(buffer) == 0) {
            printf("输入不能为空！请重新输入。\n");
            continue;
        }

        // 验证输入是否包含字母或空格
        int valid = 0;
        for (int i = 0; buffer[i]; i++) {
            if (isalpha(buffer[i]) || isspace(buffer[i])) {
                valid = 1;
                break;
            }
        }

        if (valid) {
            return; // 输入有效，退出循环
        } else {
            printf("无效输入！请输入至少包含一个字母或空格的字符串。\n");
        }
    }
}


int confirm_action(const char *action) {
    char response[10];
    printf("确认%s? (y/n): ", action);
    fgets(response, sizeof(response), stdin);
    return (response[0] == 'y' || response[0] == 'Y');
}
