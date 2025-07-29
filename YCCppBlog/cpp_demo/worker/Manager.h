//
// Created by 杨充 on 2025/6/28.
//
#include "Worker.h"
#include <iostream>
using namespace std;
#ifndef MANAGER_H
#define MANAGER_H

//经理类
class Manager : public Worker{
public:
    Manager(int id, string name, int dId);
    //显示个人信息
    virtual void showInfo();
    //获取职工岗位名称
    virtual string getDeptName();
    //定义析构函数用于释放
    ~Manager();
};


#endif //MANAGER_H
