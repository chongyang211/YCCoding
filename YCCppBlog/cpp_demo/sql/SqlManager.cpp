//
// Created by 杨充 on 2025/7/17.
//

#include "SqlManager.h"

// 创建表
void Database::createTable(const std::string& tableName, const std::vector<Column>& columns) {
    Table* table = new Table(tableName);
    table->columns = columns;
    tables.push_back(table);
    std::cout << "表 '" << tableName << "' 创建成功！" << std::endl;
}

// 插入行
void Database::insertRow(const std::string& tableName, const std::vector<void*>& data) {
    for (size_t i = 0; i < tables.size(); i++) {
        if (tables[i]->name == tableName) {
            Row* row = new Row();
            row->columnTypes.resize(tables[i]->columns.size());
            for (size_t j = 0; j < tables[i]->columns.size(); j++) {
                row->columnTypes[j] = tables[i]->columns[j].type;
                switch (tables[i]->columns[j].type) {
                    case TYPE_INT:
                        row->data.push_back(new int(*(int*)data[j]));
                        break;
                    case TYPE_FLOAT:
                        row->data.push_back(new float(*(float*)data[j]));
                        break;
                    case TYPE_STRING:
                        row->data.push_back(new std::string(*(std::string*)data[j]));
                        break;
                }
            }
            tables[i]->rows.push_back(row);
            std::cout << "行插入成功！" << std::endl;
            return;
        }
    }
    std::cout << "表 '" << tableName << "' 不存在！" << std::endl;
}

// 删除行
void Database::deleteRow(const std::string& tableName, int rowIndex) {
    for (size_t i = 0; i < tables.size(); i++) {
        if (tables[i]->name == tableName) {
            if (rowIndex >= 0 && rowIndex < tables[i]->rows.size()) {
                delete tables[i]->rows[rowIndex];
                tables[i]->rows.erase(tables[i]->rows.begin() + rowIndex);
                std::cout << "行删除成功！" << std::endl;
            } else {
                std::cout << "行索引无效！" << std::endl;
            }
            return;
        }
    }
    std::cout << "表 '" << tableName << "' 不存在！" << std::endl;
}

// 更新行
void Database::updateRow(const std::string& tableName, int rowIndex, const std::vector<void*>& data) {
    for (size_t i = 0; i < tables.size(); i++) {
        if (tables[i]->name == tableName) {
            if (rowIndex >= 0 && rowIndex < tables[i]->rows.size()) {
                Row* row = tables[i]->rows[rowIndex];
                for (size_t j = 0; j < tables[i]->columns.size(); j++) {
                    switch (tables[i]->columns[j].type) {
                        case TYPE_INT:
                            *(int*)row->data[j] = *(int*)data[j];
                            break;
                        case TYPE_FLOAT:
                            *(float*)row->data[j] = *(float*)data[j];
                            break;
                        case TYPE_STRING:
                            *(std::string*)row->data[j] = *(std::string*)data[j];
                            break;
                    }
                }
                std::cout << "行更新成功！" << std::endl;
            } else {
                std::cout << "行索引无效！" << std::endl;
            }
            return;
        }
    }
    std::cout << "表 '" << tableName << "' 不存在！" << std::endl;
}

// 查询表
void Database::queryTable(const std::string& tableName) {
    for (size_t i = 0; i < tables.size(); i++) {
        if (tables[i]->name == tableName) {
            std::cout << "表 '" << tableName << "' 数据：" << std::endl;
            for (size_t j = 0; j < tables[i]->columns.size(); j++) {
                std::cout << tables[i]->columns[j].name << "\t";
            }
            std::cout << std::endl;

            for (size_t j = 0; j < tables[i]->rows.size(); j++) {
                for (size_t k = 0; k < tables[i]->columns.size(); k++) {
                    switch (tables[i]->columns[k].type) {
                        case TYPE_INT:
                            std::cout << *(int*)tables[i]->rows[j]->data[k] << "\t";
                            break;
                        case TYPE_FLOAT:
                            std::cout << *(float*)tables[i]->rows[j]->data[k] << "\t";
                            break;
                        case TYPE_STRING:
                            std::cout << *(std::string*)tables[i]->rows[j]->data[k] << "\t";
                            break;
                    }
                }
                std::cout << std::endl;
            }
            return;
        }
    }
    std::cout << "表 '" << tableName << "' 不存在！" << std::endl;
}

