//
// Created by 杨充 on 2025/7/12.
//

#include "StudentManager.h"

// 暂停程序
void pauseProgram() {
    printf("Press Enter to continue...");
    getchar();
}

// 清屏
void clearScreen() {
    system("clear"); // 或使用 printf("\033[H\033[J");
}

int main() {
    while (1) {
        //打印菜单
        Menu();
        //读取一个字符 整数,字符在内存中映射为ASCII码
        char ch = getchar();
        printf("输入的字符是：%c \n", ch);
        switch (ch) {
            case '1': //1.录入学生信息
                printf("录入学生信息\n");
                InputStudent();
                break;
            case '2': //2.打印学生信息
                printf("打印学生信息\n");
                break;
            case '3': //3.保存学生信息
                printf("保存学生信息\n");
                break;
            case '4': //4.读取学生信息
                printf("读取学生信息\n");
                break;
            case '5': //5.统计所有学生人数
                printf("统计所有学生人数\n");
                break;
            case '6': //6.查找学生信息
                printf("查找学生信息\n");
                break;
            case '7': //7.修改学生信息
                printf("修改学生信息\n");
                break;
            case '8': //8.删除学生信息
                printf("删除学生信息\n");
                break;
            case '0': //0.退出系统
                printf("退出系统\n");
                pauseProgram();
                return 0;
                break;
            default:
                printf("输入有误，没有该功能\n\n");
                pauseProgram();
                clearScreen();
                break;
        }
    }
    return 0;
}

void Menu() {
    printf("*************************************************\n");
    printf("*\t欢迎使用高校学生成绩管理系统V1.0\t*\n");
    printf("*\t\t请选择功能\t\t\t*\n");
    printf("*************************************************\n");
    printf("*\t\t1.录入学生信息\t\t\t*\n");
    printf("*\t\t2.打印学生信息\t\t\t*\n");
    printf("*\t\t3.保存学生信息\t\t\t*\n");
    printf("*\t\t4.读取学生信息\t\t\t*\n");
    printf("*\t\t5.统计所有学生人数\t\t*\n");
    printf("*\t\t6.查找学生信息\t\t\t*\n");
    printf("*\t\t7.修改学生信息\t\t\t*\n");
    printf("*\t\t8.删除学生信息\t\t\t*\n");
    printf("*\t\t0.退出系统\t\t\t*\n");
    printf("*************************************************\n");
}


void InputStudent() {
    //创建一个人，在堆中分配内存
    Node* pNewNode = (Node*) malloc(sizeof(Node));
    //指针下一个指向空
    pNewNode->pNext = NULL;

    //查找链表的尾结点
    Node* p = g_pHead;
    //g_pHead != NULL：确保链表不为空。
    //p->pNext != NULL：确保当前节点的下一个节点不为空（即当前节点不是最后一个节点）。
    while (g_pHead != NULL && p->pNext != NULL) {
        //将指针 p 移动到下一个节点。
        p = p->pNext;
    }
    //把节点插到链表的尾节点
    if (g_pHead == NULL) {
        //无数据，则放到头部
        g_pHead = pNewNode;
    } else {
        p->pNext = pNewNode;
    }
    //录入学生信息
    printf("请输入学生姓名：\n");
    //warning: data argument not used by format string [-Wformat-extra-args]
    //这个警告是因为 scanf 的格式字符串 "%s" 只需要一个参数（即存储输入的缓冲区）
    //但你提供了两个参数（pNewNode->stu.szName 和 sizeof(pNewNode->stu.szName)）。scanf 不会使用第二个参数，因此编译器发出了警告。
    //scanf("%s", pNewNode->stu.szName, sizeof(pNewNode->stu.szName));
    scanf("%19s", pNewNode->stu.szName); // 假设 szName 的大小为 20
    // fgets(pNewNode->stu.szName, sizeof(pNewNode->stu.szName), stdin);
    // // 去掉 fgets 可能读取的换行符
    // size_t lenName = strlen(pNewNode->stu.szName);
    // if (lenName > 0 && pNewNode->stu.szName[lenName - 1] == '\n') {
    //     pNewNode->stu.szName[lenName - 1] = '\0';
    // }
    printf("请输入性别：\n");
    //scanf("%s", pNewNode->stu.szSex, sizeof(pNewNode->stu.szSex));
    scanf("%3s", pNewNode->stu.szSex);
    printf("请输入学生年龄：\n");
    scanf("%d", &pNewNode->stu.nAge);
    printf("请输入学号：\n");
    scanf("%d", &pNewNode->stu.nStuNo);
    printf("请输入成绩：\n");
    scanf("%d", &pNewNode->stu.nScore);
    printf("学生信息录入成功。\n\n");
    pauseProgram();
    clearScreen();
}
