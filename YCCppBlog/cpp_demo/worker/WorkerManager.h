//
// Created by 杨充 on 2025/6/28.
//

#include <iostream>
#include "Worker.h"
#include "Boss.h"
#include "Manager.h"
#include "Employee.h"
#include <fstream>
using namespace std;

#ifndef WORKERMANAGER_H
#define WORKERMANAGER_H

//定义一个文件名称
#define  FILENAME "empFile.txt"

class WorkerManager {
public:
    //记录文件中的人数个数
    int empNum;
    //员工数组的指针。思考，什么是数组指针？？？
    Worker ** empArray;
    //标志文件是否为空
    bool fileIsEmpty;
public:
    //构造函数
    WorkerManager();
    //析构函数
    ~WorkerManager();
    //展示菜单
    void showMenu();
    //推出系统
    void exitSystem();
    //增加职工
    void addEmp();
    //显示职工
    void showEmp();
    //删除职工
    void deleteEmp();
    //判断员工是否存在
    int isExist(int id);
    //修改职工
    void modEmp();
    //查找职工
    void findEmp();
    //排序职工
    void sortEmp();
    //清理文件
    void cleanFile();
    //保存文件
    void save();
    //读取文件
    void read();
    //统计人数
    int getEmpNum();

};

//声明，菜单输入选项
void showMainSelect();

#endif //WORKERMANAGER_H
