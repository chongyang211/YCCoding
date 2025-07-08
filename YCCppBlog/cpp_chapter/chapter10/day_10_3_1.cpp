//
// Created by 杨充 on 2025/4/28.
//
#include "iostream"
using namespace std;

class Person{
public:
    static int a;   //静态成员变量
    int c;      //普通成员变量
private:
    static int b;  //静态成员变量也是有访问权限的
};

int Person::a = 10;
int Person::b = 10;

int main() {
    //1、通过对象
    Person p1;
    p1.a = 100;
    cout << "p1.a = " << p1.a << endl;

    Person p2;
    p2.a = 200;
    p2.c = 300;
    cout << "p1.a = " << p1.a << endl; //共享同一份数据
    cout << "p2.a = " << p2.a << endl;
    cout << "p1.c = " << p1.c << endl;
    cout << "p2.c = " << p2.c << endl;

    //2、通过类名
    cout << "a = " << Person::a << endl;

    //cout << "b = " << Person::b << endl; //私有权限访问不到
    return 0;
}