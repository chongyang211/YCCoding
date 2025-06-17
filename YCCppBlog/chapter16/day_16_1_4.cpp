//
// Created by 杨充 on 2025/4/28.
//
#include "iostream"
using namespace std;


int main() {
    int number;
    double pi;
    std::string message;

    std::cout << "Enter a number: ";
    std::cin >> number;

    std::cout << "Enter the value of pi: ";
    std::cin >> pi;

    std::cout << "Enter a message: ";
    std::cin.ignore(); // 忽略之前的换行符
    std::getline(std::cin, message);

    std::cout << "You entered: " << number << ", " << pi << ", " << message << std::endl;

    return 0;
}