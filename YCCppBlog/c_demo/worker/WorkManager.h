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

void boss_duties();
void worker_duties();
void manager_duties();

// 内部函数声明
void system_display_menu();
// 系统主循环
void system_run();
// 添加职工信息
void system_add_employee(Employee* emp);
// 删除某个职工
void system_delete_employee(int id);
// 修改职工信息
void system_modify_employee(int id);
// 显示所有职工
void system_display_all();
// 查找职工信息
void system_find_employee();
// 职工信息排序
void system_sort_employees();
// 职工信息清除
void system_clear_data();

//用户验证系统


//添加新职工。包含普通员工，经理，老板
void addNewEmployee();
