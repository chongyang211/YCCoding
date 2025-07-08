//
// Created by 杨充 on 2025/4/28.
//
#include "iostream"
using namespace std;

class Person {
public:
    Person() {
        cout << "Person的构造函数调用" << endl;
    }
    ~Person() {
        cout << "Person的析构函数调用" << endl;
    }
};

int main() {
    Person p;
    cout << "main" << endl;
    return 0;
}