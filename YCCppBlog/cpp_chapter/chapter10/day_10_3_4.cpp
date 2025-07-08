//
// Created by 杨充 on 2025/4/28.
//
#include "iostream"
using namespace std;

class Person {
private:
    string name; //可读可写  姓名
    int age; //只读  年龄
    string lover; //只写  情人
public:
    string getName() {
        return name;
    }

    void setName(string name) {
        Person::name = name;
    }

    int getAge(){
        return age;
    }

    void setAge(int age) {
        Person::age = age;
    }

    void setLover(string lover) {
        this->lover = lover;
    }
};

int main() {
    Person p;
    //姓名设置
    p.setName("张三");
    cout << "姓名： " << p.getName() << endl;
    //年龄设置
    p.setAge(50);
    cout << "年龄： " << p.getAge() << endl;
    //情人设置
    p.setLover("逗比");
    //cout << "情人： " << p.lover << endl;  //只写属性，不可以读取
    return 0;
}