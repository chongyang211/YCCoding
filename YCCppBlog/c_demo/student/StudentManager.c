//
// Created by 杨充 on 2025/7/12.
//

#include "StudentManager.h"

int main() {
    //打印菜单
    Menu();
    //读取一个字符 整数,字符在内存中映射为ASCII码
    char ch = getchar();
    printf("*输入的字符是：%c \n" ,ch);
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
