//
// Created by 杨充 on 2025/7/12.
//

#include "StudentManager.h"

// 暂停程序
void pauseProgram() {
    printf("Press Enter to continue...\n");
    getchar();
}

// 清屏
void clearScreen() {
    system("clear"); // 或使用 printf("\033[H\033[J");
}

int main() {
    while (1) {
        //打印菜单
        Menu();
        //读取一个字符 整数,字符在内存中映射为ASCII码
        char ch = getchar();
        printf("输入的字符是：%c \n", ch);
        switch (ch) {
            case '1': //1.录入学生信息
                printf("录入学生信息\n");
                InputStudent();
                break;
            case '2': //2.打印学生信息
                printf("打印学生信息\n");
                PrintStudent();
                break;
            case '3': //3.保存学生信息
                printf("保存学生信息\n");
                SaveStudent();
                break;
            case '4': //4.读取学生信息
                printf("读取学生信息\n");
                ReadStudent();
                break;
            case '5': //5.统计所有学生人数
                printf("统计所有学生人数\n");
                CountStudent();
                break;
            case '6': //6.查找学生信息
                printf("查找学生信息\n");
                break;
            case '7': //7.修改学生信息
                printf("修改学生信息\n");
                break;
            case '8': //8.删除学生信息
                printf("删除学生信息\n");
                break;
            case '0': //0.退出系统
                printf("退出系统\n");
                pauseProgram();
                return 0;
                break;
            default:
                printf("输入有误，没有该功能\n\n");
                pauseProgram();
                clearScreen();
                break;
        }
    }
    return 0;
}

void Menu() {
    printf("*************************************************\n");
    printf("*\t欢迎使用高校学生成绩管理系统V1.0\t*\n");
    printf("*\t\t请选择功能\t\t\t*\n");
    printf("*************************************************\n");
    printf("*\t\t1.录入学生信息\t\t\t*\n");
    printf("*\t\t2.打印学生信息\t\t\t*\n");
    printf("*\t\t3.保存学生信息\t\t\t*\n");
    printf("*\t\t4.读取学生信息\t\t\t*\n");
    printf("*\t\t5.统计所有学生人数\t\t*\n");
    printf("*\t\t6.查找学生信息\t\t\t*\n");
    printf("*\t\t7.修改学生信息\t\t\t*\n");
    printf("*\t\t8.删除学生信息\t\t\t*\n");
    printf("*\t\t0.退出系统\t\t\t*\n");
    printf("*************************************************\n");
}


void InputStudent() {
    //创建一个人，在堆中分配内存
    Node *pNewNode = (Node *) malloc(sizeof(Node));
    //指针下一个指向空
    pNewNode->pNext = NULL;

    //查找链表的尾结点
    Node *p = g_pHead;
    //g_pHead != NULL：确保链表不为空。
    //p->pNext != NULL：确保当前节点的下一个节点不为空（即当前节点不是最后一个节点）。
    while (g_pHead != NULL && p->pNext != NULL) {
        //将指针 p 移动到下一个节点。
        p = p->pNext;
    }
    //把节点插到链表的尾节点
    if (g_pHead == NULL) {
        //无数据，则放到头部
        g_pHead = pNewNode;
    } else {
        p->pNext = pNewNode;
    }
    //录入学生信息
    printf("请输入学生姓名：\n");
    //warning: data argument not used by format string [-Wformat-extra-args]
    //这个警告是因为 scanf 的格式字符串 "%s" 只需要一个参数（即存储输入的缓冲区）
    //但你提供了两个参数（pNewNode->stu.szName 和 sizeof(pNewNode->stu.szName)）。scanf 不会使用第二个参数，因此编译器发出了警告。
    //scanf("%s", pNewNode->stu.szName, sizeof(pNewNode->stu.szName));
    scanf("%19s", pNewNode->stu.szName); // 假设 szName 的大小为 20
    // fgets(pNewNode->stu.szName, sizeof(pNewNode->stu.szName), stdin);
    // // 去掉 fgets 可能读取的换行符
    // size_t lenName = strlen(pNewNode->stu.szName);
    // if (lenName > 0 && pNewNode->stu.szName[lenName - 1] == '\n') {
    //     pNewNode->stu.szName[lenName - 1] = '\0';
    // }
    printf("请输入性别：\n");
    //scanf("%s", pNewNode->stu.szSex, sizeof(pNewNode->stu.szSex));
    scanf("%3s", pNewNode->stu.szSex);
    printf("请输入学生年龄：\n");
    scanf("%d", &pNewNode->stu.nAge);
    printf("请输入学号：\n");
    scanf("%d", &pNewNode->stu.nStuNo);
    printf("请输入成绩：\n");
    scanf("%d\n", &pNewNode->stu.nScore);
    printf("学生信息录入成功。\n\n");
    pauseProgram();
    clearScreen();
}

