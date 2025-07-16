#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>
#include <time.h>
#include <errno.h>
#include <sys/stat.h>

// ================================================
// 常量和全局定义（带安全边界）
// ================================================

#define MAX_NAME_LEN 50
#define MAX_TABLE_COUNT 20
#define MAX_COLUMNS_PER_TABLE 15
#define MAX_RECORDS_PER_TABLE 1000
#define MAX_STRING_LENGTH 255
#define MAX_QUERY_LENGTH 512
#define DB_DIRECTORY "./database/"
#define DB_EXTENSION ".db"
#define IDX_EXTENSION ".idx"
#define PATH_BUFFER_SIZE 256

// ================================================
// 数据类型定义
// ================================================

typedef enum {
    INT,
    FLOAT,
    STRING,
    BOOL
} DataType;

typedef union {
    int int_value;
    float float_value;
    char string_value[MAX_STRING_LENGTH];
    bool bool_value;
} DataValue;

typedef struct {
    char name[MAX_NAME_LEN];
    DataType type;
    bool is_indexed;
    bool is_primary;
} ColumnDef;

typedef struct {
    int id;
    DataValue values[MAX_COLUMNS_PER_TABLE];
} Record;

typedef struct {
    char name[MAX_NAME_LEN];
    int column_count;
    ColumnDef columns[MAX_COLUMNS_PER_TABLE];
    int record_count;
    Record records[MAX_RECORDS_PER_TABLE];
} Table;

typedef struct {
    int table_count;
    Table tables[MAX_TABLE_COUNT];
} Database;

typedef enum {
    COMPARISON_EQUAL,
    COMPARISON_NOT_EQUAL,
    COMPARISON_GREATER,
    COMPARISON_LESS,
    COMPARISON_GREATER_OR_EQUAL,
    COMPARISON_LESS_OR_EQUAL,
    COMPARISON_CONTAINS,
    COMPARISON_STARTS_WITH
} ComparisonOperator;

typedef struct {
    char column_name[MAX_NAME_LEN];
    ComparisonOperator op;
    DataValue value;
} Condition;

typedef struct {
    char table_name[MAX_NAME_LEN];
    Condition condition;
} Query;

// ================================================
// 数据库核心函数（带错误处理）
// ================================================

// 安全创建目录
bool create_db_directory() {
    struct stat st = {0};
    if (stat(DB_DIRECTORY, &st) == -1) {
        if (mkdir(DB_DIRECTORY, 0777) == -1) {
            perror("无法创建数据库目录");
            return false;
        }
    }
    return true;
}

// 初始化数据库
void init_database(Database *db) {
    if (!db) return;
    
    db->table_count = 0;
    for (int i = 0; i < MAX_TABLE_COUNT; i++) {
        db->tables[i].record_count = 0;
        db->tables[i].column_count = 0;
        memset(db->tables[i].name, 0, MAX_NAME_LEN);
    }
}

// 查找表
Table* find_table(Database *db, const char *table_name) {
    if (!db || !table_name) return NULL;
    
    for (int i = 0; i < db->table_count; i++) {
        if (strcmp(db->tables[i].name, table_name) == 0) {
            return &db->tables[i];
        }
    }
    return NULL;
}

// 创建新表
int create_table(Database *db, const char *table_name, ColumnDef *columns, int column_count) {
    if (!db || !table_name || column_count <= 0 || column_count > MAX_COLUMNS_PER_TABLE) {
        fprintf(stderr, "无效的创建表参数\n");
        return -1;
    }

    if (db->table_count >= MAX_TABLE_COUNT) {
        fprintf(stderr, "错误：表数量已达上限\n");
        return -1;
    }
    
    if (find_table(db, table_name) != NULL) {
        fprintf(stderr, "错误：表 '%s' 已存在\n", table_name);
        return -1;
    }
    
    Table *table = &db->tables[db->table_count];
    strncpy(table->name, table_name, MAX_NAME_LEN);
    table->column_count = column_count;
    table->record_count = 0;
    
    for (int i = 0; i < column_count; i++) {
        memcpy(&table->columns[i], &columns[i], sizeof(ColumnDef));
    }
    
    return db->table_count++;
}

