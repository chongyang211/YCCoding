//
// Created by 杨充 on 2025/6/30.
//

#include "ComputerManager.h"
#include "Student.h"

//执行指令：g++ Manager.cpp Student.cpp Teacher.cpp ComputerManager.cpp

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
    pause(); //暂停
    clearScreen(); // 清空屏幕
}

int main() {
    int select = 0;
    while (true) {
        cout << "======================  欢迎来到传智播客机房预约系统  =====================" << endl;
        cout << endl << "请输入您的身份" << endl;
        cout << "\t\t -------------------------------\n";
        cout << "\t\t|                               |\n";
        cout << "\t\t|          1.学生代表           |\n";
        cout << "\t\t|                               |\n";
        cout << "\t\t|          2.老    师           |\n";
        cout << "\t\t|                               |\n";
        cout << "\t\t|          3.管 理 员           |\n";
        cout << "\t\t|                               |\n";
        cout << "\t\t|          0.退    出           |\n";
        cout << "\t\t|                               |\n";
        cout << "\t\t -------------------------------\n";
        cout << "输入您的选择: ";
        cin >> select; //接受用户选择
        switch (select) {
            case 1: //学生身份
                loginIn(STUDENT_FILE,1);
                break;
            case 2: //老师身份
                loginIn(TEACHER_FILE,2);
                break;
            case 3: //管理员身份
                loginIn(ADMIN_FILE,3);
                break;
            case 0: //退出系统
                cout << "欢迎下一次使用" << endl;
                pause();
                return 0;
            default:
                cout << "输入有误，请重新选择！" << endl;
                clearScreen();
                break;
        }
    }
    pause();
    return 0;
}

void loginIn(string fileName, int type) {
    // Identity identity;   //注意：Identity是抽象类
    Identity *identity = NULL;
    //打开文本文件
    ifstream ifs;
    ifs.open(fileName,ios::in);
    //文件不存在
    if (!ifs.is_open()) {
        cout << "文件不存在" <<endl;
        ifs.close();
        return;
    }
    int id = 0;
    string name;
    string pwd;
    if (type == 1) {    //学生登陆
        cout << "请输入你的学号" << endl;
        cin >> id;
    } else if (type ==2) {  //教师登录
        cout << "请输入你的职工号" << endl;
        cin >> id;
    }
    cout << "请输入用户名：" << endl;
    cin >> name;
    cout << "请输入密码： " << endl;
    cin >> pwd;
    if (type == 1) {
        //学生登录验证
        cout << "学生登录验证" << endl;
        int fId;
        string fName;
        string fPwd;
        while (ifs>>fId && ifs>>fName && ifs>>fPwd) {
            //用户输入的id，姓名和密钥，跟文件做对比
            if (id == fId && name == fName && pwd == fPwd) {
                cout << "学生验证登录成功!" << endl;
                pauseAndCls();
                identity = new Student(id, name, pwd);
                return;
            }
        }
    } else if (type == 2) {
        //教师登录验证
        cout << "教师登录验证" << endl;
        //教师登录验证
        int fId;
        string fName;
        string fPwd;
        while (ifs >> fId && ifs >> fName && ifs >> fPwd) {
            if (id == fId && name == fName && pwd == fPwd) {
                cout << "教师验证登录成功!" << endl;
                pauseAndCls();
                identity = new Teacher(id, name, pwd);
                return;
            }
        }
    } else if (type == 3) {
        //管理员登录验证
        cout << "管理员登录验证" << endl;
    }
    cout << "验证登录失败!" << endl;
    pauseAndCls();
}




