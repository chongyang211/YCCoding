//
// Created by 杨充 on 2025/3/13.
//
#include <iostream>
using namespace std;
int main() {
    bool flag = true;
    cout << flag << endl; // 1
    flag = false;
    cout << flag << endl; // 0
    cout << "size of bool = " << sizeof(bool) << endl; //1
    return 0;
}
//1
//0
//size of bool = 1