//
// Created by 杨充 on 2025/6/28.
//
#pragma once
#include<iostream>
#include <string>
#include "Worker.h"
using namespace std;

//创建普通员工类
//Polymorphic class 'Employee' has a non-virtual public destructor
//多态类“Employee”具有非虚拟公共析构函数
//这个警告的意思是：Employee 类是一个多态类（即它至少有一个虚函数），但它的析构函数没有声明为 virtual。
//这可能会导致派生类对象在通过基类指针删除时，派生类的析构函数不会被调用，从而引发资源泄漏或未定义行为。
class Employee : public Worker {
public:
    //构造函数
    Employee(int mId, string mName, int dId);
    //显示个人信息
    virtual void showInfo();
    //获取职工岗位名称
    virtual string getDeptName();
    //定义析构函数用于释放
    ~Employee();
};



