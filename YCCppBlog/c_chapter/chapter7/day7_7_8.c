//
// Created by 杨充 on 2025/7/8.
//

#include <stdio.h>

//多级指针

int main() {
    int num = 10;
    int *ptr1 = &num;   //一级指针，指向整型变量
    int **ptr2 = &ptr1; //二级指针，指向指针 ptr1
    printf("Value of num: %d\n", num);
    printf("Value of num using ptr1: %d\n", *ptr1);
    printf("Value of num using ptr2: %d\n", **ptr2);
    return 0;
}
//在这个示例中，我们定义了一个整型变量 num，一个一级指针 ptr1，指向整型变量 num，以及一个二级指针 ptr2，指向一级指针 ptr1。
//通过多级指针，我们可以间接地访问和修改变量的值。
//int *ptr1 = &num;：ptr1 是一个指向整型变量的指针，存储了整型变量 num 的地址。
//int **ptr2 = &ptr1;：ptr2 是一个指向指针 ptr1 的指针，存储了指针 ptr1 的地址。
//通过 *ptr1 和 **ptr2，我们可以分别访问到整型变量 num 的值。多级指针在某些情况下非常有用，特别是在处理动态内存分配或复杂数据结构时。需要注意的是，多级指针的使用可能会增加代码的复杂性，因此在使用时要小心确保指针的正确性。