// 查找列索引
int find_column_index(Table *table, const char *column_name) {
    if (!table || !column_name) return -1;
    
    for (int i = 0; i < table->column_count; i++) {
        if (strcmp(table->columns[i].name, column_name) == 0) {
            return i;
        }
    }
    return -1;
}

// 添加记录
int insert_record(Database *db, const char *table_name, Record *record) {
    if (!db || !table_name || !record) return -1;
    
    Table *table = find_table(db, table_name);
    if (!table) {
        fprintf(stderr, "错误：找不到表\n");
        return -1;
    }
    
    if (table->record_count >= MAX_RECORDS_PER_TABLE) {
        fprintf(stderr, "错误：表 '%s' 记录数已达上限\n", table_name);
        return -1;
    }
    
    // 设置记录ID
    record->id = table->record_count;
    memcpy(&table->records[table->record_count], record, sizeof(Record));
    table->record_count++;
    
    return record->id;
}


// 评估条件（修复布尔值比较）
bool evaluate_condition(Condition *condition, Table *table, Record *record) {
    if (!condition || !table || !record) return false;

    int col_index = find_column_index(table, condition->column_name);
    if (col_index == -1) return false;

    ColumnDef *col_def = &table->columns[col_index];
    DataValue *rec_value = &record->values[col_index];
    DataValue *cond_value = &condition->value;

    switch (col_def->type) {
        case INT:
            switch (condition->op) {
                case COMPARISON_EQUAL:
                    return rec_value->int_value == cond_value->int_value;
                case COMPARISON_NOT_EQUAL:
                    return rec_value->int_value != cond_value->int_value;
                case COMPARISON_GREATER:
                    return rec_value->int_value > cond_value->int_value;
                case COMPARISON_LESS:
                    return rec_value->int_value < cond_value->int_value;
                case COMPARISON_GREATER_OR_EQUAL:
                    return rec_value->int_value >= cond_value->int_value;
                case COMPARISON_LESS_OR_EQUAL:
                    return rec_value->int_value <= cond_value->int_value;
                default:
                    return false;
            }

        case FLOAT:
            switch (condition->op) {
                case COMPARISON_EQUAL:
                    return rec_value->float_value == cond_value->float_value;
                case COMPARISON_NOT_EQUAL:
                    return rec_value->float_value != cond_value->float_value;
                case COMPARISON_GREATER:
                    return rec_value->float_value > cond_value->float_value;
                case COMPARISON_LESS:
                    return rec_value->float_value < cond_value->float_value;
                case COMPARISON_GREATER_OR_EQUAL:
                    return rec_value->float_value >= cond_value->float_value;
                case COMPARISON_LESS_OR_EQUAL:
                    return rec_value->float_value <= cond_value->float_value;
                default:
                    return false;
            }

        case STRING:
            switch (condition->op) {
                case COMPARISON_EQUAL:
                    return strcmp(rec_value->string_value, cond_value->string_value) == 0;
                case COMPARISON_NOT_EQUAL:
                    return strcmp(rec_value->string_value, cond_value->string_value) != 0;
                case COMPARISON_CONTAINS:
                    return strstr(rec_value->string_value, cond_value->string_value) != NULL;
                case COMPARISON_STARTS_WITH:
                    return strncmp(rec_value->string_value, cond_value->string_value,
                                  strlen(cond_value->string_value)) == 0;
                default:
                    return false;
            }

        case BOOL:
            // 修复布尔值比较：确保类型一致
            if (condition->op == COMPARISON_EQUAL) {
                return rec_value->bool_value == (cond_value->int_value != 0);
            }
            return false;
    }
    return false;
}

// 删除记录（优化内存移动）
int delete_records(Database *db, const char *table_name, Query *query) {
    if (!db || !table_name || !query) return 0;
    
    Table *table = find_table(db, table_name);
    if (!table) {
        fprintf(stderr, "错误：找不到表\n");
        return 0;
    }
    
    int deleted = 0;
    for (int i = 0; i < table->record_count; i++) {
        if (evaluate_condition(&query->condition, table, &table->records[i])) {
            // 安全移动记录
            if (table->record_count > 1 && i < table->record_count - 1) {
                memmove(&table->records[i], &table->records[i+1], 
                       (table->record_count - i - 1) * sizeof(Record));
            }
            table->record_count--;
            i--; // 重新检查当前位置
            deleted++;
        }
    }
    return deleted;
}

