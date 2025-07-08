//
// Created by 杨充 on 2025/4/28.
//
#include "iostream"
using namespace std;


class ShallowCopy {
public:
    int* data;

    ShallowCopy(int value) {
        data = new int(value);
    }

    ~ShallowCopy() {
        delete data;
    }
};

void test1() {
    ShallowCopy obj1(10);
    ShallowCopy obj2 = obj1; // 默认拷贝构造函数（浅拷贝）
    *obj2.data = 20;         // 修改 obj2 的数据
    std::cout << *obj1.data; // 输出 20，obj1 的数据也被修改
}

class DeepCopy {
public:
    int* data;

    DeepCopy(int value) {
        data = new int(value);
    }

    // 深拷贝构造函数
    DeepCopy(const DeepCopy& other) {
        data = new int(*other.data); // 分配新内存并复制数据
    }

    ~DeepCopy() {
        delete data;
    }
};

void test2() {
    DeepCopy obj1(10);
    DeepCopy obj2 = obj1; // 调用深拷贝构造函数
    *obj2.data = 20;      // 修改 obj2 的数据
    std::cout << *obj1.data; // 输出 10，obj1 的数据不受影响
}

int main() {
    test1();
    test2();
    return 0;
}

