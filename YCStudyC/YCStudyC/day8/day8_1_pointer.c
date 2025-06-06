//
// Created by 杨充 on 2023/5/25.
//
//stdio.h 是一个头文件 (标准输入输出头文件) , #include 是一个预处理命令，用来引入头文件。
#include <stdio.h>

//8.1.1.1 如何访问内存地址
void test_8_1_1_1();
//8.1.1.2 理解什么是指针
void test_8_1_1_2();
//8.1.1.3 如何去使用指针
void test_8_1_1_3();
//8.1.1.4 使用指针的注意事项
void test_8_1_1_4();
//8.1.1.5 C语言中的NULL指针
void test_8_1_1_5();



//所有的 C 语言程序都需要包含 main() 函数。 代码从 main() 函数开始执行。
int main(){
    test_8_1_1_1();
    test_8_1_1_2();
    test_8_1_1_3();
    test_8_1_1_4();
    test_8_1_1_5();
    return 0;
}

//8.1.1.1 如何访问内存地址
void test_8_1_1_1() {
    //在 C 语言中，可以使用指针来访问内存地址。指针是一个变量，它存储了内存地址的值。以下是一些示例代码，展示了如何访问内存地址：
    int num = 42;
    int *ptr = &num;

    //在上述代码中，我们定义了一个整数变量 num，并使用 & 运算符获取 num 的内存地址，并将其存储在指针 ptr 中。
    //然后，我们使用 %p 格式说明符打印出 num 的内存地址和 ptr 的值。最后，使用解引用操作符 * 访问指针 ptr 所指向的值。

    printf("num 的值：%d\n", num);
    printf("num 的内存地址：%p\n", &num);
    printf("ptr 的值：%p\n", ptr);
    printf("ptr 指向的值：%d\n", *ptr);
    printf("ptr 指针的内存地址：%p\n", &ptr);

    //num 的值：42
    //num 的内存地址：0x16fd5736c
    //ptr 的值：0x16fd5736c
    //ptr 指向的值：42
    //ptr 指针的内存地址：0x16fd57360

    //请注意，访问内存地址需要谨慎操作，因为直接操作内存可能会导致错误和不可预测的行为。
    //在实际开发中，需要确保正确使用指针，并遵循内存管理的最佳实践，以避免潜在的问题。
}



//8.1.1.2 理解什么是指针
void test_8_1_1_2() {
    //指针也就是内存地址，指针变量是用来存放内存地址的变量。
    //就像其他变量或常量一样，您必须在使用指针存储其他变量地址之前，对其进行声明。指针变量声明的一般形式为：

    int    *ip;    /* 一个整型的指针 */
    double *dp;    /* 一个 double 型的指针 */
    float  *fp;    /* 一个浮点型的指针 */
    char   *ch;    /* 一个字符型的指针 */
}


//8.1.1.3 如何去使用指针
void test_8_1_1_3() {
    int var = 20;   //实际变量的声明
    int *ip;    //指针变量的声明
    ip = &var;  //在指针变量中存储 var 的地址
    printf("var 的值：%d\n", var);
    printf("var 变量的地址: %p\n", &var  );
    /* 在指针变量中存储的地址 */
    printf("ip 变量存储的地址: %p\n", ip );
    /* 使用指针访问值 */
    printf("*ip 变量的值: %d\n", *ip );
    //var 的值：20
    //var 变量的地址: 0x16d29736c
    //ip 变量存储的地址: 0x16d29736c
    //*ip 变量的值: 20
}


//8.1.1.4 使用指针的注意事项
void test_8_1_1_4() {

}


//8.1.1.5 C语言中的NULL指针
void test_8_1_1_5() {
    int *ptr = NULL;
    /* 在指针变量中存储的地址 */
    printf("ptr 变量存储的地址: %p\n", ptr );
    /* 使用指针访问值 */
    printf("*ptr 变量的值: %d\n", *ptr );

    if (ptr != NULL) {
        // 指针不为 NULL，可以安全地使用它
    }

    //free(ptr);
    ptr = NULL;
}







