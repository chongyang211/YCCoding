//
// Created by 杨充 on 2025/7/17.
//

#ifndef SQLMANAGER_H
#define SQLMANAGER_H

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <cstring>

#define MAX_NAME_LENGTH 50
#define MAX_STRING_LENGTH 100

// 数据类型枚举
enum DataType {
    TYPE_INT,
    TYPE_FLOAT,
    TYPE_STRING
};

// 列类
class Column {
public:
    std::string name;
    DataType type;

    Column(const std::string& name, DataType type) : name(name), type(type) {}
};

// 行类
class Row {
public:
    std::vector<void*> data;
    std::vector<DataType> columnTypes;

    ~Row() {
        for (size_t i = 0; i < data.size(); i++) {
            if (data[i]) {
                switch (columnTypes[i]) {
                    case TYPE_INT:
                        delete static_cast<int*>(data[i]);
                        break;
                    case TYPE_FLOAT:
                        delete static_cast<float*>(data[i]);
                        break;
                    case TYPE_STRING:
                        delete static_cast<std::string*>(data[i]);
                        break;
                }
            }
        }
    }
};

// 表类
class Table {
public:
    std::string name;
    std::vector<Column> columns;
    std::vector<Row*> rows;

    Table(const std::string& name) : name(name) {}

    ~Table() {
        for (size_t i = 0; i < rows.size(); i++) {
            if (rows[i]) delete rows[i];
        }
    }
};

// 数据库类
class Database {
public:
    std::vector<Table*> tables;

    ~Database() {
        for (size_t i = 0; i < tables.size(); i++) {
            if (tables[i]) delete tables[i];
        }
    }

    void createTable(const std::string& tableName, const std::vector<Column>& columns);
    void insertRow(const std::string& tableName, const std::vector<void*>& data);
    void deleteRow(const std::string& tableName, int rowIndex);
    void updateRow(const std::string& tableName, int rowIndex, const std::vector<void*>& data);
    void queryTable(const std::string& tableName);
    void saveToFile(const std::string& filename);
    void loadFromFile(const std::string& filename);
};

#endif //SQLMANAGER_H
