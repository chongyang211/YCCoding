//
// Created by 杨充 on 2025/6/28.
//
#pragma once
#include<iostream>
#include "Worker.h"
using namespace std;

//创建普通员工类
class Employee : public Worker {
public:
    //构造函数
    Employee(int id, string name, int dId);
    //显示个人信息
    virtual void showInfo();
    //获取职工岗位名称
    virtual string getDeptName();
};



