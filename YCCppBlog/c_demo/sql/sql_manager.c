#include "sql_manager.h"

#include "../bank/BankManager.h"

// 创建表
void createTable(Database *db, const char *tableName, Column *columns, int columnCount) {
    if (db->tableCount >= MAX_COLUMNS) {
        printf("无法创建表：数据库表数量已达上限！\n");
        return;
    }
    //获取索引
    int tableCount = db->tableCount;
    //获取目标表指针
    Table *table = &db->tables[tableCount];
    strncpy(table->name, tableName, MAX_NAME_LENGTH);
    table->columnCount = columnCount;
    for (int i = 0; i < columnCount; i++) {
        strncpy(table->columns[i].name, columns[i].name, MAX_NAME_LENGTH);
        table->columns[i].type = columns[i].type;
    }
    table->rowCount = 0;
    db->tableCount++;
    printf("表 '%s' 创建成功！\n", tableName);
}

// 插入行
void insertRow(Database *db, const char *tableName, void **data) {
    Table *table = NULL;
    //遍历查找目标表
    for (int i = 0; i < db->tableCount; i++) {
        if (strcmp(db->tables[i].name, tableName) == 0) {
            table = &db->tables[i];
            break;
        }
    }

    if (!table) {
        printf("表 '%s' 不存在！\n", tableName);
        return;
    }
    if (table->rowCount >= MAX_ROWS) {
        printf("无法插入行：表 '%s' 行数已达上限！\n", tableName);
        return;
    }

    Row *row = &table->rows[table->rowCount];
    for (int i = 0; i < table->columnCount; i++) {
        switch (table->columns[i].type) {
            case TYPE_INT:
                row->data[i] = malloc(sizeof(int));
                *(int *)row->data[i] = *(int *)data[i];
                break;
            case TYPE_FLOAT:
                row->data[i] = malloc(sizeof(float));
                *(float *)row->data[i] = *(float *)data[i];
                break;
            case TYPE_STRING:
                row->data[i] = malloc(MAX_STRING_LENGTH);
                strncpy((char *)row->data[i], (char *)data[i], MAX_STRING_LENGTH);
                break;
        }
    }
    table->rowCount++;
    printf("行插入成功！\n");
}

// 删除行
void deleteRow(Database *db, const char *tableName, int rowIndex) {
    Table *table = NULL;
    for (int i = 0; i < db->tableCount; i++) {
        if (strcmp(db->tables[i].name, tableName) == 0) {
            table = &db->tables[i];
            break;
        }
    }
    if (!table) {
        printf("表 '%s' 不存在！\n", tableName);
        return;
    }
    if (rowIndex < 0 || rowIndex >= table->rowCount) {
        printf("行索引无效！\n");
        return;
    }
    for (int i = rowIndex; i < table->rowCount - 1; i++) {
        table->rows[i] = table->rows[i + 1];
    }
    table->rowCount--;
    printf("行删除成功！\n");
}

// 更新行
void updateRow(Database *db, const char *tableName, int rowIndex, void **data) {
    Table *table = NULL;
    for (int i = 0; i < db->tableCount; i++) {
        if (strcmp(db->tables[i].name, tableName) == 0) {
            table = &db->tables[i];
            break;
        }
    }
    if (!table) {
        printf("表 '%s' 不存在！\n", tableName);
        return;
    }
    if (rowIndex < 0 || rowIndex >= table->rowCount) {
        printf("行索引无效！\n");
        return;
    }
    Row *row = &table->rows[rowIndex];
    for (int i = 0; i < table->columnCount; i++) {
        switch (table->columns[i].type) {
            case TYPE_INT:
                *(int *)row->data[i] = *(int *)data[i];
                break;
            case TYPE_FLOAT:
                *(float *)row->data[i] = *(float *)data[i];
                break;
            case TYPE_STRING:
                strncpy((char *)row->data[i], (char *)data[i], MAX_STRING_LENGTH);
                break;
        }
    }
    printf("行更新成功！\n");
}

