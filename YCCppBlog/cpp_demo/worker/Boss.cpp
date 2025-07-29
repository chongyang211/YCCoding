//
// Created by 杨充 on 2025/6/28.
//

#include "Boss.h"


Boss::Boss(int id, string name, int dId) {
    this->id = id;
    this->name = name;
    this->deptId = dId;
}

void Boss::showInfo() {
    cout << "职工编号： " << this->id
        << " \t职工姓名： " << this->name
        << " \t岗位：" << this->getDeptName()
        << " \t岗位职责：管理公司所有事务" << endl;
}

string Boss::getDeptName() {
    return string("总裁");
}

Boss::~Boss() {

}




