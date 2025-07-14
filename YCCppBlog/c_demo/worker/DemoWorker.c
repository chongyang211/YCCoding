//
// Created by 杨充 on 2025/7/14.
//
//
// Created by 杨充 on 2025/7/14.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include <time.h>

#include "WorkerLogger.h"
#include "WorkManager.h"
#include "WorkerError.h"
#include "WorkerValid.h"

void worker_duties() {
    printf("职责：完成经理交给的任务\n");
}

void manager_duties() {
    printf("职责：完成老板交给的任务\n");
}

void boss_duties() {
    printf("职责：管理公司所有事务\n");
}

/*-------------------------------------------
 *  日志记录系统
 *------------------------------------------*/

void log_event(const char* message) {
    FILE* log_file = fopen(LOG_FILE, "a");
    if (!log_file) return;

    time_t now = time(NULL);
    char* time_str = ctime(&now);
    time_str[strlen(time_str)-1] = '\0'; // Remove newline

    fprintf(log_file, "[%s] %s\n", time_str, message);
    fclose(log_file);
}


/*-------------------------------------------
 *  异常处理系统（150行）
 *------------------------------------------*/

void handle_error(ErrorCode code, const char* context) {
    char log_msg[512];
    snprintf(log_msg, sizeof(log_msg), "ERROR [%d:%s] - %s",
             code, error_messages[code], context);
    log_event(log_msg);
}

/*-------------------------------------------
 *  职工工厂系统（150行）
 *------------------------------------------*/
