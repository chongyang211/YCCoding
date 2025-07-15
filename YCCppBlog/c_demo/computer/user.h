//
// Created by 杨充 on 2025/7/15.
//
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// 用户结构体
typedef struct {
    char id[20];
    char name[50];
    char password[50];
} User;

// 教师结构体
typedef struct {
    User user;
    int approvalCount;
} Teacher;

// 学生结构体
typedef struct {
    User user;
    int reservationCount;
} Student;

// 管理员结构体
typedef struct {
    User user;
} Admin;

// 用户类型枚举
typedef enum {
    USER_NONE,
    STUDENT_USER,
    TEACHER_USER,
    ADMIN_USER
} UserType;
