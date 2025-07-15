#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>
#include <unistd.h>

// 全局常量
#define MAX_STUDENTS 1000
#define MAX_TEACHERS 100
#define MAX_ORDERS 1000
#define MAX_COMPUTER_ROOMS 3
#define FILENAME_STUDENTS "students.dat"
#define FILENAME_TEACHERS "teachers.dat"
#define FILENAME_ADMIN "admin.dat"
#define FILENAME_ORDERS "orders.dat"
#define FILENAME_ROOMS "rooms.dat"

// 状态枚举
typedef enum {
    PENDING,
    APPROVED,
    REJECTED,
    CANCELLED
} OrderStatus;

// 时间段枚举
typedef enum {
    MORNING,
    AFTERNOON
} TimeSlot;

// 用户结构体
typedef struct {
    char id[20];
    char name[50];
    char password[50];
} User;

// 学生结构体
typedef struct {
    User user;
    int reservationCount;
} Student;

// 教师结构体
typedef struct {
    User user;
    int approvalCount;
} Teacher;

// 管理员结构体
typedef struct {
    User user;
} Admin;

// 机房信息结构体
typedef struct {
    int roomId;
    int capacity;
    char description[100];
} ComputerRoom;

// 预约记录结构体
typedef struct {
    int orderId;
    time_t requestTime;
    time_t reservationDate;
    int studentId;
    char studentName[50];
    int teacherId;
    char teacherName[50];
    int roomId;
    TimeSlot timeSlot;
    OrderStatus status;
    time_t processTime;
} ReservationOrder;

// 全局变量
ComputerRoom computerRooms[MAX_COMPUTER_ROOMS] = {
    {1, 20, "1号机房 --- 最大容量20人"},
    {2, 50, "2号机房 --- 最多容量50人"},
    {3, 100, "3号机房 --- 最多容量100人"}
};

Student students[MAX_STUDENTS];
Teacher teachers[MAX_TEACHERS];
Admin admin;
ReservationOrder orders[MAX_ORDERS];

int studentCount = 0;
int teacherCount = 0;
int orderCount = 0;
int currentUserId = -1;
char currentUserName[50] = "";


// 用户类型枚举
typedef enum {
    USER_NONE,
    STUDENT_USER,
    TEACHER_USER,
    ADMIN_USER
} UserType;

UserType currentUserType = USER_NONE;

// 函数声明
void saveData();
void loadData();
void initSystem();
int isValidDate(int year, int month, int day);
int isFutureDate(time_t date);
int isWeekday(time_t date);
void clearInputBuffer();
void mainMenu();
void studentMenu();
void teacherMenu();
void adminMenu();
void login();
void applyReservation();
void viewMyReservations();
void viewAllReservations();
void cancelReservation();
void reviewReservation();
void addAccount();
void viewAccounts();
void viewComputerRooms();
void clearReservations();
void logout();

// 工具函数：清空输入缓冲区
void clearInputBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

// 工具函数：验证日期有效性
int isValidDate(int year, int month, int day) {
    if (year < 1900 || month < 1 || month > 12 || day < 1 || day > 31)
        return 0;

    struct tm tm = {0};
    tm.tm_year = year - 1900;
    tm.tm_mon = month - 1;
    tm.tm_mday = day;

    time_t t = mktime(&tm);
    return (t != -1 && tm.tm_mday == day);
}

// 工具函数：检查是否为未来日期
int isFutureDate(time_t date) {
    time_t now = time(NULL);
    return (date > now);
}

// 工具函数：检查是否为工作日（周一到周五）
int isWeekday(time_t date) {
    struct tm *tm = localtime(&date);
    return (tm->tm_wday >= 1 && tm->tm_wday <= 5); // 周一到周五
}

// 主函数
int main() {
    initSystem();
    loadData();
    mainMenu();
    return 0;
}

