//
// Created by 杨充 on 2025/6/28.
//
#include "Worker.h"
#include "Boss.h"
#include "Manager.h"
#include "Employee.h"

//测试一下职工多态
void testWorker() {
    //Worker *worker;
    Worker *worker = NULL;
    worker = new Employee(1, "张三", 1);
    worker->showInfo();
    delete worker;

    worker = new Manager(2, "李四", 2);
    worker->showInfo();
    delete worker;

    worker = new Boss(3, "王五", 3);
    worker->showInfo();
    delete worker;
}


int main() {
    testWorker();
    return 0;
}