//打印学生信息
void PrintStudent() {
    clearScreen();
    //遍历链表
    Node *p = g_pHead;
    if (p == NULL) {
        printf("系统中暂无学生信息，请录入后再来打印查看。\n\n");
        return;
    }
    printf("*********************************************************************************\n");
    printf("*\t\t\t欢迎使用高校学生成绩管理系统V1.0\t\t\t*\n");
    printf("*********************************************************************************\n");
    printf("*\t学号\t*\t姓名\t*\t性别\t*\t年龄\t*\t成绩\t*\n");
    printf("*********************************************************************************\n");
    while (p != NULL) {
        printf("*\t%d\t*\t%s\t*\t%s\t*\t%d\t*\t%d\t*\n",
               p->stu.nStuNo,
               p->stu.szName,
               p->stu.szSex,
               p->stu.nAge,
               p->stu.nScore
        );
        //指向下一个节点
        p = p->pNext;
        printf("*********************************************************************************\n");
    }
    pauseProgram();
    clearScreen();
}

//保存学生信息
void SaveStudent() {
    //打开文件
    FILE *pFile;
    pFile = fopen("stuinfo.txt", "w");
    if (pFile == NULL) {
        printf("打开文件失败。\n");
        return;
    }
    //写入数据
    Node *p = g_pHead;
    //判断是否有数据
    if (p == NULL) {
        printf("链表中没有数据\n");
        return;
    }
    while (p != NULL) {
        fwrite(&p->stu, sizeof(Node), 1, pFile);
        //指向下一个
        p = p->pNext;
    }
    //关闭文件
    fclose(pFile);
    printf("数据保存成功。\n");
    pauseProgram();
}

//读取学生信息
void ReadStudent() {
    clearScreen();
    //打开文件
    FILE *pFile;
    pFile = fopen("stuinfo.txt", "r");
    if (pFile == NULL) {
        printf("打开文件失败。\n");
        return;
    }

    //创建一个人，在堆中分配内存
    Node *p = (Node *) malloc(sizeof(Node));
    p->pNext = NULL;
    g_pHead = p;
    //逐个单词读入文本内容
    char str[200];
    int i = 0;
    while (fscanf(pFile, "%s", str) != EOF) {
        //读文件
        //单词不是*或者空时，进行赋值
        // 检查输入是否为 "*"
        if (strcmp(str, "*") == 0) {
            printf("Input terminated.\n");
            continue;
        }
        switch (i) {
            case 0:
                p->stu.nStuNo = atoi(str);
                break;
            case 1:
                strcpy(p->stu.szName, str);
                break;
            case 2:
                strcpy(p->stu.szSex, str);
                break;
            case 3:
                p->stu.nAge = atoi(str);
                break;
            case 4:
                p->stu.nScore = atoi(str);
                break;
            default:
                // Node *pNewNode = (Node *) malloc(sizeof(Node));
                // if (pNewNode == NULL) {
                //     printf("Memory allocation failed!\n");
                //     return;
                // }
                // pNewNode->pNext = NULL;
                // p->pNext = pNewNode;
                // p = pNewNode;
                // p->stu.nStuNo = atoi(str);
                // i = 0;
                break;
        }
        i++;
    }
    //打印读取结果
    PrintStudent();
}

void CountStudent() {
    int countStu = 0;
    //遍历链表
    Node *p = g_pHead;
    while (p != NULL) {
        countStu++;
        p = p->pNext;
    }
    printf("学生总人数：%d\n\n", countStu);
    pauseProgram();
}


//查找学生信息
void FindStudent() {
    //通过学号查找学生
    int stuNum;
    printf("请输入查找学生学号：");
    scanf("%d", &stuNum);
    //遍历链表查找
    Node* p = g_pHead;
    //对表头进行展示一次
    bool isShowHead = false;
    //记录是否有找到该学号的学生信息
    bool isFindStu = false;
    while (p != NULL) {
        //对比学工号
        if (stuNum == p->stu.nStuNo) {
            if (!isShowHead) {
                printf("*********************************************************************************\n");
                printf("*\t学号\t*\t姓名\t*\t性别\t*\t年龄\t*\t成绩\t*\n");
                printf("*********************************************************************************\n");
                isShowHead = true;
            }
            printf("*\t%d\t*\t%s\t*\t%s\t*\t%d\t*\t%d\t*\n",
                p->stu.nStuNo,
                p->stu.szName,
                p->stu.szSex,
                p->stu.nAge,
                p->stu.nScore
            );
            isFindStu = true;
            printf("*********************************************************************************\n");
        }
        p = p->pNext;
    }
    if (!isFindStu) {
        printf("学号输入有误，系统中暂无该学生信息。\n\n");
    }
    pauseProgram();
    clearScreen();
}
