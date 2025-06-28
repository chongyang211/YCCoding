//
// Created by 杨充 on 2025/6/28.
//

#include <iostream>
#include "Worker.h"
#include "Boss.h"
#include "Manager.h"
#include "Employee.h"
using namespace std;

#ifndef WORKERMANAGER_H
#define WORKERMANAGER_H



class WorkerManager {
public:
    //构造函数
    WorkerManager();
    //析构函数
    ~WorkerManager();
    //展示菜单
    void Show_Menu();
    //推出系统
    void exitSystem();
};

//声明，菜单输入选项
void showMainSelect();

#endif //WORKERMANAGER_H
