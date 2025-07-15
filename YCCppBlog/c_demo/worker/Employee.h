//
// Created by 杨充 on 2025/7/14.
//
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include <time.h>
#include "WorkerTools.h"

/*-------------------------------------------
 *  职工类层次结构
 *------------------------------------------*/

typedef struct Employee {
    // 职工编号
    int id;
    // 姓名
    char name[MAX_NAME_LEN];
    // 部门编号
    int dept_id;
    // 职责函数指针
    void (*show_duties)();
} Employee;


// 普通员工
typedef struct Worker {
    Employee base;
} Worker;

// 经理
typedef struct Manager {
    Employee base;
    int team_size;
} Manager;

// 老板
typedef struct Boss {
    Employee base;
    float company_shares;
} Boss;

//由于系统功能较多，我们用一个管理结构体（EmployeeSystem）来管理所有职工，包含一个职工数组或链表。
//考虑到职工数量不确定，我们使用动态数组（可扩容数组）。
typedef struct EmployeeSystem {
    // 动态数组
    Employee* employees[MAX_EMPLOYEES];
    // 当前职工数量
    int count;
    void (*display_menu)();
    void (*run)();
    void (*add_employee)(Employee*);
    void (*delete_employee)(int);
    void (*modify_employee)(int);
    void (*display_all)();
    void (*find_employee)();
    void (*sort_employees)();
    void (*clear_data)();
} EmployeeSystem;

void worker_duties();
void manager_duties();
void boss_duties();


