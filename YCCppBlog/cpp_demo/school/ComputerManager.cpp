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

                break;
            case 2: //老师身份

                break;
            case 3: //管理员身份

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




