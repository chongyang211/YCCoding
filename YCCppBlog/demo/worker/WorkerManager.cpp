//
// Created by 杨充 on 2025/6/28.
//

#include "WorkerManager.h"

WorkerManager::WorkerManager() {
    //构造函数实现
    //初始化人数
    this->empNum = 0;
    //初始化数组指针
    this->empArray = NULL;
}

WorkerManager::~WorkerManager() {
    //析构函数实现
}

void clearScreen() {
    std::cout << "\033[2J\033[1;1H"; // 使用 ANSI Escape Codes 清空屏幕
    std::cout << "清空屏幕" << std::endl;
}

void pause() {
    //替代 system("pause")
    //system("pause");
    std::cout << "点击enter键，暂停";
    std::cin.get(); // 等待用户输入字符
}

//暂停和清空屏幕
void pauseAndCls() {
    pause();    //暂停
    clearScreen(); // 清空屏幕
}

int main() {
    showMainSelect();
    pause();
    return 0;
}

//声明，菜单输入选项
void showMainSelect() {
    WorkerManager wm;
    int choice = 0;
    while (true) {
        //展示菜单
        wm.Show_Menu();
        cout << "请输入您的选择:" << endl;
        cin >> choice;
        switch (choice) {
            case 0: //退出系统
                wm.exitSystem();
                break;
            case 1: //添加职工
                testWorker();
                break;
            case 2: //显示职工
                break;
            case 3: //删除职工
                break;
            case 4: //修改职工
                break;
            case 5: //查找职工
                break;
            case 6: //排序职工
                break;
            case 7: //清空文件
                break;
            default:
                system("cls");
            break;
        }
    }
}

void WorkerManager::Show_Menu() {
    cout << "********************************************" << endl;
    cout << "*********  欢迎使用职工管理系统！ **********" << endl;
    cout << "*************  0.退出管理程序  *************" << endl;
    cout << "*************  1.增加职工信息  *************" << endl;
    cout << "*************  2.显示职工信息  *************" << endl;
    cout << "*************  3.删除离职职工  *************" << endl;
    cout << "*************  4.修改职工信息  *************" << endl;
    cout << "*************  5.查找职工信息  *************" << endl;
    cout << "*************  6.按照编号排序  *************" << endl;
    cout << "*************  7.清空所有文档  *************" << endl;
    cout << "********************************************" << endl;
    cout << endl;
}

void WorkerManager::exitSystem() {
    cout << "欢迎下次使用" << endl;
    pause();
    exit(0);
}




