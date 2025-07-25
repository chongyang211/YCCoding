//
// Created by 杨充 on 2025/6/30.
//
#pragma once
#include<iostream>
#include <string>
using namespace std;
#include "Identity.h"


//学生类
class Student : public Identity {
public:
    //学生学号
    int id;
    //默认构造
    Student();
    //有参构造(学号、姓名、密码)
    Student(int id, string name , string pwd);
    //菜单界面
    virtual void openMenu();
    //申请预约
    void applyOrder();
    //查看我的预约
    void showMyOrder();
    //查看所有预约
    void showAllOrder();
    //取消预约
    void cancelOrder();
};



