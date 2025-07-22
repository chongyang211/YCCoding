//
// Created by 杨充 on 2025/7/17.
//

#ifndef SQL_MANAGER_H
#define SQL_MANAGER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_NAME_LENGTH 50
#define MAX_COLUMNS 20
#define MAX_ROWS 1000
#define MAX_STRING_LENGTH 100

// 数据类型枚举
typedef enum {
    //int类型数据
    TYPE_INT,
    //float类型数据
    TYPE_FLOAT,
    //字符串类型数据
    TYPE_STRING
} DataType;


// 列结构体
typedef struct {
    char name[MAX_NAME_LENGTH]; // 列名
    DataType type;              // 数据类型
} Column;

// 行结构体
typedef struct {
    void *data[MAX_COLUMNS]; // 存储每列的数据
} Row;

// 表结构体
typedef struct {
    char name[MAX_NAME_LENGTH]; // 表名
    Column columns[MAX_COLUMNS]; // 列信息
    int columnCount;            // 列数
    Row rows[MAX_ROWS];         // 行数据
    int rowCount;               // 行数
} Table;


// 数据库结构体
typedef struct {
    Table tables[MAX_COLUMNS]; // 所有表
    int tableCount;            // 表数
} Database;

//菜单展示
void Menu();

// 函数声明
//创建表
void createTable(Database *db, const char *tableName, Column *columns, int columnCount);
//插入行数据
void insertRow(Database *db, const char *tableName, void **data);
//删除行数据
void deleteRow(Database *db, const char *tableName, int rowIndex);
//更新行数据
void updateRow(Database *db, const char *tableName, int rowIndex, void **data);
//查询表操作
void queryTable(Database *db, const char *tableName);
// 保存文件数据
void saveDatabase(Database *db, const char *filename);
// 读取文件数据
void loadDatabase(Database *db, const char *filename);


#endif //SQL_MANAGER_H
