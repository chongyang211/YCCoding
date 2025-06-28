//
// Created by 杨充 on 2025/6/28.
//

#include "Employee.h"

Employee::Employee(int id, string name, int dId) {
    this->id = id;
    this->name = name;
    this->deptId = dId;
}

void Employee::showInfo() {
    cout << "职工编号： " << this->id
            << " \t职工姓名： " << this->name
            << " \t岗位：" << this->getDeptName()
            << " \t岗位职责：完成经理交给的任务" << endl;
}

string Employee::getDeptName() {
    return string("员工");
}
