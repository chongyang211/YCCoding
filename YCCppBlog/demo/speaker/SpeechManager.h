//
// Created by 杨充 on 2025/6/28.
//
#pragma once
#include<iostream>
#include <map>
#include <algorithm>
#include <random>
#include <vector>
#include "Speaker.h"

using namespace std;

#ifndef SPEECHMANAGER_H
#define SPEECHMANAGER_H


//演讲管理类
class SpeechManager {
public:
    //比赛选手 容器  12人
    vector<int>v1;
    //第一轮晋级容器  6人
    vector<int>v2;
    //胜利前三名容器  3人
    vector<int> victory;
    //存放编号 以及对应的 具体选手 容器
    map<int, Speaker> speaker;
    //比赛轮次
    int index;
    //构造函数
    SpeechManager();
    //析构函数
    ~SpeechManager();
    //展示菜单
    void showMenu();
    //推出
    void exitSystem();
    //初始化属性
    void initSpeech();
    //初始化创建12名选手
    void createSpeaker();
    //开始比赛-比赛流程控制
    void startSpeech();
    //抽签
    void speechDraw();
    //比赛
    void speechContest();
};



#endif //SPEECHMANAGER_H