// 初始化系统
void initSystem() {
    // 设置随机种子
    srand((unsigned int)time(NULL));

    // 创建数据文件（如果不存在）
    FILE *file;
    if ((file = fopen(FILENAME_STUDENTS, "r")) == NULL) {
        file = fopen(FILENAME_STUDENTS, "w");
        if (file) fclose(file);
    }
    // ... 类似创建其他数据文件

    printf("系统初始化完成\n");
    sleep(1);
}

// 保存数据到文件
void saveData() {
    FILE *file;

    // 保存学生数据
    file = fopen(FILENAME_STUDENTS, "wb");
    if (file) {
        fwrite(&studentCount, sizeof(int), 1, file);
        fwrite(students, sizeof(Student), studentCount, file);
        fclose(file);
    }

    // 保存教师数据
    file = fopen(FILENAME_TEACHERS, "wb");
    if (file) {
        fwrite(&teacherCount, sizeof(int), 1, file);
        fwrite(teachers, sizeof(Teacher), teacherCount, file);
        fclose(file);
    }

    // 保存管理员数据
    file = fopen(FILENAME_ADMIN, "wb");
    if (file) {
        fwrite(&admin, sizeof(Admin), 1, file);
        fclose(file);
    }

    // 保存预约数据
    file = fopen(FILENAME_ORDERS, "wb");
    if (file) {
        fwrite(&orderCount, sizeof(int), 1, file);
        fwrite(orders, sizeof(ReservationOrder), orderCount, file);
        fclose(file);
    }

    // 保存机房数据
    file = fopen(FILENAME_ROOMS, "wb");
    if (file) {
        fwrite(computerRooms, sizeof(ComputerRoom), MAX_COMPUTER_ROOMS, file);
        fclose(file);
    }
}

// 从文件加载数据
void loadData() {
    FILE *file;

    // 加载学生数据
    file = fopen(FILENAME_STUDENTS, "rb");
    if (file) {
        fread(&studentCount, sizeof(int), 1, file);
        fread(students, sizeof(Student), studentCount, file);
        fclose(file);
    }

    // 加载教师数据
    file = fopen(FILENAME_TEACHERS, "rb");
    if (file) {
        fread(&teacherCount, sizeof(int), 1, file);
        fread(teachers, sizeof(Teacher), teacherCount, file);
        fclose(file);
    }

    // 加载管理员数据
    file = fopen(FILENAME_ADMIN, "rb");
    if (file) {
        fread(&admin, sizeof(Admin), 1, file);
        fclose(file);
    }

    // 加载预约数据
    file = fopen(FILENAME_ORDERS, "rb");
    if (file) {
        fread(&orderCount, sizeof(int), 1, file);
        fread(orders, sizeof(ReservationOrder), orderCount, file);
        fclose(file);
    }

    // 加载机房数据
    file = fopen(FILENAME_ROOMS, "rb");
    if (file) {
        fread(computerRooms, sizeof(ComputerRoom), MAX_COMPUTER_ROOMS, file);
        fclose(file);
    }

    // 如果没有管理员，创建默认管理员
    if (strlen(admin.user.id) == 0) {
        strcpy(admin.user.id, "admin");
        strcpy(admin.user.name, "系统管理员");
        strcpy(admin.user.password, "admin123");
        saveData();
    }

    printf("数据加载成功\n");
    sleep(1);
}


