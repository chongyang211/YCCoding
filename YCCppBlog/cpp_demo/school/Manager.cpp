//
// Created by 杨充 on 2025/6/30.
//

#include "Manager.h"

//默认构造
Manager::Manager() {

}

//有参构造
Manager::Manager(string name , string pwd) {
    this->name = name;
    this->pwd = pwd;
}

//选择菜单
void Manager::openMenu() {
    cout << "欢迎管理员："<<this->name << "登录！" << endl;
    cout << "\t\t ---------------------------------\n";
    cout << "\t\t|                                |\n";
    cout << "\t\t|          1.添加账号            |\n";
    cout << "\t\t|                                |\n";
    cout << "\t\t|          2.查看账号            |\n";
    cout << "\t\t|                                |\n";
    cout << "\t\t|          3.查看机房            |\n";
    cout << "\t\t|                                |\n";
    cout << "\t\t|          4.清空预约            |\n";
    cout << "\t\t|                                |\n";
    cout << "\t\t|          0.注销登录            |\n";
    cout << "\t\t|                                |\n";
    cout << "\t\t ---------------------------------\n";
    cout << "请选择您的操作： " << endl;
}

//添加账号
void Manager::addPerson() {
}

//查看账号
void Manager::showPerson() {
}

//查看机房信息
void Manager::showComputer() {
}

//清空预约记录
void Manager::cleanFile() {
}

