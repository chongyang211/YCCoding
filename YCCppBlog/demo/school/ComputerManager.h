//
// Created by 杨充 on 2025/6/30.
//
#include <iostream>
#include <string>
#include <iostream>
#include <fstream>
#include "GlobalFile.h"
#include "Identity.h"
using namespace std;
#ifndef COMPUTERMANAGER_H
#define COMPUTERMANAGER_H



class ComputerManager {
public:
    //fileName  --- 操作的文件名
    //type      ---  登录的身份  （1代表学生、2代表老师、3代表管理员）
    void loginIn(string fileName, int type);
};



#endif //COMPUTERMANAGER_H