// 登录函数
void login(UserType userType) {
    char id[20], password[50], name[50];
    int found = 0;

    system("clear");
    printf("\n============== %s登录 ==============\n",
           userType == STUDENT_USER ? "学生" :
           userType == TEACHER_USER ? "教师" : "管理员");

    // 获取用户输入
    printf("\n请输入%s: ",
          userType == STUDENT_USER ? "学号" :
          userType == TEACHER_USER ? "职工号" : "管理员账号");
    scanf("%s", id);
    printf("请输入姓名: ");
    scanf("%s", name);
    printf("请输入密码: ");
    scanf("%s", password);

    // 验证用户
    switch(userType) {
        case STUDENT_USER:
            for (int i = 0; i < studentCount; i++) {
                if (strcmp(students[i].user.id, id) == 0 &&
                    strcmp(students[i].user.name, name) == 0 &&
                    strcmp(students[i].user.password, password) == 0) {
                    currentUserId = i;
                    strcpy(currentUserName, name);
                    currentUserType = STUDENT_USER;
                    found = 1;
                    studentMenu();
                    break;
                }
            }
            break;
        case TEACHER_USER:
            for (int i = 0; i < teacherCount; i++) {
                if (strcmp(teachers[i].user.id, id) == 0 &&
                    strcmp(teachers[i].user.name, name) == 0 &&
                    strcmp(teachers[i].user.password, password) == 0) {
                    currentUserId = i;
                    strcpy(currentUserName, name);
                    currentUserType = TEACHER_USER;
                    found = 1;
                    teacherMenu();
                    break;
                }
            }
            break;
        case ADMIN_USER:
            if (strcmp(admin.user.id, id) == 0 &&
                strcmp(admin.user.name, name) == 0 &&
                strcmp(admin.user.password, password) == 0) {
                strcpy(currentUserName, name);
                currentUserType = ADMIN_USER;
                found = 1;
                adminMenu();
            }
            break;
        default:
            break;
    }

    if (!found) {
        printf("\n登录失败，用户名或密码错误\n");
        sleep(1);
    }
}


// 主菜单
void mainMenu() {
    int choice;
    do {
        system("clear");
        printf("\n============== 机房预约系统 ==============\n");
        printf("\n当前用户: %s\n", currentUserName);
        printf("\n1. 学生登录\n");
        printf("2. 教师登录\n");
        printf("3. 管理员登录\n");
        printf("0. 退出系统\n");
        printf("\n=========================================\n");
        printf("请选择操作: ");

        if (scanf("%d", &choice) != 1) {
            clearInputBuffer();
            choice = -1;
        }

        switch(choice) {
            case 1:
                login(STUDENT_USER);
                break;
            case 2:
                login(TEACHER_USER);
                break;
            case 3:
                login(ADMIN_USER);
                break;
            case 0:
                saveData();
                printf("\n感谢使用，再见！\n");
                exit(0);
            default:
                printf("无效选择，请重新输入\n");
                sleep(1);
        }
    } while (choice != 0);
}

// 学生菜单
void studentMenu() {
    int choice;
    do {
        system("clear");
        printf("\n============== 学生菜单 ==============\n");
        printf("欢迎，%s同学\n", currentUserName);
        printf("\n1. 申请预约\n");
        printf("2. 查看我的预约\n");
        printf("3. 查看所有预约\n");
        printf("4. 取消预约\n");
        printf("0. 注销登录\n");
        printf("\n=====================================\n");
        printf("请选择操作: ");

        if (scanf("%d", &choice) != 1) {
            clearInputBuffer();
            choice = -1;
        }

        switch(choice) {
            case 1:
                applyReservation();
                break;
            case 2:
                viewMyReservations();
                break;
            case 3:
                viewAllReservations();
                break;
            case 4:
                cancelReservation();
                break;
            case 0:
                logout();
                break;
            default:
                printf("无效选择，请重新输入\n");
                sleep(1);
        }
    } while (choice != 0);
}

// 教师菜单
void teacherMenu() {
    int choice;
    do {
        system("clear");
        printf("\n============== 教师菜单 ==============\n");
        printf("欢迎，%s老师\n", currentUserName);
        printf("\n1. 查看所有预约\n");
        printf("2. 审核预约\n");
        printf("0. 注销登录\n");
        printf("\n=====================================\n");
        printf("请选择操作: ");

        if (scanf("%d", &choice) != 1) {
            clearInputBuffer();
            choice = -1;
        }

        switch(choice) {
            case 1:
                viewAllReservations();
                break;
            case 2:
                reviewReservation();
                break;
            case 0:
                logout();
                break;
            default:
                printf("无效选择，请重新输入\n");
                sleep(1);
        }
    } while (choice != 0);
}

