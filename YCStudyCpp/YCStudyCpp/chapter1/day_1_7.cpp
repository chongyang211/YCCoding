//
// Created by 杨充 on 2025/3/13.
//
#include <iostream>
using namespace std;

//定义命名空间
namespace first_space {
    void fun() {
        cout << "Inside first_space" << endl;
    }
}
namespace second_space {
    void fun() {
        cout << "Inside second_space" << endl;
    }
}
void test1() {
    cout << "定义命名空间" << endl;
    // 调用第一个命名空间中的函数
    first_space::fun();
    // 调用第二个命名空间中的函数
    second_space::fun();
}

//using 指令
using namespace first_space;
//using 指令引入的名称遵循正常的范围规则。名称从使用 using 指令开始是可见的，直到该范围结束。此时，在范围以外定义的同名实体是隐藏的。
void test2() {
    cout << "using 指令" << endl;
    fun();
}

//嵌套的命名空间
namespace first_sp{
    void funSp() {
        cout << "Inside first_sp，逗比充1" << endl;
    }
    namespace second_sp {
        void funSp() {
            cout << "Inside second_sp，逗比充2" << endl;
        }
    }
}
using namespace first_sp::second_sp;


void test3() {
    cout << "嵌套的命名空间" << endl;
    // 调用第二个命名空间中的函数
    funSp();
}

int main() {
    test1();
    test2();
    test3();
    return 0;
}

