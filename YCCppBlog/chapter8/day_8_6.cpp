//
// Created by 杨充 on 2025/6/12.
//

#include <iostream>
using namespace std;

void showValue(const int & v) {
    //v += 10; //常量不能做新的赋值。会直接编译报错
    cout << v << endl;
}

int main() {
    //int& ref = 10;//引用本身需要一个合法的内存空间，因此这行错误
    const int & ref = 10;
    // ref = 100;
    int a = 10;
    showValue(a);
    return 0;
}

