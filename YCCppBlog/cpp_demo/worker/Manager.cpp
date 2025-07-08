//
// Created by 杨充 on 2025/6/28.
//

#include "Manager.h"

Manager::Manager(int id, string name, int dId) {
    this->id = id;
    this->name = name;
    this->deptId = dId;
}

void Manager::showInfo() {
    cout << "职工编号： " << this->id
    << " \t职工姓名： " << this->name
    << " \t岗位：" << this->getDeptName()
    << " \t岗位职责：完成老板交给的任务,并下发任务给员工" << endl;
}

string Manager::getDeptName() {
    return string("经理");
}

