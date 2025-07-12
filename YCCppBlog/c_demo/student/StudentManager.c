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
}
