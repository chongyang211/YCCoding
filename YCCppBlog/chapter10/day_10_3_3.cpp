//
// Created by 杨充 on 2025/4/28.
//
#include "iostream"
using namespace std;

//三种权限
//公共权限  public     类内可以访问  类外可以访问
//保护权限  protected  类内可以访问  类外不可以访问
//私有权限  private    类内可以访问  类外不可以访问

class Person {
    //公共权限
public:
    string name;
    //保护权限
protected:
    string car;
    //私有权限
private:
    int password;
public:
    void func() {
        name = "打工充";
        car = "雅阁";
        password = 123456;
    }
};

int main() {
    Person p;
    p.name = "逗比";
    //p.car = "奔驰";   //保护权限类外访问不到
    //p.password = 123; //私有权限类外访问不到
    return 0;
}