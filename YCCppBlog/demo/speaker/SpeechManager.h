//
// Created by 杨充 on 2025/6/28.
//
#pragma once
#include<iostream>
using namespace std;

#ifndef SPEECHMANAGER_H
#define SPEECHMANAGER_H


//演讲管理类
class SpeechManager {
public:
    //构造函数
    SpeechManager();
    //析构函数
    ~SpeechManager();
    //展示菜单
    void showMenu();
    //推出
    void exitSystem();
};



#endif //SPEECHMANAGER_H
