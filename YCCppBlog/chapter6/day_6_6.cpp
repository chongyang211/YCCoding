//
// Created by 杨充 on 2025/4/28.
//

#include "day_6_6.h"


void swap(int a, int b) {
    int temp = a;
    a = b;
    b = temp;
    cout << "a = " << a << endl;
    cout << "b = " << b << endl;
}

int main() {
    int a = 100;
    int b = 200;
    swap(a, b);
    return 0;
}