// 更新记录（添加空指针检查）
int update_records(Database *db, const char *table_name, Query *query, 
                  const char *column_name, DataValue *new_value) {
    if (!db || !table_name || !query || !column_name) return 0;
    
    Table *table = find_table(db, table_name);
    if (!table) {
        fprintf(stderr, "错误：找不到表\n");
        return 0;
    }
    
    int col_index = find_column_index(table, column_name);
    if (col_index == -1) {
        fprintf(stderr, "错误：找不到列\n");
        return 0;
    }
    
    int updated = 0;
    for (int i = 0; i < table->record_count; i++) {
        if (evaluate_condition(&query->condition, table, &table->records[i])) {
            if (new_value) {
                table->records[i].values[col_index] = *new_value;
            }
            updated++;
        }
    }
    return updated;
}


// 查询记录（添加边界检查）
int query_records(Database *db, Query *query, Record *results, int max_results) {
    if (!db || !query || !results || max_results <= 0) return 0;
    
    Table *table = find_table(db, query->table_name);
    if (!table) {
        fprintf(stderr, "表未找到\n");
        return 0;
    }
    
    int found = 0;
    for (int i = 0; i < table->record_count && found < max_results; i++) {
        if (evaluate_condition(&query->condition, table, &table->records[i])) {
            results[found++] = table->records[i];
        }
    }
    return found;
}

// ================================================
// 文件操作函数（增强安全性）
// ================================================

// 安全保存数据库到文件
bool save_database(Database *db) {
    if (!db) return false;
    
    if (!create_db_directory()) {
        return false;
    }
    
    char meta_path[PATH_BUFFER_SIZE];
    snprintf(meta_path, sizeof(meta_path), "%sdatabase.meta", DB_DIRECTORY);
    FILE *meta_file = fopen(meta_path, "wb");
    if (!meta_file) {
        perror("无法打开元数据文件");
        return false;
    }
    
    // 写入表数量
    if (fwrite(&db->table_count, sizeof(int), 1, meta_file) != 1) {
        perror("元数据写入失败");
        fclose(meta_file);
        return false;
    }
    
    for (int t = 0; t < db->table_count; t++) {
        Table *table = &db->tables[t];
        
        // 保存表元数据
        size_t name_len = strlen(table->name) + 1;
        if (fwrite(&name_len, sizeof(size_t), 1, meta_file) != 1 ||
            fwrite(table->name, sizeof(char), name_len, meta_file) != name_len ||
            fwrite(&table->column_count, sizeof(int), 1, meta_file) != 1 ||
            fwrite(&table->record_count, sizeof(int), 1, meta_file) != 1) {
            perror("表元数据写入失败");
            fclose(meta_file);
            return false;
        }
        
        // 保存列定义
        for (int c = 0; c < table->column_count; c++) {
            ColumnDef *col = &table->columns[c];
            if (fwrite(col->name, sizeof(char), MAX_NAME_LEN, meta_file) != MAX_NAME_LEN ||
                fwrite(&col->type, sizeof(DataType), 1, meta_file) != 1 ||
                fwrite(&col->is_indexed, sizeof(bool), 1, meta_file) != 1 ||
                fwrite(&col->is_primary, sizeof(bool), 1, meta_file) != 1) {
                perror("列定义写入失败");
                fclose(meta_file);
                return false;
            }
        }
        
        // 保存数据到单独文件
        char table_file_path[PATH_BUFFER_SIZE];
        snprintf(table_file_path, sizeof(table_file_path), "%s%s%s", DB_DIRECTORY, table->name, DB_EXTENSION);
        FILE *table_file = fopen(table_file_path, "wb");
        if (!table_file) {
            perror("无法打开表数据文件");
            fclose(meta_file);
            return false;
        }
        
        for (int r = 0; r < table->record_count; r++) {
            if (fwrite(&table->records[r].id, sizeof(int), 1, table_file) != 1) {
                perror("记录ID写入失败");
                fclose(table_file);
                fclose(meta_file);
                return false;
            }
            
            for (int c = 0; c < table->column_count; c++) {
                ColumnDef *col_def = &table->columns[c];
                switch (col_def->type) {
                    case INT:
                        if (fwrite(&table->records[r].values[c].int_value, sizeof(int), 1, table_file) != 1) {
                            perror("整数值写入失败");
                            fclose(table_file);
                            fclose(meta_file);
                            return false;
                        }
                        break;
                    case FLOAT:
                        if (fwrite(&table->records[r].values[c].float_value, sizeof(float), 1, table_file) != 1) {
                            perror("浮点数值写入失败");
                            fclose(table_file);
                            fclose(meta_file);
                            return false;
                        }
                        break;
                    case STRING:
                        if (fwrite(table->records[r].values[c].string_value, sizeof(char), MAX_STRING_LENGTH, table_file) != MAX_STRING_LENGTH) {
                            perror("字符串值写入失败");
                            fclose(table_file);
                            fclose(meta_file);
                            return false;
                        }
                        break;
                    case BOOL:
                        if (fwrite(&table->records[r].values[c].bool_value, sizeof(bool), 1, table_file) != 1) {
                            perror("布尔值写入失败");
                            fclose(table_file);
                            fclose(meta_file);
                            return false;
                        }
                        break;
                }
            }
        }
        fclose(table_file);
    }
    
    fclose(meta_file);
    return true;
}

