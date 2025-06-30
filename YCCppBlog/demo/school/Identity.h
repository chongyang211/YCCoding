//
// Created by 杨充 on 2025/6/30.
//

#ifndef IDENTITY_H
#define IDENTITY_H

#pragma once
#include <iostream>
using namespace std;

//身份抽象类
class Identity {
public:
    //操作菜单
    //纯虚函数：在 C++ 中，纯虚函数是在基类中声明但没有实现的函数，语法为 virtual void functionName() = 0;。
    //派生类必须实现纯虚函数：如果一个类继承了抽象类，它必须实现所有纯虚函数，否则它也会被视为抽象类，无法实例化。
    virtual void openMenu() = 0;
    // virtual void openMenu() {}
    //用户名和密码
    string name;
    string pwd;
};

#endif //IDENTITY_H