// 查询表
void queryTable(Database *db, const char *tableName) {
    Table *table = NULL;
    for (int i = 0; i < db->tableCount; i++) {
        if (strcmp(db->tables[i].name, tableName) == 0) {
            table = &db->tables[i];
            break;
        }
    }
    if (!table) {
        printf("表 '%s' 不存在！\n", tableName);
        return;
    }
    printf("表 '%s' 数据：\n", tableName);
    for (int i = 0; i < table->columnCount; i++) {
        printf("%-20s", table->columns[i].name);
    }
    printf("\n");

    for (int i = 0; i < table->rowCount; i++) {
        Row *row = &table->rows[i];
        for (int j = 0; j < table->columnCount; j++) {
            switch (table->columns[j].type) {
                case TYPE_INT:
                    printf("%-20d", *(int *)row->data[j]);
                    break;
                case TYPE_FLOAT:
                    printf("%-20.2f", *(float *)row->data[j]);
                    break;
                case TYPE_STRING:
                    printf("%-20s", (char *)row->data[j]);
                    break;
            }
        }
        printf("\n");
    }
}


// 保存数据库到文件
void saveDatabase(Database *db, const char *filename) {
    //打开文件
    FILE *file = fopen(filename, "wb");
    if (!file) {
        printf("无法打开文件 '%s'！\n", filename);
        return;
    }
    fwrite(&db->tableCount, sizeof(int), 1, file);
    for (int i = 0; i < db->tableCount; i++) {
        Table *table = &db->tables[i];
        fwrite(table->name, sizeof(char), MAX_NAME_LENGTH, file);
        fwrite(&table->columnCount, sizeof(int), 1, file);
        fwrite(table->columns, sizeof(Column), MAX_COLUMNS, file);
        fwrite(&table->rowCount, sizeof(int), 1, file);
        //写行数据
        for (int j = 0; j < table->rowCount; j++) {
            Row *row = &table->rows[j];
            for (int k = 0; k < table->columnCount; k++) {
                switch (table->columns[k].type) {
                    case TYPE_INT:
                        fwrite(row->data[k], sizeof(int), 1, file);
                        break;
                    case TYPE_FLOAT:
                        fwrite(row->data[k], sizeof(float), 1, file);
                        break;
                    case TYPE_STRING:
                        fwrite(row->data[k], sizeof(char), MAX_STRING_LENGTH, file);
                        break;
                }
            }
        }
    }
    fclose(file);
    printf("数据库保存成功！\n");
}

// 从文件加载数据库
void loadDatabase(Database *db, const char *filename) {
    FILE *file = fopen(filename, "rb");
    if (!file) {
        printf("无法打开文件 '%s'！\n", filename);
        return;
    }
    fread(&db->tableCount, sizeof(int), 1, file);
    for (int i = 0; i < db->tableCount; i++) {
        Table *table = &db->tables[i];
        fread(table->name, sizeof(char), MAX_NAME_LENGTH, file);
        fread(&table->columnCount, sizeof(int), 1, file);
        fread(table->columns, sizeof(Column), MAX_COLUMNS, file);
        fread(&table->rowCount, sizeof(int), 1, file);
        for (int j = 0; j < table->rowCount; j++) {
            Row *row = &table->rows[j];
            for (int k = 0; k < table->columnCount; k++) {
                switch (table->columns[k].type) {
                    case TYPE_INT:
                        row->data[k] = malloc(sizeof(int));
                        fread(row->data[k], sizeof(int), 1, file);
                        break;
                    case TYPE_FLOAT:
                        row->data[k] = malloc(sizeof(float));
                        fread(row->data[k], sizeof(float), 1, file);
                        break;
                    case TYPE_STRING:
                        row->data[k] = malloc(MAX_STRING_LENGTH);
                        fread(row->data[k], sizeof(char), MAX_STRING_LENGTH, file);
                        break;
                }
            }
        }
    }
    fclose(file);
    printf("数据库加载成功！\n");
}