// 安全加载数据库
bool load_database(Database *db) {
    if (!db) return false;
    
    char meta_path[PATH_BUFFER_SIZE];
    snprintf(meta_path, sizeof(meta_path), "%sdatabase.meta", DB_DIRECTORY);
    FILE *meta_file = fopen(meta_path, "rb");
    if (!meta_file) {
        perror("无法打开元数据文件");
        return false;
    }
    
    // 读取表数量
    if (fread(&db->table_count, sizeof(int), 1, meta_file) != 1) {
        perror("元数据读取失败");
        fclose(meta_file);
        return false;
    }
    
    for (int t = 0; t < db->table_count; t++) {
        Table *table = &db->tables[t];
        
        // 加载表元数据
        size_t name_len;
        if (fread(&name_len, sizeof(size_t), 1, meta_file) != 1 ||
            fread(table->name, sizeof(char), name_len, meta_file) != name_len ||
            fread(&table->column_count, sizeof(int), 1, meta_file) != 1 ||
            fread(&table->record_count, sizeof(int), 1, meta_file) != 1) {
            perror("表元数据读取失败");
            fclose(meta_file);
            return false;
        }
        
        // 加载列定义
        for (int c = 0; c < table->column_count; c++) {
            ColumnDef *col = &table->columns[c];
            if (fread(col->name, sizeof(char), MAX_NAME_LEN, meta_file) != MAX_NAME_LEN ||
                fread(&col->type, sizeof(DataType), 1, meta_file) != 1 ||
                fread(&col->is_indexed, sizeof(bool), 1, meta_file) != 1 ||
                fread(&col->is_primary, sizeof(bool), 1, meta_file) != 1) {
                perror("列定义读取失败");
                fclose(meta_file);
                return false;
            }
        }
        
        // 从文件加载数据
        char table_file_path[PATH_BUFFER_SIZE];
        snprintf(table_file_path, sizeof(table_file_path), "%s%s%s", DB_DIRECTORY, table->name, DB_EXTENSION);
        FILE *table_file = fopen(table_file_path, "rb");
        if (!table_file) {
            perror("无法打开表数据文件");
            fclose(meta_file);
            return false;
        }
        
        for (int r = 0; r < table->record_count; r++) {
            if (fread(&table->records[r].id, sizeof(int), 1, table_file) != 1) {
                perror("记录ID读取失败");
                fclose(table_file);
                fclose(meta_file);
                return false;
            }
            
            for (int c = 0; c < table->column_count; c++) {
                ColumnDef *col_def = &table->columns[c];
                switch (col_def->type) {
                    case INT:
                        if (fread(&table->records[r].values[c].int_value, sizeof(int), 1, table_file) != 1) {
                            perror("整数值读取失败");
                            fclose(table_file);
                            fclose(meta_file);
                            return false;
                        }
                        break;
                    case FLOAT:
                        if (fread(&table->records[r].values[c].float_value, sizeof(float), 1, table_file) != 1) {
                            perror("浮点数值读取失败");
                            fclose(table_file);
                            fclose(meta_file);
                            return false;
                        }
                        break;
                    case STRING:
                        if (fread(table->records[r].values[c].string_value, sizeof(char), MAX_STRING_LENGTH, table_file) != MAX_STRING_LENGTH) {
                            perror("字符串值读取失败");
                            fclose(table_file);
                            fclose(meta_file);
                            return false;
                        }
                        table->records[r].values[c].string_value[MAX_STRING_LENGTH-1] = '\0'; // 确保以空字符结尾
                        break;
                    case BOOL:
                        if (fread(&table->records[r].values[c].bool_value, sizeof(bool), 1, table_file) != 1) {
                            perror("布尔值读取失败");
                            fclose(table_file);
                            fclose(meta_file);
                            return false;
                        }
                        break;
                }
            }
        }
        fclose(table_file);
    }
    
    fclose(meta_file);
    return true;
}

