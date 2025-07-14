//
// Created by 杨充 on 2025/7/14.
//
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include <time.h>

#ifndef WORKMANAGER_H
#define WORKMANAGER_H

void boss_duties();
void worker_duties();
void manager_duties();

// 系统主循环
void system_run();
// 内部函数声明
void system_display_menu();


#endif //WORKMANAGER_H
