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

void test2_2(){
    class Student yc1;  //正确
    Student yc2;  //同样正确
    Student yc3[100];   //创建对象数组
}

void test2_3() {
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

void test2_4() {
    Student stu;
    Student *pStuCopy = &stu;

    Student *pStu = new Student;
    pStu->name = "打工充";
    pStu->age = 30;
    pStu->score = 92.5f;
    pStu->say();
    delete pStu;  //删除对象
}

int main() {
    test2_2();
    test2_3();
    return 0;
}