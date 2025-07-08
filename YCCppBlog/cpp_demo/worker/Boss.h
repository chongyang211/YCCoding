//
// Created by 杨充 on 2025/6/28.
//
#pragma once
#include<iostream>
#include "Worker.h"
using namespace std;
#ifndef BOSS_H
#define BOSS_H

//老板类
class Boss : public Worker {
public:
    //构造方法
    Boss(int id, string name, int dId);
    //显示个人信息
    virtual void showInfo();
    //获取职工岗位名称
    virtual string getDeptName();
};



#endif //BOSS_H
