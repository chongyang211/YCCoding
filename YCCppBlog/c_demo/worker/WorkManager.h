//
// Created by 杨充 on 2025/7/14.
//
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include <time.h>
#include "Employee.h"
#ifndef WORKMANAGER_H
#define WORKMANAGER_H

void boss_duties();
void worker_duties();
void manager_duties();

// 系统主循环
void system_run();
// 内部函数声明
void system_display_menu();

//记录日志
void log_event(const char* message);

void system_display_menu();
void system_run();
void system_add_employee(Employee* emp);
void system_delete_employee(int id);
void system_modify_employee(int id);
void system_display_all();
void system_find_employee();
void system_sort_employees();
void system_clear_data();

#endif //WORKMANAGER_H