// ================================================
// 数据库实用工具函数（带空指针检查）
// ================================================

void print_record(Table *table, Record *record) {
    if (!table || !record) return;
    
    printf("ID: %d\n", record->id);
    for (int i = 0; i < table->column_count; i++) {
        printf("%s: ", table->columns[i].name);
        switch (table->columns[i].type) {
            case INT:
                printf("%d\n", record->values[i].int_value);
                break;
            case FLOAT:
                printf("%.2f\n", record->values[i].float_value);
                break;
            case STRING:
                printf("%s\n", record->values[i].string_value);
                break;
            case BOOL:
                printf("%s\n", record->values[i].bool_value ? "true" : "false");
                break;
        }
    }
    printf("--------------------\n");
}

void print_table_structure(Table *table) {
    if (!table) {
        printf("无效的表\n");
        return;
    }
    
    printf("\nTable: %s\n", table->name);
    printf("Columns:\n");
    for (int i = 0; i < table->column_count; i++) {
        printf("  %s (%s) %s%s\n",
              table->columns[i].name,
              table->columns[i].type == INT ? "INT" : 
              table->columns[i].type == FLOAT ? "FLOAT" : 
              table->columns[i].type == STRING ? "STRING" : "BOOL",
              table->columns[i].is_primary ? "PRIMARY KEY " : "",
              table->columns[i].is_indexed ? "INDEXED" : "");
    }
    printf("Total records: %d\n", table->record_count);
}

// 查询解析函数（增加容错能力）
int parse_query(const char *input, Query *query) {
    if (!input || !query) return -1;
    
    char table_name[64] = {0};
    char column_name[64] = {0};
    char op_str[16] = {0};
    char value_str[128] = {0};
    
    // 安全解析
    if (sscanf(input, "SELECT * FROM %63s WHERE %63s%15s%127[^\n]", 
              table_name, column_name, op_str, value_str) < 4) {
        return -1;
    }
    
    // 复制表名和列名
    strncpy(query->table_name, table_name, MAX_NAME_LEN);
    strncpy(query->condition.column_name, column_name, MAX_NAME_LEN);
    
    // 解析操作符
    if (strcmp(op_str, "=") == 0) query->condition.op = COMPARISON_EQUAL;
    else if (strcmp(op_str, "!=") == 0) query->condition.op = COMPARISON_NOT_EQUAL;
    else if (strcmp(op_str, ">") == 0) query->condition.op = COMPARISON_GREATER;
    else if (strcmp(op_str, "<") == 0) query->condition.op = COMPARISON_LESS;
    else if (strcmp(op_str, ">=") == 0) query->condition.op = COMPARISON_GREATER_OR_EQUAL;
    else if (strcmp(op_str, "<=") == 0) query->condition.op = COMPARISON_LESS_OR_EQUAL;
    else if (strcmp(op_str, "CONTAINS") == 0) query->condition.op = COMPARISON_CONTAINS;
    else if (strcmp(op_str, "STARTSWITH") == 0) query->condition.op = COMPARISON_STARTS_WITH;
    else return -1;
    
    // 安全解析值
    if (strcasecmp(value_str, "true") == 0) {
        query->condition.value.bool_value = true;
    } else if (strcasecmp(value_str, "false") == 0) {
        query->condition.value.bool_value = false;
    } else if (strchr(value_str, '.') != NULL) {
        query->condition.value.float_value = atof(value_str);
    } else {
        query->condition.value.int_value = atoi(value_str);
        strncpy(query->condition.value.string_value, value_str, MAX_STRING_LENGTH);
    }
    
    return 0;
}

