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
        age = 0;
    }
    Person(int age) {
        cout << "有参构造函数!" << endl;
        age = age;
    }
    Person(const Person& p) {
        cout << "拷贝构造函数!" << endl;
        age = p.age;
    }
    //析构函数在释放内存之前调用
    ~Person() {
        cout << "析构函数!" << endl;
    }
};

//1. 使用一个已经创建完毕的对象来初始化一个新对象
void test1() {
    Person man(80);//p对象已经创建完毕
    Person newman1(man); //调用拷贝构造函数
    Person newman2 = man;   //拷贝构造
    Person newman3;
    newman3 = man;//不是调用拷贝构造函数，赋值操作
}

//2. 值传递的方式给函数参数传值
//相当于Person p1 = p;
void doWork(Person p) {
    cout << "给函数参数传值 " << &p << endl;
}
void test2() {
    Person p;   //无参构造函数
    doWork(p);
}

Person doWork2() {
    Person p1;
    cout << (int *)&p1 << endl;
    return p1;
}

//3. 以值方式返回局部对象
void test3() {
    Person p = doWork2();
    cout << (int *)&p << endl;
}

int main() {
    test1();
    test2();
    test3();
    return 0;
}