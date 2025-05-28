//
// Created by 杨充 on 2025/4/28.
//
#include "iostream"
using namespace std;

class C1 {
    int a;  //默认是私有权限
};

struct C2 {
    int a;  //默认是公共权限
};

//struct和class区别
void test1() {
    C1 c1;
    // c1.a = 10; //错误，访问权限是私有
    C2 c2;
    c2.a = 10; //正确，访问权限是公共
}

int main() {
    test1();
    return 0;
}