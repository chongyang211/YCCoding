//
// Created by 杨充 on 2025/6/30.
//

#include "Student.h"


//默认构造
Student::Student() {
}


//有参构造(学号、姓名、密码)
Student::Student(int id, string name, string pwd) {
    this->id = id;
    this->name = name;
    this->pwd = pwd;
}

//菜单界面
void Student::openMenu() {
}

//申请预约
void Student::applyOrder() {
}

//查看我的预约
void Student::showMyOrder() {
}

//查看所有预约
void Student::showAllOrder() {
}

//取消预约
void Student::cancelOrder() {
}