// 保存数据库到文件
void Database::saveToFile(const std::string& filename) {
    std::ofstream file(filename.c_str(), std::ios::binary);
    if (!file) {
        std::cout << "无法打开文件 '" << filename << "'！" << std::endl;
        return;
    }

    size_t tableCount = tables.size();
    file.write((char*)&tableCount, sizeof(tableCount));

    for (size_t i = 0; i < tables.size(); i++) {
        Table* table = tables[i];
        size_t nameLength = table->name.size();
        file.write((char*)&nameLength, sizeof(nameLength));
        file.write(table->name.c_str(), nameLength);

        size_t columnCount = table->columns.size();
        file.write((char*)&columnCount, sizeof(columnCount));

        for (size_t j = 0; j < table->columns.size(); j++) {
            const Column& column = table->columns[j];
            size_t columnNameLength = column.name.size();
            file.write((char*)&columnNameLength, sizeof(columnNameLength));
            file.write(column.name.c_str(), columnNameLength);

            file.write((char*)&column.type, sizeof(column.type));
        }

        size_t rowCount = table->rows.size();
        file.write((char*)&rowCount, sizeof(rowCount));

        for (size_t j = 0; j < table->rows.size(); j++) {
            Row* row = table->rows[j];
            for (size_t k = 0; k < table->columns.size(); k++) {
                switch (table->columns[k].type) {
                    case TYPE_INT:
                        file.write((char*)row->data[k], sizeof(int));
                        break;
                    case TYPE_FLOAT:
                        file.write((char*)row->data[k], sizeof(float));
                        break;
                    case TYPE_STRING: {
                        std::string* str = (std::string*)row->data[k];
                        size_t strLength = str->size();
                        file.write((char*)&strLength, sizeof(strLength));
                        file.write(str->c_str(), strLength);
                        break;
                    }
                }
            }
        }
    }

    file.close();
    std::cout << "数据库保存成功！" << std::endl;
}

// 从文件加载数据库
void Database::loadFromFile(const std::string& filename) {
    std::ifstream file(filename.c_str(), std::ios::binary);
    if (!file) {
        std::cout << "无法打开文件 '" << filename << "'！" << std::endl;
        return;
    }

    size_t tableCount;
    file.read((char*)&tableCount, sizeof(tableCount));

    for (size_t i = 0; i < tableCount; i++) {
        size_t nameLength;
        file.read((char*)&nameLength, sizeof(nameLength));
        char* nameBuffer = new char[nameLength + 1];
        file.read(nameBuffer, nameLength);
        nameBuffer[nameLength] = '\0';
        std::string tableName(nameBuffer);
        delete[] nameBuffer;

        Table* table = new Table(tableName);

        size_t columnCount;
        file.read((char*)&columnCount, sizeof(columnCount));

        for (size_t j = 0; j < columnCount; j++) {
            size_t columnNameLength;
            file.read((char*)&columnNameLength, sizeof(columnNameLength));
            char* columnNameBuffer = new char[columnNameLength + 1];
            file.read(columnNameBuffer, columnNameLength);
            columnNameBuffer[columnNameLength] = '\0';
            std::string columnName(columnNameBuffer);
            delete[] columnNameBuffer;

            DataType columnType;
            file.read((char*)&columnType, sizeof(columnType));

            table->columns.push_back(Column(columnName, columnType));
        }

        size_t rowCount;
        file.read((char*)&rowCount, sizeof(rowCount));

        for (size_t j = 0; j < rowCount; j++) {
            Row* row = new Row();
            for (size_t k = 0; k < table->columns.size(); k++) {
                const Column& column = table->columns[k];
                switch (column.type) {
                    case TYPE_INT: {
                        int* value = new int;
                        file.read((char*)value, sizeof(int));
                        row->data.push_back(value);
                        break;
                    }
                    case TYPE_FLOAT: {
                        float* value = new float;
                        file.read((char*)value, sizeof(float));
                        row->data.push_back(value);
                        break;
                    }
                    case TYPE_STRING: {
                        size_t strLength;
                        file.read((char*)&strLength, sizeof(strLength));
                        char* strBuffer = new char[strLength + 1];
                        file.read(strBuffer, strLength);
                        strBuffer[strLength] = '\0';
                        std::string* value = new std::string(strBuffer);
                        delete[] strBuffer;
                        row->data.push_back(value);
                        break;
                    }
                }
            }
            table->rows.push_back(row);
        }

        tables.push_back(table);
    }

    file.close();
    std::cout << "数据库加载成功！" << std::endl;
}

int main() {
    Database db;

    // 示例：创建表
    std::vector<Column> columns;
    columns.push_back(Column("ID", TYPE_INT));
    columns.push_back(Column("Name", TYPE_STRING));
    columns.push_back(Column("Age", TYPE_INT));
    db.createTable("Users", columns);

    // 示例：插入行
    std::vector<void*> row1;
    row1.push_back(new int(1));
    row1.push_back(new std::string("Alice"));
    row1.push_back(new int(25));
    db.insertRow("Users", row1);

    std::vector<void*> row2;
    row2.push_back(new int(2));
    row2.push_back(new std::string("Bob"));
    row2.push_back(new int(30));
    db.insertRow("Users", row2);

    // 示例：查询表
    db.queryTable("Users");

    // 示例：保存数据库
    db.saveToFile("database.dat");

    // 示例：加载数据库
    Database newDb;
    newDb.loadFromFile("database.dat");
    newDb.queryTable("Users");

    // 释放动态分配的内存
    for (size_t i = 0; i < row1.size(); i++) {
        if (row1[i]) {
            switch (i) {
                case 0: // int
                    delete static_cast<int*>(row1[i]);
                break;
                case 1: // std::string
                    delete static_cast<std::string*>(row1[i]);
                break;
                case 2: // int
                    delete static_cast<int*>(row1[i]);
                break;
            }
        }
    }

    for (size_t i = 0; i < row2.size(); i++) {
        if (row2[i]) {
            switch (i) {
                case 0: // int
                    delete static_cast<int*>(row2[i]);
                break;
                case 1: // std::string
                    delete static_cast<std::string*>(row2[i]);
                break;
                case 2: // int
                    delete static_cast<int*>(row2[i]);
                break;
            }
        }
    }

    return 0;
}