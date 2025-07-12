//
// Created by 杨充 on 2025/7/12.
//

#ifndef STUDENTCMANAGER_H
#define STUDENTCMANAGER_H
#include <stdio.h>
#include <stdlib.h>
#include <stdio.h>
#include<string.h>		//strlen()

//定义一个学生结构体
typedef struct tagStudent {
    char szName[20];	//姓名
    char szSex[4];		//性别
    int  nAge;			//年龄
    int  nStuNo;		//学号
    int  nScore;		//成绩
} Student;



//菜单展示
void Menu();

#endif //STUDENTCMANAGER_H
