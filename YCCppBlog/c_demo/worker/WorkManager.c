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
}