//
// Created by 杨充 on 2025/6/28.
//
#pragma once
#include<iostream>
#include<string>
using namespace std;


//职工抽象基类
//子类有：普通员工，经理，老板
class Worker {
public:
    int id; //职工编号
    string name;  //职工姓名
    int deptId; //职工所在部门名称编号
    //函数前面加上virtual关键字，变成虚函数，那么编译器在编译的时候就不能确定函数调用了。
    //展示个人信息
    virtual void showInfo() = 0;
    //获取岗位名称
    virtual string getDeptName() = 0;
};
