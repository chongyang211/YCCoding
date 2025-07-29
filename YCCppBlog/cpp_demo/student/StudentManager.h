//
// Created by 杨充 on 2025/6/28.
//

#include <string>
#include <iostream>
using namespace std;

#ifndef STUDENTMANAGER_H
#define STUDENTMANAGER_H



//联系人结构体
struct Person {
    string name;    //姓名
    int sex;    //性别，1男，2女
    int age;    //年龄
    string phone;   //电话
    string address; //住址
};

#define MAX = 100;  //最大人数
//通讯录结构体
struct AddressBooks {
    //error: flexible array member 'personArray' with type 'struct Person[]' is not at the end of struct
    //struct Person personArray[]; //通讯录中保存的联系人数组
    int size;   //通讯录中人员人数
    // 柔性数组成员必须是结构体的最后一个成员
    // 这里定义一个柔性数组成员 personArray
    struct Person personArray[100]; //通讯录中保存的联系人数组
};

//声明，菜单界面
void showMenu();

//声明，菜单输入选项
void showMainSelect();

//添加联系人信息
void addPerson(AddressBooks *abs);

//显示联系人
void showPerson(AddressBooks * abs);

//检查联系人是否存在
int isExist(AddressBooks * abs, string name);

//删除指定联系人信息
void deletePerson(AddressBooks * abs);

//查找指定联系人信息
void findPerson(AddressBooks * abs);

//修改指定联系人信息
void modifyPerson(AddressBooks * abs);

//清空所有联系人
void cleanPerson(AddressBooks * abs);

#endif //STUDENTMANAGER_H
