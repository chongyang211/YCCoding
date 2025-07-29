//
// Created by 杨充 on 2025/6/28.
//

#include "WorkerManager.h"

//编译指令：g++ Worker.cpp Boss.cpp Manager.cpp Employee.cpp WorkerManager.cpp

WorkerManager::WorkerManager() {
    //构造函数实现
    read();
}

WorkerManager::~WorkerManager() {
    //析构函数实现
    if (this->empArray != NULL) {
        delete[] this->empArray;
    }
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
    WorkerManager wm;
    int choice = 0;
    while (true) {
        //展示菜单
        wm.showMenu();
        cout << "请输入您的选择:" << endl;
        cin >> choice;
        switch (choice) {
            case 0: //退出系统
                wm.exitSystem();
                return 0;
            case 1: //添加职工
                wm.addEmp();
            break;
            case 2: //显示职工
                wm.showEmp();
                break;
            case 3: //删除职工
                wm.deleteEmp();
                break;
            case 4: //修改职工
                wm.modEmp();
                break;
            case 5: //查找职工
                break;
            case 6: //排序职工
                break;
            case 7: //清空文件
                break;
            default:
                pause();
            break;
        }
    }
    return 0;
}

//声明，菜单输入选项
void showMainSelect() {

}

void WorkerManager::showMenu() {
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
    pauseAndCls();
}

void WorkerManager::addEmp() {
    cout << "请输入增加职工数量： " << endl;
    int addNum = 0;
    cin >> addNum;
    if (addNum == 0) {
        cout << "输入有误，请重新输入大于0的数值" << endl;
        return;
    }
    //计算新空间大小
    int newSize = this->empNum + addNum;
    //开辟新空间
    Worker ** newSpace = new Worker*[newSize];
    //将原空间下内容存放到新空间下
    if (this->empArray != NULL){
        for (int i = 0; i < this->empNum; i++){
            newSpace[i] = this->empArray[i];
        }
    }
    //输入新数据
    for (int i = 0; i < addNum; i++) {
        int id;
        string name;
        int dSelect;
        cout << "请输入第 " << i + 1 << " 个新职工编号：" << endl;
        cin >> id;
        cout << "请输入第 " << i + 1 << " 个新职工姓名：" << endl;
        cin >> name;
        cout << "请选择该职工的岗位：" << endl;
        cout << "1、普通职工" << endl;
        cout << "2、经理" << endl;
        cout << "3、老板" << endl;
        cin >> dSelect;
        Worker * worker = NULL;
        switch (dSelect) {
            case 1: //普通员工
                worker = new Employee(id, name, 1);
                break;
            case 2: //经理
                worker = new Manager(id, name, 2);
                break;
            case 3: //老板
                worker = new Boss(id, name, 3);
                break;
            default:
                break;
        }
        newSpace[this->empNum + i] = worker;
    }
    //释放原有空间
    delete[] this->empArray;
    //更改新空间的指向
    this->empArray = newSpace;
    //更新新的个数
    this->empNum = newSize;
    //更新职工不为空标志
    this->fileIsEmpty = false;
    //提示信息
    cout << "成功添加" << addNum << "名新职工！" << endl;
    //输入新数据
    clearScreen();
}

//文件保存用户数据
void WorkerManager::save() {
    ofstream ofs;
    ofs.open(FILENAME, ios::out);
    //检查文件是否成功打开
    if (!ofs.is_open()) {
        std::cerr << "Failed to open file!" << std::endl;
        return;
    }
    for (int i = 0; i < this->empNum; i++) {
        ofs << this->empArray[i]->id << " "
                << this->empArray[i]->name << " "
                << this->empArray[i]->deptId << endl;
    }
    ofs.close();
}


