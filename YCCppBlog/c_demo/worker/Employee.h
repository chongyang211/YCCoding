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
    int id;
    char name[MAX_NAME_LEN];
    int dept_id;
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

typedef struct EmployeeSystem {
    Employee* employees[MAX_EMPLOYEES];
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


