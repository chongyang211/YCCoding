//
// Created by 杨充 on 2025/4/28.
//
#include "iostream"
using namespace std;

class Person {
public:
    static int a;
    int b;
    static void func() {
        cout << "func调用，可以直接用类调用，也可以用对象调用" << endl;
        a = 100;
        //b = 100; //错误，不可以访问非静态成员变量
    }
    void func3() {
        cout << "普通func调用，必须用对象调用" << endl;
    }
private:
    //静态成员函数也是有访问权限的
    static void func2() {
        cout << "func2调用" << endl;
    }
};
int Person::a = 10;

int main() {
    //1、通过对象
    Person p1;
    p1.func();
    p1.func3();

    //2、通过类名
    Person::func();
    //Person::func2(); //私有权限访问不到
    return 0;
}