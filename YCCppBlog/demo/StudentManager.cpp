//
// Created by 杨充 on 2025/6/28.
//

#include "StudentManager.h"
#include "iostream"
using namespace std;

int main() {
    showMainSelect();
    return 0;
}

void clearScreen() {
    std::cout << "\033[2J\033[1;1H"; // 使用 ANSI Escape Codes 清空屏幕
}

//暂停和清空屏幕
void pauseAndCls() {
    //替代 system("pause")
    //system("pause");
    std::cout << "点击enter键，暂停";
    std::cin.get(); // 等待用户输入字符
    //替代 system("cls")
    //system("cls");
    clearScreen(); // 清空屏幕
    std::cout << "清空屏幕" << std::endl;
}

//菜单界面
void showMenu() {
    cout << "***************************" << endl;
    cout << "*****  1、添加联系人  *****" << endl;
    cout << "*****  2、显示联系人  *****" << endl;
    cout << "*****  3、删除联系人  *****" << endl;
    cout << "*****  4、查找联系人  *****" << endl;
    cout << "*****  5、修改联系人  *****" << endl;
    cout << "*****  6、清空联系人  *****" << endl;
    cout << "*****  0、退出通讯录  *****" << endl;
    cout << "***************************" << endl;
}

void showMainSelect() {
    //创建通讯录
    AddressBooks address;
    //初始化通讯录人数
    address.size = 0;
    int select = 0;
    while (true) {
        showMenu();
        //从键盘输入
        cin >> select;
        switch (select) {
            case 1:  //添加联系人
                addPerson(&address);
                break;
            case 2:  //显示联系人
                showPerson(&address);
                break;
            case 3:  //删除联系人
                break;
            case 4:  //查找联系人
                break;
            case 5:  //修改联系人
                break;
            case 6:  //清空联系人
                break;
            case 0:  //退出通讯录
                cout << "欢迎下次使用" << endl;
                //在 macOS 上，由于没有类似于 Windows 中的 system("pause") 函数来暂停控制台输出
                //system("pause");
                std::cout << "Press Enter to continue..."  << endl;
                std::cin.get(); // 等待用户输入字符
                return;
                break;
            default:
                break;
        }
    }
    showMenu();
    //在 macOS 上，由于没有类似于 Windows 中的 system("pause") 函数来暂停控制台输出
    //system("pause");
    std::cout << "Press Enter to continue...";
    std::cin.get(); // 等待用户输入字符
}

//添加联系人信息
void addPerson(AddressBooks *abs) {
    if (abs->size == 100) {
        cout << "通讯录已经满了，无法添加" << endl;
        return;
    }
    string name;
    cout << "请输入姓名：" << endl;
    cin >> name;
    //直接赋值
    abs->personArray[abs->size].name = name;

    cout << "请输入性别：" << endl;
    cout << "1 -- 男" << endl;
    cout << "2 -- 女" << endl;
    int sex = 0;
    while (true) {
        cin >> sex;
        if (sex == 1 || sex ==2) {
            abs->personArray[abs->size].sex = sex;
            break;
        }
        cout << "输入性别有误，请重新输入" << endl;
    }

    int age;
    cout << "请输入年龄：" << endl;
    cin >> age;
    abs->personArray[abs->size].age = age;

    string phone;
    cout << "请输入电话：" << endl;
    cin >> phone;
    abs->personArray[abs->size].phone = phone;

    string address;
    cout << "请输入家庭地址：" << endl;
    cin >> address;
    abs->personArray[abs->size].address = address;

    //更新通讯录
    abs->size ++;
    cout << "添加用户：" << name << "成功。用户列表size是：" << abs->size << endl;
    pauseAndCls();
}

//显示所有联系人信息
void showPerson(AddressBooks * abs) {
    if (abs->size == 0) {
        cout << "通讯录为空，没有联系人" << endl;
        return;
    }
    for (int i=0 ; i< abs->size ; i++) {
        cout << "姓名：" << abs->personArray[i].name << "\t";
        cout << "性别：" << (abs->personArray[i].sex == 1 ? "男" : "女") << "\t";
        cout << "年龄：" << abs->personArray[i].age << "\t";
        cout << "电话：" << abs->personArray[i].phone << "\t";
        cout << "住址：" << abs->personArray[i].address << endl;
    }
    pauseAndCls();
}