Employee* create_worker(int id, const char* name, int dept_id) {
    Worker* w = (Worker*)malloc(sizeof(Worker));
    if (!w) {
        handle_error(ERR_MEMORY, "创建普通员工失败");
        return NULL;
    }

    w->base.id = id;
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

Employee* create_boss(int id, const char* name, int dept_id, float shares) {
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

/*-------------------------------------------
 *  数据存储系统（400行）
 *------------------------------------------*/
typedef struct EmployeeRecord {
    int id;
    char name[MAX_NAME_LEN];
    int dept_id;
    int emp_type;  // 1=普通员工 2=经理 3=老板
    union {
        struct {
            int team_size;
        } manager;
        struct {
            float company_shares;
        } boss;
    } spec;
} EmployeeRecord;

void serialize_employee(Employee* emp, EmployeeRecord* record) {
    record->id = emp->id;
    strcpy(record->name, emp->name);
    record->dept_id = emp->dept_id;

    if (emp->show_duties == worker_duties) {
        record->emp_type = 1;
    }
    else if (emp->show_duties == manager_duties) {
        record->emp_type = 2;
        Manager* m = (Manager*)emp;
        record->spec.manager.team_size = m->team_size;
    }
    else if (emp->show_duties == boss_duties) {
        record->emp_type = 3;
        Boss* b = (Boss*)emp;
        record->spec.boss.company_shares = b->company_shares;
    }
}

Employee* deserialize_record(EmployeeRecord* record) {
    switch (record->emp_type) {
        case 1:
            return create_worker(record->id, record->name, record->dept_id);
        case 2:
            return create_manager(record->id, record->name, record->dept_id,
                                record->spec.manager.team_size);
        case 3:
            return create_boss(record->id, record->name, record->dept_id,
                             record->spec.boss.company_shares);
        default:
            handle_error(ERR_DATA_CORRUPT, "未知的职工类型");
            return NULL;
    }
}

int save_to_file(Employee* employees[], int count) {
    FILE* fp = fopen(DATA_FILE, "wb");
    if (!fp) {
        handle_error(ERR_FILE_OPEN, "保存数据");
        return -1;
    }

    // 写入魔数标识和版本
    const unsigned int MAGIC = 0x454D504C; // 'EMPL'
    const unsigned short VERSION = 0x0100;
    fwrite(&MAGIC, sizeof(MAGIC), 1, fp);
    fwrite(&VERSION, sizeof(VERSION), 1, fp);

    // 写入记录数
    fwrite(&count, sizeof(count), 1, fp);

    // 写入记录
    for (int i = 0; i < count; i++) {
        EmployeeRecord record;
        serialize_employee(employees[i], &record);
        fwrite(&record, sizeof(record), 1, fp);
    }

    // 计算并写入校验和
    long file_size = ftell(fp);
    fseek(fp, 0, SEEK_SET);
    unsigned char checksum = 0;
    for (long i = 0; i < file_size; i++) {
        checksum ^= fgetc(fp);
    }
    fputc(checksum, fp);

    fclose(fp);
    log_event("职工数据已保存到文件");
    return count;
}

int load_from_file(Employee* employees[]) {
    FILE* fp = fopen(DATA_FILE, "rb");
    if (!fp) {
        handle_error(ERR_FILE_OPEN, "加载数据");
        return -1;
    }

    // 校验魔数
    unsigned int magic;
    fread(&magic, sizeof(magic), 1, fp);
    if (magic != 0x454D504C) {
        handle_error(ERR_DATA_CORRUPT, "无效的文件格式");
        fclose(fp);
        return -1;
    }

    // 校验版本
    unsigned short version;
    fread(&version, sizeof(version), 1, fp);
    if (version != 0x0100) {
        handle_error(ERR_DATA_CORRUPT, "不支持的版本");
        fclose(fp);
        return -1;
    }

    // 读取记录数
    int count;
    fread(&count, sizeof(count), 1, fp);

    if (count > MAX_EMPLOYEES) {
        handle_error(ERR_DATA_CORRUPT, "记录数超过最大值");
        fclose(fp);
        return -1;
    }

    // 计算校验和
    long file_size;
    fseek(fp, 0, SEEK_END);
    file_size = ftell(fp);
    fseek(fp, 0, SEEK_SET);

    unsigned char checksum = 0;
    for (long i = 0; i < file_size - 1; i++) {
        checksum ^= fgetc(fp);
    }

    // 验证校验和
    unsigned char file_checksum = fgetc(fp);
    if (checksum != file_checksum) {
        handle_error(ERR_DATA_CORRUPT, "校验和错误");
        fclose(fp);
        return -1;
    }

    // 重定位到数据开始
    fseek(fp, sizeof(magic) + sizeof(version) + sizeof(count), SEEK_SET);

    // 读取记录
    int loaded = 0;
    for (int i = 0; i < count; i++) {
        EmployeeRecord record;
        if (fread(&record, sizeof(record), 1, fp) != 1) {
            handle_error(ERR_DATA_CORRUPT, "读取记录失败");
            break;
        }

        employees[i] = deserialize_record(&record);
        if (employees[i]) {
            loaded++;
        }
    }

    fclose(fp);
    log_event("职工数据已从文件加载");
    return loaded;
}

/*-------------------------------------------
 *  用户输入验证系统（200行）
 *------------------------------------------*/

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

/*-------------------------------------------
 *  职工管理系统核心（500行）
 *------------------------------------------*/

// 全局系统实例
EmployeeSystem* this_system;

// 创建系统实例
EmployeeSystem* create_employee_system() {
    EmployeeSystem* sys = malloc(sizeof(EmployeeSystem));
    if (!sys) {
        handle_error(ERR_MEMORY, "创建系统");
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

// 菜单显示
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

// 添加职工
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

// 删除职工
void system_delete_employee(int id) {
    for (int i = 0; i < this_system->count; i++) {
        if (this_system->employees[i]->id == id) {
            char name[MAX_NAME_LEN];
            strcpy(name, this_system->employees[i]->name);

            free(this_system->employees[i]);

            // 移动数组元素
            for (int j = i; j < this_system->count - 1; j++) {
                this_system->employees[j] = this_system->employees[j+1];
            }

            this_system->count--;

            char log_msg[100];
            snprintf(log_msg, sizeof(log_msg), "删除职工: %s (ID: %d)", name, id);
            log_event(log_msg);
            return;
        }
    }

    handle_error(ERR_ID_NOT_FOUND, "删除职工");
    printf("错误: 未找到ID为 %d 的职工\n", id);
}

// 修改职工
void system_modify_employee(int id) {
    for (int i = 0; i < this_system->count; i++) {
        if (this_system->employees[i]->id == id) {
            Employee* emp = this_system->employees[i];

            printf("\n当前信息:\n");
            printf("姓名: %s\n部门ID: %d\n", emp->name, emp->dept_id);
            emp->show_duties();

            // 获取新信息
            char new_name[MAX_NAME_LEN];
            get_valid_string("输入新姓名", new_name, MAX_NAME_LEN);
            strcpy(emp->name, new_name);

            int new_dept = get_valid_int("输入新部门ID", 1, 100);
            emp->dept_id = new_dept;

            // 如果是经理或老板需要特殊处理
            if (emp->show_duties == manager_duties) {
                Manager* m = (Manager*)emp;
                m->team_size = get_valid_int("输入新的团队人数", 1, 100);
            } else if (emp->show_duties == boss_duties) {
                Boss* b = (Boss*)emp;
                b->company_shares = get_valid_int("输入新的公司股份", 1, 100);
            }

            char log_msg[100];
            snprintf(log_msg, sizeof(log_msg), "修改职工信息: %s (ID: %d)", new_name, id);
            log_event(log_msg);
            return;
        }
    }

    handle_error(ERR_ID_NOT_FOUND, "修改职工");
    printf("错误: 未找到ID为 %d 的职工\n", id);
}

// 显示所有职工
void system_display_all() {
    if (this_system->count == 0) {
        printf("没有职工记录\n");
        return;
    }

    printf("\n%-10s %-20s %-15s %-10s\n", "职工ID", "姓名", "部门", "职位");
    printf("================================================\n");

    for (int i = 0; i < this_system->count; i++) {
        Employee* emp = this_system->employees[i];
        char type[20];

        if (emp->show_duties == worker_duties) {
            strcpy(type, "普通员工");
        } else if (emp->show_duties == manager_duties) {
            strcpy(type, "经理");
        } else {
            strcpy(type, "老板");
        }

        printf("%-10d %-20s %-15d %-10s\n",
               emp->id, emp->name, emp->dept_id, type);
    }

    log_event("显示所有职工信息");
}

// 查找职工
void system_find_employee() {
    printf("\n查找选项:\n");
    printf("1. 按职工ID查找\n");
    printf("2. 按姓名查找\n");
    int choice = get_valid_int("请选择", 1, 2);

    if (choice == 1) {
        int id = get_valid_int("输入职工ID", MIN_ID, MAX_ID);

        for (int i = 0; i < this_system->count; i++) {
            if (this_system->employees[i]->id == id) {
                Employee* emp = this_system->employees[i];
                printf("\n职工信息:\n");
                printf("ID: %d\n姓名: %s\n部门: %d\n",
                      emp->id, emp->name, emp->dept_id);
                emp->show_duties();
                return;
            }
        }
        printf("未找到该职工\n");
    }
    else {
        char name[MAX_NAME_LEN];
        get_valid_string("输入职工姓名", name, MAX_NAME_LEN);
        int found = 0;

        for (int i = 0; i < this_system->count; i++) {
            if (strcmp(this_system->employees[i]->name, name) == 0) {
                if (!found) {
                    printf("\n查找到以下职工:\n");
                    found = 1;
                }
                Employee* emp = this_system->employees[i];
                printf("ID: %d, 姓名: %s, 部门: %d\n",
                      emp->id, emp->name, emp->dept_id);
            }
        }

        if (!found) {
            printf("未找到姓名为 %s 的职工\n", name);
        }
    }
}

// 排序功能
int compare_id_asc(const void* a, const void* b) {
    Employee* emp1 = *(Employee**)a;
    Employee* emp2 = *(Employee**)b;
    return emp1->id - emp2->id;
}

int compare_id_desc(const void* a, const void* b) {
    return compare_id_asc(b, a);
}

void system_sort_employees() {
    if (this_system->count == 0) {
        printf("没有可排序的数据\n");
        return;
    }

    printf("\n排序选项:\n");
    printf("1. 升序排序\n");
    printf("2. 降序排序\n");
    int choice = get_valid_int("请选择", 1, 2);

    if (choice == 1) {
        qsort(this_system->employees, this_system->count, sizeof(Employee*), compare_id_asc);
        printf("已按ID升序排序\n");
    } else {
        qsort(this_system->employees, this_system->count, sizeof(Employee*), compare_id_desc);
        printf("已按ID降序排序\n");
    }

    system_display_all();
}

// 清空数据
void system_clear_data() {
    if (this_system->count == 0) {
        printf("系统中无数据\n");
        return;
    }

    if (!confirm_action("清空所有职工数据")) {
        printf("操作已取消\n");
        return;
    }

    // 备份到文件
    FILE* backup = fopen(BACKUP_FILE, "wb");
    if (backup) {
        fwrite(&this_system->count, sizeof(int), 1, backup);
        for (int i = 0; i < this_system->count; i++) {
            EmployeeRecord rec;
            serialize_employee(this_system->employees[i], &rec);
            fwrite(&rec, sizeof(rec), 1, backup);
        }
        fclose(backup);
        log_event("已创建数据备份");
    }

    // 释放内存
    for (int i = 0; i < this_system->count; i++) {
        free(this_system->employees[i]);
    }
    this_system->count = 0;

    // 清空数据文件
    FILE* data = fopen(DATA_FILE, "w");
    if (data) fclose(data);

    log_event("已清空所有职工数据");
    printf("所有职工数据已清除\n");
}

// 系统主循环
void system_run() {
    int running = 1;
    while (running) {
        system_display_menu();
        int choice = get_valid_int("请选择", 0, 9);

        switch (choice) {
            case 1: {
                printf("\n添加新职工:\n");
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
                break;
            }
            case 2:
                this_system->display_all();
                break;
            case 3: {
                int id = get_valid_int("输入要删除的职工ID", MIN_ID, MAX_ID);
                this_system->delete_employee(id);
                break;
            }
            case 4: {
                int id = get_valid_int("输入要修改的职工ID", MIN_ID, MAX_ID);
                this_system->modify_employee(id);
                break;
            }
            case 5:
                this_system->find_employee();
                break;
            case 6:
                this_system->sort_employees();
                break;
            case 7:
                this_system->clear_data();
                break;
            case 8:
                if (save_to_file(this_system->employees, this_system->count) > 0) {
                    printf("数据导出成功\n");
                }
                break;
            case 9: {
                int count = load_from_file(this_system->employees);
                if (count > 0) {
                    this_system->count = count;
                    printf("成功加载 %d 条记录\n", count);
                }
                break;
            }
            case 0:
                running = 0;
                printf("系统已退出\n");
                break;
        }
    }
}

/*-------------------------------------------
 *  主函数及全局数据（100行）
 *------------------------------------------*/


int main() {
    printf("=== 企业职工管理系统 ===\n");
    printf("系统初始化中...\n");

    log_event("=== 系统启动 ===");

    this_system = create_employee_system();
    if (!this_system) {
        return 1;
    }

    // 尝试加载现有数据
    int count = load_from_file(this_system->employees);
    if (count > 0) {
        this_system->count = count;
        printf("成功加载 %d 名职工记录\n", count);
    } else {
        printf("无数据文件或加载失败\n");
    }

    // 运行主系统
    this_system->run();

    // 退出时保存数据
    save_to_file(this_system->employees, this_system->count);

    // 清理资源
    for (int i = 0; i < this_system->count; i++) {
        free(this_system->employees[i]);
    }
    free(this_system);

    log_event("=== 系统关闭 ===");
    return 0;
}