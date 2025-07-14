//
// Created by 杨充 on 2025/7/13.
//
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#ifndef BANKMANAGER_H
#define BANKMANAGER_H

//最多用户不超过100
#define MAX_ACCOUNTS 100
//用户名最多是50位
#define NAME_LENGTH 50
//用户文件名称
#define FILENAME "bank_accounts.txt"

//定义用户结构体
typedef struct {
    int accountNumber;  //用户卡号
    char name[NAME_LENGTH]; //用户名
    double balance; //余额
} BankAccount;

//展示菜单
void showMenu();

// 开户
void openAccount();

// 存款
void deposit();

// 取款
void withdraw();

// 查询余额
void checkBalance();

#endif //BANKMANAGER_H
