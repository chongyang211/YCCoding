//
// Created by 杨充 on 2025/6/30.
//

#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include<iostream>
using namespace std;
#include "Identity.h"

class Teacher : public Identity {
public:
    //默认构造
    Teacher();
    //有参构造 (职工编号，姓名，密码)
    Teacher(int empId, string name, string pwd);
    //菜单界面
    virtual void openMenu();
    //查看所有预约
    void showAllOrder();
    //审核预约
    void validOrder();
    //教师编号
    int empId;
};