// ================================================
// 演示用例和测试函数（优化初始化）
// ================================================

void demo_setup_database(Database *db) {
    if (!db) return;
    
    // 创建users表
    ColumnDef users_columns[] = {
        {"id", INT, true, true},
        {"name", STRING, true, false},
        {"age", INT, false, false},
        {"active", BOOL, false, false}
    };
    int user_col_count = sizeof(users_columns)/sizeof(ColumnDef);
    create_table(db, "users", users_columns, user_col_count);
    
    // 添加用户记录（安全初始化）
    Record user1;
    memset(&user1, 0, sizeof(Record));
    user1.values[0].int_value = 1;
    strncpy(user1.values[1].string_value, "John Doe", MAX_STRING_LENGTH);
    user1.values[2].int_value = 32;
    user1.values[3].bool_value = true;
    insert_record(db, "users", &user1);
    
    Record user2;
    memset(&user2, 0, sizeof(Record));
    user2.values[0].int_value = 2;
    strncpy(user2.values[1].string_value, "Jane Smith", MAX_STRING_LENGTH);
    user2.values[2].int_value = 28;
    user2.values[3].bool_value = true;
    insert_record(db, "users", &user2);
    
    Record user3;
    memset(&user3, 0, sizeof(Record));
    user3.values[0].int_value = 3;
    strncpy(user3.values[1].string_value, "Bob Johnson", MAX_STRING_LENGTH);
    user3.values[2].int_value = 45;
    user3.values[3].bool_value = false;
    insert_record(db, "users", &user3);
    
    // 创建products表
    ColumnDef products_columns[] = {
        {"id", INT, true, true},
        {"name", STRING, true, false},
        {"price", FLOAT, false, false}
    };
    int product_col_count = sizeof(products_columns)/sizeof(ColumnDef);
    create_table(db, "products", products_columns, product_col_count);
    
    // 添加产品记录
    Record product1;
    memset(&product1, 0, sizeof(Record));
    product1.values[0].int_value = 101;
    strncpy(product1.values[1].string_value, "Laptop", MAX_STRING_LENGTH);
    product1.values[2].float_value = 1299.99;
    insert_record(db, "products", &product1);
    
    Record product2;
    memset(&product2, 0, sizeof(Record));
    product2.values[0].int_value = 102;
    strncpy(product2.values[1].string_value, "Smartphone", MAX_STRING_LENGTH);
    product2.values[2].float_value = 799.99;
    insert_record(db, "products", &product2);
}

// ================================================
// 主函数 - 增强错误处理
// ================================================

int main() {
    // 确保数据库目录存在
    if (!create_db_directory()) {
        return EXIT_FAILURE;
    }
    
    Database db;
    init_database(&db);
    
    printf("简易文件数据库系统（修复版）\n");
    printf("==============================\n\n");
    
    // 设置演示数据
    demo_setup_database(&db);
    
    // 打印表结构
    Table *users = find_table(&db, "users");
    if (users) print_table_structure(users);
    
    Table *products = find_table(&db, "products");
    if (products) print_table_structure(products);
    
    // 保存数据库
    if (save_database(&db)) {
        printf("\n数据库已成功保存\n");
        
        // 测试加载
        Database loaded_db;
        init_database(&loaded_db);
        if (load_database(&loaded_db)) {
            printf("数据库已成功加载\n");
            
            // 验证加载的数据
            Table *loaded_table = find_table(&loaded_db, "users");
            if (loaded_table && loaded_table->record_count > 0) {
                printf("\n验证第一条用户记录: ");
                printf("ID=%d, Name=%s\n", 
                      loaded_table->records[0].id,
                      loaded_table->records[0].values[1].string_value);
            }
        } else {
            printf("数据库加载失败\n");
        }
    } else {
        printf("数据库保存失败\n");
    }
    
    return EXIT_SUCCESS;
}