// 暂停程序
void pauseProgram() {
    printf("Press Enter to continue...\n");
    getchar();
}

// 清屏
void clearScreen() {
    system("clear"); // 或使用 printf("\033[H\033[J");
}

Database db = {0};
const char *filename = "database.txt";

void userCreateTable() {
    // 示例：创建表
    Column columns[] = {
        {"ID", TYPE_INT},
        {"Name", TYPE_STRING},
        {"Age", TYPE_INT}
    };
    createTable(&db, "Users", columns, 3);
}

void userInsertRow() {
    // 示例：插入行
    void *row1[] = {&(int){1}, "Alice", &(int){25}};
    void *row2[] = {&(int){2}, "Bob", &(int){30}};
    insertRow(&db, "Users", row1);
    insertRow(&db, "Users", row2);
}

void userSaveFile() {
    // 示例：保存数据库
    saveDatabase(&db, filename);
}

void test() {
    Database db = {0};
    const char *filename = "database.dat";

    // 示例：创建表
    Column columns[] = {
        {"ID", TYPE_INT},
        {"Name", TYPE_STRING},
        {"Age", TYPE_INT}
    };
    createTable(&db, "Users", columns, 3);

    // 示例：插入行
    void *row1[] = {&(int){1}, "Alice", &(int){25}};
    void *row2[] = {&(int){2}, "Bob", &(int){30}};
    insertRow(&db, "Users", row1);
    insertRow(&db, "Users", row2);

    // 示例：查询表
    queryTable(&db, "Users");

    // 示例：保存数据库
    saveDatabase(&db, filename);

    // 示例：加载数据库
    Database newDb = {0};
    loadDatabase(&newDb, filename);
    queryTable(&newDb, "Users");
}


int main() {
    int choice;
    while (1) {
        //打印菜单
        Menu();
        //读取控制台字符
        scanf("%d", &choice);
        printf("输入的字符是：%c \n", choice);
        switch (choice) {
            case 1: //1.创建表
                printf("创建表\n");
                userCreateTable();
                break;
            case 2: //2.插入行
                printf("插入行\n");
                userInsertRow();
                break;
            case 3: //3.删除行
                printf("删除行\n");
                break;
            case 4: //4.更新行
                printf("更新行\n");
                break;
            case 5: //5.查询表
                printf("查询表\n");
                break;
            case 6: //6.保存数据到文件
                printf("保存数据到文件\n");
                userSaveFile();
                break;
            case 7: //7.从文件读取数据
                printf("从文件读取数据\n");
                break;
            case 0: //0.退出系统
                printf("退出系统\n");
                return 0;
            default:
                printf("输入有误，没有该功能\n\n");
                break;
        }
    }

    //
    // // 示例：查询表
    // queryTable(&db, "Users");
    //
    //
    // // 示例：加载数据库
    // Database newDb = {0};
    // loadDatabase(&newDb, filename);
    // queryTable(&newDb, "Users");

    return 0;
}

//菜单
void Menu() {
    printf("*************************************************\n");
    printf("*\t欢迎简易数据库系统1.0\t*\n");
    printf("*\t\t请选择功能\t\t\t*\n");
    printf("*************************************************\n");
    printf("*\t\t1.创建表\t\t\t*\n");
    printf("*\t\t2.插入行\t\t\t*\n");
    printf("*\t\t3.删除行\t\t\t*\n");
    printf("*\t\t4.更新行\t\t\t*\n");
    printf("*\t\t5.查询表\t\t\t*\n");
    printf("*\t\t6.保存数据到文件\t\t*\n");
    printf("*\t\t7.从文件读取数据\t\t*\n");
    printf("*\t\t0.退出系统\t\t\t*\n");
    printf("*************************************************\n");
}

