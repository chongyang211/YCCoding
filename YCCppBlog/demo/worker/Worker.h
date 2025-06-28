//
// Created by 杨充 on 2025/6/28.
//
#pragma once
#include<iostream>
#include<string>
using namespace std;

#ifndef WORKER_H
#define WORKER_H

//职工抽象基类
class Worker {
public:
    int id; //职工编号
    string name;  //职工姓名
    int deptId; //职工所在部门名称编号
    //展示个人信息
    virtual void showInfo() = 0;
    //获取岗位名称
    virtual string getDeptName() = 0;
};

#endif //WORKER_H