// 管理员菜单
void adminMenu() {
    int choice;
    do {
        system("clear");
        printf("\n============== 管理员菜单 ==============\n");
        printf("欢迎，%s\n", currentUserName);
        printf("\n1. 添加账号\n");
        printf("2. 查看账号\n");
        printf("3. 查看机房\n");
        printf("4. 清空预约\n");
        printf("0. 注销登录\n");
        printf("\n=======================================\n");
        printf("请选择操作: ");

        if (scanf("%d", &choice) != 1) {
            clearInputBuffer();
            choice = -1;
        }

        switch(choice) {
            case 1:
                addAccount();
                break;
            case 2:
                viewAccounts();
                break;
            case 3:
                viewComputerRooms();
                break;
            case 4:
                clearReservations();
                break;
            case 0:
                logout();
                break;
            default:
                printf("无效选择，请重新输入\n");
                sleep(1);
        }
    } while (choice != 0);
}

// 申请预约（学生功能）
void applyReservation() {
    if (currentUserType != STUDENT_USER) return;

    system("clear");
    printf("\n============== 机房预约申请 ==============\n");

    // 获取日期输入
    printf("\n请选择预约日期 (YYYY MM DD): ");
    int year, month, day;
    if (scanf("%d %d %d", &year, &month, &day) != 3 || !isValidDate(year, month, day)) {
        printf("无效日期格式\n");
        clearInputBuffer();
        sleep(1);
        return;
    }

    // 创建日期时间
    struct tm tm = {0};
    tm.tm_year = year - 1900;
    tm.tm_mon = month - 1;
    tm.tm_mday = day;
    time_t date = mktime(&tm);

    // 验证日期
    if (!isFutureDate(date) || !isWeekday(date)) {
        printf("只能预约未来周一到周五的日期\n");
        sleep(1);
        return;
    }

    // 显示可预约机房
    printf("\n可选机房:\n");
    for (int i = 0; i < MAX_COMPUTER_ROOMS; i++) {
        printf("%d. %s\n", computerRooms[i].roomId, computerRooms[i].description);
    }

    // 选择机房
    int roomId;
    printf("\n请选择机房号: ");
    scanf("%d", &roomId);

    if (roomId < 1 || roomId > MAX_COMPUTER_ROOMS) {
        printf("无效机房号\n");
        sleep(1);
        return;
    }

    // 选择时间段
    printf("\n请选择时间段:\n");
    printf("1. 上午 (8:00-12:00)\n");
    printf("2. 下午 (14:00-18:00)\n");
    printf("请选择: ");

    int timeChoice;
    scanf("%d", &timeChoice);
    TimeSlot slot = (timeChoice == 1) ? MORNING : AFTERNOON;

    // 检查冲突预约
    for (int i = 0; i < orderCount; i++) {
        if (orders[i].reservationDate == date &&
            orders[i].roomId == roomId &&
            orders[i].timeSlot == slot &&
            orders[i].status != CANCELLED) {
            printf("该时间段已被预约，请选择其他时间\n");
            sleep(1);
            return;
        }
    }

    // 创建预约
    ReservationOrder newOrder;
    newOrder.orderId = orderCount + 1;
    newOrder.requestTime = time(NULL);
    newOrder.reservationDate = date;
    newOrder.roomId = roomId;
    newOrder.timeSlot = slot;
    newOrder.studentId = currentUserId;
    strcpy(newOrder.studentName, currentUserName);
    newOrder.teacherId = -1; // 尚未分配教师
    newOrder.status = PENDING;
    newOrder.processTime = 0;

    orders[orderCount] = newOrder;
    orderCount++;
    saveData();

    printf("\n预约申请已提交! 申请号: %d\n", newOrder.orderId);
    printf("请等待教师审核...\n");
    sleep(2);
}

