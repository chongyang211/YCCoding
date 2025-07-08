//
// Created by 杨充 on 2025/4/28.
//
#include "iostream"
using namespace std;



class Person {
public:
    int age;
public:
    Person() {
        cout << "无参构造函数!" << endl;
    }
    Person(int a) {
        age = a;
        cout << "有参构造函数!" << a << endl;
    }
    Person (Person& p) {
        age = p.age;
        cout << "拷贝构造函数!" << &p << endl;
    }
    ~Person() {
        cout << "析构函数" << endl;
    }
};

void test1() {
    Person p1(10);
    //如果不写拷贝构造，编译器会自动添加拷贝构造，并且做浅拷贝操作
    Person p2(p1);
    cout << "p2的年龄为： " << p2.age << endl;
}

void test2() {
    //如果用户提供有参构造，编译器不会提供默认构造，会提供拷贝构造
    Person p1; //此时如果用户自己没有提供默认构造，会出错
    Person p2(10); //用户提供的有参
    Person p3(p2); //此时如果用户没有提供拷贝构造，编译器会提供
    //如果用户提供拷贝构造，编译器不会提供其他构造函数
    Person p4; //此时如果用户自己没有提供默认构造，会出错
    Person p5(10); //此时如果用户自己没有提供有参，会出错
    Person p6(p5); //用户自己提供拷贝构造
}

int main() {
    test1();
    test2();
    return 0;
}