//
// Created by 杨充 on 2025/7/12.
//

#ifndef STUDENTCMANAGER_H
#define STUDENTCMANAGER_H
#include <stdio.h>
#include <stdlib.h>
#include <stdio.h>
#include<string.h>		//strlen()
#include <stdbool.h> // 引入布尔类型支持


//定义一个学生结构体
typedef struct tagStudent {
    char szName[20];	//姓名
    char szSex[4];		//性别
    int  nAge;			//年龄
    int  nStuNo;		//学号
    int  nScore;		//成绩
} Student;

//链表
//节点
typedef struct tagNode {
    Student stu;			//学生信息
    struct tagNode* pNext;	//指向下一个节点
} Node;

//创建头节点。这个是结构体指针
Node* g_pHead = NULL;		//指向头节点

//菜单展示
void Menu();

//录入信息
void InputStudent();

//打印学生信息
void PrintStudent();

//保存学生信息
void SaveStudent();

//读取学生信息
void ReadStudent();

//统计所有学生人数
void CountStudent();

//查找学生信息
void FindStudent();

#endif //STUDENTCMANAGER_H
