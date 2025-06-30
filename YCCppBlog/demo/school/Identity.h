//
// Created by 杨充 on 2025/6/30.
//
#pragma once
#include <iostream>
using namespace std;

#ifndef IDENTITY_H
#define IDENTITY_H

//身份抽象类
class Identity {
public:
    //操作菜单
    virtual void openMenu() = 0;
    //用户名和密码
    string name;
    string pwd;
};

#endif //IDENTITY_H
