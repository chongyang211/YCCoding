//
// Created by 杨充 on 2025/4/28.
//
#include "iostream"
using namespace std;

//类定义和对象

class Student{
public:
    //成员变量
    char *name;
    int age;
    float score;

    //成员函数
    void say(){
        cout<<name<<"的年龄是"<<age<<"，成绩是"<<score<<endl;
    }
};

void test2_1(){
    class Student yc1;  //正确
    Student yc2;  //同样正确
    Student yc3[100];   //创建对象数组
}

void test2_2() {
    Student stu;
    stu.name = "打工充";
    stu.age = 30;
    stu.score = 100;
    stu.say();

    Student student;
    student.name = "新生代民工充";
    student.age = 30;
    student.score = 100;
    student.say();
}

void test2_3() {
    Student stu;
    Student *pStuCopy = &stu;

    Student *pStu = new Student;
    pStu->name = "打工充";
    pStu->age = 30;
    pStu->score = 92.5f;
    pStu->say();
    delete pStu;  //删除对象
}

const double PI = 3.14;
//封装一个圆类，求圆的周长
//class代表设计一个类，后面跟着的是类名
class Circle {
public:     //访问权限  公共的权限
    //属性
    int r;
    //行为
    //获取到圆的周长
    double calculate() {
        //获取圆的周长
        return 2*PI*r;
    }
};

void test2_4() {
    //通过圆类，创建圆的对象
    // c1就是一个具体的圆
    Circle c1;
    c1.r = 10; //给圆对象的半径 进行赋值操作
    //2 * pi * 10 = = 62.8
    cout << "圆的周长为： " << c1.calculate() << endl;

}

int main() {
    test2_2();
    test2_3();
    return 0;
}