void WorkerManager::read() {
    ifstream ifs;
    ifs.open(FILENAME,ios::in);
    //文件不存在情况
    if (!ifs.is_open()){
        cout << "文件不存在" << endl; //测试输出
        this->empNum = 0;  //初始化人数
        this->fileIsEmpty = true; //初始化文件为空标志
        this->empArray = NULL; //初始化数组
        ifs.close(); //关闭文件
        return;
    }
    //文件存在，并且没有记录
    char ch;
    ifs >> ch;
    if (ifs.eof()) {
        cout << "文件为空!" << endl;
        this->empNum = 0;
        this->fileIsEmpty = true;
        this->empArray = NULL;
        ifs.close();
        return;
    }
    int num =  this->getEmpNum();
    cout << "职工个数为：" << num << endl;  //测试代码
    this->empNum = num;  //更新成员属性
}


int WorkerManager::getEmpNum() {
    ifstream ifs;
    ifs.open(FILENAME , ios::in);
    int id;
    string name;
    int dId;
    int num = 0;
    //下面这个写法是什么意思
    //这段代码的作用是从文件流 ifs 中依次读取 id、name 和 dId 三个数据
    //ifs >> id，从文件流 ifs 中读取一个值，并将其存储到变量 id 中。如果读取成功，表达式返回 true；否则返回 false。
    while (ifs>>id && ifs>>name && ifs>>dId) {
        //记录人数
        num++;
    }
    ifs.close();
    return num;
}

//显示职工
void WorkerManager::showEmp() {
    if (this->fileIsEmpty) {
        cout << "文件不存在或记录为空！" << endl;
    } else {
        for (int i=0 ; i<this->empNum ; i++) {
            Worker * worker = this->empArray[i];
            worker->showInfo();
        }
    }
    pause();
}

void WorkerManager::deleteEmp() {
    if (this->fileIsEmpty) {
        cout << "文件不存在或记录为空！" << endl;
    } else {
        //按职工编号删除
        cout << "请输入想要删除的职工号：" << endl;
        int id = 0;
        cin >> id;
        int index = isExist(id);
        if (index == -1) {
            cout << "删除失败，未找到该职工" << endl;
            return;
        }
        //删除数据中某个数组，那就是index后数据都往前挪动一位
        for (int i=index ; i<this->empNum ; i++) {
            this->empArray[i] = this->empArray[i+1];
        }
        this->empNum--;
        this->save();   //保存文件
        cout << "删除成功！" << endl;
    }
    pause();
}

//判断员工是否存在
int WorkerManager::isExist(int id) {
    int index = -1;
    for (int i=0 ; i< this->empNum ; i++) {
        Worker *worker = this->empArray[i];
        if (worker->id == id) {
            index = i;
            break;
        }
    }
    return index;
}

//修改职工
void WorkerManager::modEmp() {
    if (this->fileIsEmpty) {
        cout << "文件不存在或记录为空！" << endl;
    } else {
        //按职工编号进行修改
        cout << "请输入想要修改的职工号：" << endl;
        int id;
        cin >> id;
        int index = isExist(id);
        if (index == -1) {
            cout << "修改失败，未找到该职工" << endl;
            return;
        }
        int newId = 0;
        string newName = "";
        int newSelect = 0;
        cout << "查到： " << id << "号职工，请输入新职工号： " << endl;
        cin >> newId;
        cout << "请输入新姓名： " << endl;
        cin >> newName;
        cout << "请输入新的岗位： " << endl;
        cout << "1、普通职工" << endl;
        cout << "2、经理" << endl;
        cout << "3、老板" << endl;
        cin >> newSelect;
        //先删除
        delete this->empArray[index];
        Worker* worker = NULL;
        switch (newSelect) {
            case1:
                worker = new Employee(newId, newName, newSelect);
                break;
            case2:
                worker = new Manager(newId, newName, newSelect);
                break;
            case 3:
                worker = new Boss(newId, newName, newSelect);
                break;
            default:
                break;
        }
        //更改数据 到数组中
        this->empArray[index] = worker;
        cout << "修改成功！" << endl;
        save();
    }
    pause();
}