// 查看我的预约（学生功能）
void viewMyReservations() {
    if (currentUserType != STUDENT_USER) return;

    system("clear");
    printf("\n============== 我的预约记录 ==============\n");
    printf("\n%-6s %-15s %-15s %-8s %-12s %-10s\n",
           "单号", "日期", "机房", "时段", "审核教师", "状态");

    const  char *statusStr[] = {"审核中", "已通过", "已拒绝", "已取消"};
    const  char *slotStr[] = {"上午", "下午"};
    int count = 0;

    for (int i = 0; i < orderCount; i++) {
        if (orders[i].studentId == currentUserId) {
            struct tm *tm = localtime(&orders[i].reservationDate);
            char dateStr[20];
            strftime(dateStr, sizeof(dateStr), "%Y-%m-%d", tm);

            printf("%-6d %-15s %d号机房\t %-8s %-12s %-10s\n",
                  orders[i].orderId, dateStr, orders[i].roomId,
                  slotStr[orders[i].timeSlot],
                  orders[i].teacherId == -1 ? "尚未分配" : orders[i].teacherName,
                  statusStr[orders[i].status]);
            count++;
        }
    }

    if (count == 0) {
        printf("\n暂无预约记录\n");
    }

    printf("\n按任意键返回...");
    getchar();
    getchar();
}

// 取消预约（学生功能）
void cancelReservation() {
    if (currentUserType != STUDENT_USER) return;

    system("clear");
    printf("\n============== 取消预约 ==============\n");

    int orderId;
    printf("\n请输入要取消的预约单号: ");
    scanf("%d", &orderId);

    int found = 0;
    for (int i = 0; i < orderCount; i++) {
        if (orders[i].orderId == orderId && orders[i].studentId == currentUserId) {
            if (orders[i].status == APPROVED || orders[i].status == PENDING) {
                orders[i].status = CANCELLED;
                orders[i].processTime = time(NULL);
                saveData();
                printf("预约已取消\n");
                found = 1;
                break;
            } else {
                printf("该预约无法取消\n");
                found = 1;
                break;
            }
        }
    }

    if (!found) {
        printf("未找到您的该预约记录\n");
    }

    sleep(1);
}

// 查看所有预约（学生/教师功能）
void viewAllReservations() {
    if (currentUserType == USER_NONE) return;

    system("clear");
    printf("\n============== 所有预约记录 ==============\n");
    printf("\n%-6s %-10s %-15s %-15s %-8s %-12s %-10s\n",
           "单号", "学生", "日期", "机房", "时段", "审核教师", "状态");

    const char *statusStr[] = {"审核中", "已通过", "已拒绝", "已取消"};
    const char *slotStr[] = {"上午", "下午"};
    int count = 0;

    for (int i = 0; i < orderCount; i++) {
        if (currentUserType == STUDENT_USER && orders[i].studentId != currentUserId)
            continue;

        struct tm *tm = localtime(&orders[i].reservationDate);
        char dateStr[20];
        strftime(dateStr, sizeof(dateStr), "%Y-%m-%d", tm);

        printf("%-6d %-10s %-15s %d号机房\t %-8s %-12s %-10s\n",
              orders[i].orderId, orders[i].studentName, dateStr,
              orders[i].roomId,
              slotStr[orders[i].timeSlot],
              orders[i].teacherId == -1 ? "尚未分配" : orders[i].teacherName,
              statusStr[orders[i].status]);
        count++;
    }

    if (count == 0) {
        printf("\n暂无预约记录\n");
    }

    printf("\n按任意键返回...");
    getchar();
    getchar();
}

