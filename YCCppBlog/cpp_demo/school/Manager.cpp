//
// Created by 杨充 on 2025/6/30.
//

#include "Manager.h"
#include "GlobalFile.h"
#include "ToolUtils.h"

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
    cout << "请输入添加账号的类型" << endl;
    cout << "1、添加学生" << endl;
    cout << "2、添加老师" << endl;
    string fileName;
    string tip;
    ofstream ofs;
    int select = 0;
    cin >> select;
    //相当于往file文件写数据内容
    if (select == 1){
        fileName = STUDENT_FILE;    //学生
        tip = "请输入学号： ";
    } else {
        fileName = TEACHER_FILE;    //老师
        tip = "请输入职工编号：";
    }
    //打开文件
    ofs.open(fileName, ios::out | ios::app);
    if (!ofs.is_open()) {
        cout << "文件找不到" << endl;
        return;
    }
    int id;
    string name;
    string pwd;
    cout <<tip << endl;
    cin >> id;
    cout << "请输入姓名： " << endl;
    cin >> name;
    cout << "请输入密码： " << endl;
    cin >> pwd;
    //将控制台的信息输入到file文件中
    ofs << id << " " << name << " " << pwd << " " << endl;
    cout << "添加成功" << endl;
    pauseAndCls();
    ofs.close();
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

void Manager::initVector() {
    //读取学生文件信息
    ifstream ifs;
    ifs.open(STUDENT_FILE, ios::in);
    if (!ifs.is_open()) {
        cout << "学生文件读取失败" << endl;
        return;
    }
    vStu.clear();
    Student s;
    while (ifs >> s.id && ifs >> s.name &&  ifs >> s.pwd) {
        vStu.push_back(s);
    }
    cout << "当前学生数量为： " << vStu.size() << endl;
    ifs.close(); //学生初始化
    //读取老师文件信息
    ifs.open(TEACHER_FILE, ios::in);
    if (!ifs.is_open()) {
        cout << "老师文件读取失败" << endl;
        return;
    }
    vTea.clear();
    Teacher t;
    while (ifs >> t.empId && ifs >> t.name &&  ifs >> t.pwd) {
        vTea.push_back(t);
    }
    cout << "当前教师数量为： " << vTea.size() << endl;
    ifs.close();
}

