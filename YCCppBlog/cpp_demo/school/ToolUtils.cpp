//
// Created by 杨充 on 2025/8/4.
//

#include "ToolUtils.h"
#include <iostream>
#include <fstream>
using namespace std;

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