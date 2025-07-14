#include "WorkManager.h"
#include "WorkerTools.h"
#include "WorkerLogger.h"
#include "WorkerError.h"

// 全局系统实例
EmployeeSystem* this_system;

// 创建系统实例
EmployeeSystem* create_employee_system() {
    EmployeeSystem* sys = malloc(sizeof(EmployeeSystem));
    if (!sys) {
        // handle_error(ERR_MEMORY, "创建系统");
        return NULL;
    }
    memset(sys, 0, sizeof(EmployeeSystem));
    sys->count = 0;

    // 绑定方法
    sys->display_menu = system_display_menu;
    sys->run = system_run;
    sys->add_employee = system_add_employee;
    sys->delete_employee = system_delete_employee;
    sys->modify_employee = system_modify_employee;
    sys->display_all = system_display_all;
    sys->find_employee = system_find_employee;
    sys->sort_employees = system_sort_employees;
    sys->clear_data = system_clear_data;

    return sys;
}

void worker_duties() {
    printf("职责：完成经理交给的任务\n");
}

void manager_duties() {
    printf("职责：完成老板交给的任务\n");
}

void boss_duties() {
    printf("职责：管理公司所有事务\n");
}

void log_event(const char* message) {
    FILE* log_file = fopen(LOG_FILE, "a");
    if (!log_file) return;

    time_t now = time(NULL);
    char* time_str = ctime(&now);
    time_str[strlen(time_str)-1] = '\0'; // Remove newline

    fprintf(log_file, "[%s] %s\n", time_str, message);
    fclose(log_file);
}

void handle_error(ErrorCode code, const char* context) {
    char log_msg[512];
    snprintf(log_msg, sizeof(log_msg), "ERROR [%d:%s] - %s",
             code, error_messages[code], context);
    log_event(log_msg);
}

int main() {
    system_run();
    return 0;
}

// 内部函数声明
void system_display_menu() {
    printf("\n=== 职工管理系统 ===\n");
    printf("1. 添加职工信息\n");
    printf("2. 显示所有职工\n");
    printf("3. 删除职工\n");
    printf("4. 修改职工信息\n");
    printf("5. 查找职工\n");
    printf("6. 按职工编号排序\n");
    printf("7. 清空所有数据\n");
    printf("8. 导出数据到文件\n");
    printf("9. 从文件加载数据\n");
    printf("0. 退出系统\n");
    printf("==================\n");
}

// 系统主循环
void system_run() {
    int choice;
    int running = 1;
    while (running) {
        system_display_menu();
        scanf("%d", &choice);
        switch (choice) {
            case 1:
                printf("\n添加新职工:\n");
                addNewEmployee();
                break;
            case 2:
                printf("\n显示所有职工:\n");
                break;
            case 3:
                printf("\n删除职工:\n");
                break;
            case 4:
                printf("\n修改职工信息:\n");
                break;
            case 5:
                printf("\n查找职工:\n");
                break;
            case 6:
                printf("\n按职工编号排序:\n");
                break;
            case 7:
                printf("\n清空所有数据:\n");
                break;
            case 8:
                printf("\n导出数据到文件:\n");
                break;
            case 9: {
                printf("\n从文件加载数据:\n");
                break;
            }
            case 0:
                running = 0;
                printf("系统已退出\n");
                exit(0);
                break;
        }
    }
}

int get_valid_int(const char* prompt, int min_val, int max_val) {
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

void get_valid_string(const char* prompt, char* buffer, int max_len) {
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


int confirm_action(const char* action) {
    char response[10];
    printf("确认%s? (y/n): ", action);
    fgets(response, sizeof(response), stdin);
    return (response[0] == 'y' || response[0] == 'Y');
}

Employee* create_worker(int id, const char* name , int dept_id) {
    Worker* w = malloc(sizeof(Worker));
    if (!w) {
        handle_error(ERR_MEMORY, "创建普通员工失败");
        return NULL;
    }
    w->base.id = id;
    //w->base.name = name;
    strncpy(w->base.name, name, MAX_NAME_LEN-1);
    w->base.dept_id = dept_id;
    w->base.show_duties = worker_duties;
    return (Employee*)w;
}

Employee* create_manager(int id, const char* name, int dept_id, int team_size) {
    Manager* m = (Manager*)malloc(sizeof(Manager));
    if (!m) {
        handle_error(ERR_MEMORY, "创建经理失败");
        return NULL;
    }
    m->base.id = id;
    strncpy(m->base.name, name, MAX_NAME_LEN-1);
    m->base.dept_id = dept_id;
    m->base.show_duties = manager_duties;
    m->team_size = team_size;
    return (Employee*)m;
}

Employee* create_boss(int id, const char* name , int dept_id , float shares) {
    Boss* b = (Boss*)malloc(sizeof(Boss));
    if (!b) {
        handle_error(ERR_MEMORY, "创建老板失败");
        return NULL;
    }
    b->base.id = id;
    strncpy(b->base.name, name, MAX_NAME_LEN-1);
    b->base.dept_id = dept_id;
    b->base.show_duties = boss_duties;
    b->company_shares = shares;
    return (Employee*)b;
}

//添加新职工
void addNewEmployee() {
    printf("选择职位类型:\n");
    printf("1. 普通员工\n2. 经理\n3. 老板\n");
    int type = get_valid_int("请选择", 1, 3);
    int id = get_valid_int("输入职工ID", MIN_ID, MAX_ID);
    char name[MAX_NAME_LEN];
    get_valid_string("输入姓名", name, MAX_NAME_LEN);
    int dept = get_valid_int("输入部门ID", 1, 100);
    Employee* new_emp = NULL;
    if (type == 1) {
        new_emp = create_worker(id, name, dept);
    } else if (type == 2) {
        int team = get_valid_int("输入团队人数", 1, 100);
        new_emp = create_manager(id, name, dept, team);
    } else {
        int shares = get_valid_int("输入公司股份比例", 1, 100);
        new_emp = create_boss(id, name, dept, shares);
    }
    if (new_emp) {
        this_system->add_employee(new_emp);
    }
}

void system_add_employee(Employee* emp) {
    if (!emp) return;
    // 检查ID是否已存在
    for (int i = 0; i < this_system->count; i++) {
        if (this_system->employees[i]->id == emp->id) {
            handle_error(ERR_ID_EXISTS, "添加职工");
            printf("错误: ID %d 已存在!\n", emp->id);
            free(emp);
            return;
        }
    }
    if (this_system->count >= MAX_EMPLOYEES) {
        handle_error(ERR_LIST_FULL, "添加职工");
        printf("错误: 职工列表已满!\n");
        free(emp);
        return;
    }
    this_system->employees[this_system->count++] = emp;
    char log_msg[100];
    snprintf(log_msg, sizeof(log_msg), "添加职工: %s (ID: %d)", emp->name, emp->id);
    log_event(log_msg);
}

void system_delete_employee(int id) {

}
void system_modify_employee(int id) {

}
void system_display_all() {

}
void system_find_employee() {

}
void system_sort_employees() {

}
void system_clear_data() {

}