// 审核预约（教师功能）
void reviewReservation() {
    if (currentUserType != TEACHER_USER) return;

    system("clear");
    printf("\n============== 审核预约申请 ==============\n");

    // 显示待审核预约
    printf("\n%-6s %-10s %-15s %-15s %-8s\n",
           "单号", "学生", "日期", "机房", "时段");

    const char *slotStr[] = {"上午", "下午"};
    int pendingCount = 0;
    int pendingOrders[MAX_ORDERS];

    for (int i = 0; i < orderCount; i++) {
        if (orders[i].status == PENDING) {
            struct tm *tm = localtime(&orders[i].reservationDate);
            char dateStr[20];
            strftime(dateStr, sizeof(dateStr), "%Y-%m-%d", tm);

            printf("%-6d %-10s %-15s %d号机房\t %-8s\n",
                  orders[i].orderId, orders[i].studentName, dateStr,
                  orders[i].roomId, slotStr[orders[i].timeSlot]);

            pendingOrders[pendingCount++] = i;
        }
    }

    if (pendingCount == 0) {
        printf("\n暂无待审核预约\n");
        printf("\n按任意键返回...");
        getchar();
        getchar();
        return;
    }

    int orderId;
    printf("\n请输入要审核的预约单号: ");
    scanf("%d", &orderId);

    int found = 0;
    for (int i = 0; i < pendingCount; i++) {
        int idx = pendingOrders[i];
        if (orders[idx].orderId == orderId) {
            found = 1;

            printf("\n审核预约单: %d\n", orderId);
            printf("学生: %s\n", orders[idx].studentName);

            struct tm *tm = localtime(&orders[idx].reservationDate);
            char dateStr[20];
            strftime(dateStr, sizeof(dateStr), "%Y-%m-%d", tm);
            printf("日期: %s\n", dateStr);

            printf("机房: %d号机房\n", orders[idx].roomId);
            printf("时段: %s\n", slotStr[orders[idx].timeSlot]);

            int decision;
            printf("\n请决定: (1=通过, 2=拒绝): ");
            scanf("%d", &decision);

            if (decision == 1) {
                orders[idx].status = APPROVED;
                // 检查机房容量是否足够
                int count = 0;
                for (int j = 0; j < orderCount; j++) {
                    if (j != idx &&
                        orders[j].reservationDate == orders[idx].reservationDate &&
                        orders[j].roomId == orders[idx].roomId &&
                        orders[j].timeSlot == orders[idx].timeSlot &&
                        orders[j].status == APPROVED) {
                        count++;
                    }
                }

                if (count >= computerRooms[orders[idx].roomId-1].capacity) {
                    printf("机房在该时段预约已达上限，无法通过\n");
                    sleep(1);
                    return;
                }

                printf("已批准预约\n");
            } else if (decision == 2) {
                orders[idx].status = REJECTED;
                printf("已拒绝预约\n");
            } else {
                printf("无效选择，取消操作\n");
                sleep(1);
                return;
            }

            orders[idx].teacherId = currentUserId;
            strcpy(orders[idx].teacherName, currentUserName);
            orders[idx].processTime = time(NULL);
            saveData();
            break;
        }
    }

    if (!found) {
        printf("未找到该预约或该预约不需要审核\n");
    }

    sleep(1);
}

// 添加账号（管理员功能）
void addAccount() {
    if (currentUserType != ADMIN_USER) return;

    system("clear");
    printf("\n============== 添加账号 ==============\n");

    int accountType;
    printf("\n添加账号类型:\n");
    printf("1. 学生\n");
    printf("2. 教师\n");
    printf("请选择: ");
    scanf("%d", &accountType);

    if (accountType != 1 && accountType != 2) {
        printf("无效选择\n");
        sleep(1);
        return;
    }

    // 检查ID重复
    char id[20];
    printf("\n请输入%s: ", accountType == 1 ? "学号" : "职工号");
    scanf("%s", id);

    if (accountType == 1) {
        for (int i = 0; i < studentCount; i++) {
            if (strcmp(students[i].user.id, id) == 0) {
                printf("该学号已存在\n");
                sleep(1);
                return;
            }
        }
    } else {
        for (int i = 0; i < teacherCount; i++) {
            if (strcmp(teachers[i].user.id, id) == 0) {
                printf("该职工号已存在\n");
                sleep(1);
                return;
            }
        }
    }

    // 创建新用户
    User newUser;
    strcpy(newUser.id, id);
    printf("请输入姓名: ");
    scanf("%s", newUser.name);
    printf("请输入密码: ");
    scanf("%s", newUser.password);

    if (accountType == 1) {
        students[studentCount].user = newUser;
        students[studentCount].reservationCount = 0;
        studentCount++;
        printf("\n学生账号添加成功! 学号: %s\n", newUser.id);
    } else {
        teachers[teacherCount].user = newUser;
        teachers[teacherCount].approvalCount = 0;
        teacherCount++;
        printf("\n教师账号添加成功! 职工号: %s\n", newUser.id);
    }

    saveData();
    sleep(1);
}

