//
// Created by 杨充 on 2025/7/14.
//
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

int get_valid_int(const char *prompt, int min_val, int max_val);

void get_valid_string(const char *prompt, char *buffer, int max_len);

int confirm_action(const char *action);
