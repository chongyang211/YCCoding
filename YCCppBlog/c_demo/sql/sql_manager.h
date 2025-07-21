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
    TYPE_INT,
    TYPE_FLOAT,
    TYPE_STRING
} DataType;

// 列结构体
typedef struct {
    char name[MAX_NAME_LENGTH];
    DataType type;
} Column;

// 行结构体
typedef struct {
    void *data[MAX_COLUMNS]; // 存储每列的数据
} Row;

// 表结构体
typedef struct {
    char name[MAX_NAME_LENGTH];
    Column columns[MAX_COLUMNS];
    int columnCount;
    Row rows[MAX_ROWS];
    int rowCount;
} Table;

// 数据库结构体
typedef struct {
    Table tables[MAX_COLUMNS];
    int tableCount;
} Database;

//菜单展示
void Menu();

// 函数声明
void createTable(Database *db, const char *tableName, Column *columns, int columnCount);
void insertRow(Database *db, const char *tableName, void **data);
void deleteRow(Database *db, const char *tableName, int rowIndex);
void updateRow(Database *db, const char *tableName, int rowIndex, void **data);
void queryTable(Database *db, const char *tableName);
void saveDatabase(Database *db, const char *filename);
void loadDatabase(Database *db, const char *filename);


#endif //SQL_MANAGER_H