// 查看账号（管理员功能）
void viewAccounts() {
    if (currentUserType != ADMIN_USER) return;

    system("clear");
    printf("\n============== 查看账号 ==============\n");

    int accountType;
    printf("\n查看账号类型:\n");
    printf("1. 学生\n");
    printf("2. 教师\n");
    printf("3. 管理员\n");
    printf("请选择: ");
    scanf("%d", &accountType);

    switch(accountType) {
        case 1: // 学生
            printf("\n%-15s %-10s %-15s %-5s\n", "学号", "姓名", "密码", "预约数");
            for (int i = 0; i < studentCount; i++) {
                printf("%-15s %-10s %-15s %-5d\n",
                      students[i].user.id,
                      students[i].user.name,
                      "********", // 安全考虑不显示真实密码
                      students[i].reservationCount);
            }
            break;
        case 2: // 教师
            printf("\n%-15s %-10s %-15s %-5s\n", "职工号", "姓名", "密码", "审核数");
            for (int i = 0; i < teacherCount; i++) {
                printf("%-15s %-10s %-15s %-5d\n",
                      teachers[i].user.id,
                      teachers[i].user.name,
                      "********",
                      teachers[i].approvalCount);
            }
            break;
        case 3: // 管理员
            printf("\n%-15s %-10s %-15s\n", "管理员账号", "姓名", "密码");
            printf("%-15s %-10s %-15s\n",
                  admin.user.id,
                  admin.user.name,
                  "********");
            break;
        default:
            printf("无效选择\n");
            sleep(1);
            return;
    }

    printf("\n按任意键返回...");
    getchar();
    getchar();
}

// 查看机房（管理员功能）
void viewComputerRooms() {
    if (currentUserType != ADMIN_USER) return;

    system("clear");
    printf("\n============== 机房信息 ==============\n");
    printf("\n%-5s %-10s %s\n", "编号", "容量", "描述");

    for (int i = 0; i < MAX_COMPUTER_ROOMS; i++) {
        printf("%-5d %-10d %s\n",
              computerRooms[i].roomId,
              computerRooms[i].capacity,
              computerRooms[i].description);
    }

    printf("\n按任意键返回...");
    getchar();
    getchar();
}

// 清空预约（管理员功能）
void clearReservations() {
    if (currentUserType != ADMIN_USER) return;

    system("clear");
    printf("\n============== 清空预约记录 ==============\n");

    char confirm;
    printf("\n警告：这将删除所有预约记录，确定吗？(y/n): ");
    scanf(" %c", &confirm);

    if (tolower(confirm) == 'y') {
        orderCount = 0;

        // 重置学生预约计数
        for (int i = 0; i < studentCount; i++) {
            students[i].reservationCount = 0;
        }

        // 重置教师审核计数
        for (int i = 0; i < teacherCount; i++) {
            teachers[i].approvalCount = 0;
        }

        saveData();
        printf("所有预约记录已清空\n");
    } else {
        printf("操作已取消\n");
    }

    sleep(1);
}

// 注销登录
void logout() {
    currentUserId = -1;
    currentUserName[0] = '\0';
    currentUserType = USER_NONE;
    printf("注销成功\n");
    sleep(1);
}