//
// Created by 杨充 on 2025/7/15.
//
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>
#include <time.h>

// ================================================
// 常量和全局定义
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
// 数据库核心函数
// ================================================

// 初始化数据库
void init_database(Database *db) {
    db->table_count = 0;
    for (int i = 0; i < MAX_TABLE_COUNT; i++) {
        db->tables[i].record_count = 0;
        db->tables[i].column_count = 0;
        memset(db->tables[i].name, 0, MAX_NAME_LEN);
    }
}

// 查找表
Table* find_table(Database *db, const char *table_name) {
    for (int i = 0; i < db->table_count; i++) {
        if (strcmp(db->tables[i].name, table_name) == 0) {
            return &db->tables[i];
        }
    }
    return NULL;
}

// 创建新表
int create_table(Database *db, const char *table_name, ColumnDef *columns, int column_count) {
    if (db->table_count >= MAX_TABLE_COUNT) {
        fprintf(stderr, "Error: Maximum table count reached\n");
        return -1;
    }

    if (find_table(db, table_name) != NULL) {
        fprintf(stderr, "Error: Table '%s' already exists\n", table_name);
        return -1;
    }

    if (column_count <= 0 || column_count > MAX_COLUMNS_PER_TABLE) {
        fprintf(stderr, "Error: Invalid column count\n");
        return -1;
    }

    Table *table = &db->tables[db->table_count];
    strncpy(table->name, table_name, MAX_NAME_LEN);
    table->column_count = column_count;
    table->record_count = 0;

    for (int i = 0; i < column_count; i++) {
        table->columns[i] = columns[i];
    }

    db->table_count++;
    return db->table_count - 1;
}

// 查找列索引
int find_column_index(Table *table, const char *column_name) {
    for (int i = 0; i < table->column_count; i++) {
        if (strcmp(table->columns[i].name, column_name) == 0) {
            return i;
        }
    }
    return -1;
}

// 创建索引
void create_index(Table *table, const char *column_name) {
    int col_index = find_column_index(table, column_name);
    if (col_index == -1) {
        fprintf(stderr, "Error: Column not found\n");
        return;
    }
    table->columns[col_index].is_indexed = true;
}

// 删除索引
void drop_index(Table *table, const char *column_name) {
    int col_index = find_column_index(table, column_name);
    if (col_index == -1) {
        fprintf(stderr, "Error: Column not found\n");
        return;
    }
    table->columns[col_index].is_indexed = false;
}

// 添加记录
int insert_record(Database *db, const char *table_name, Record *record) {
    Table *table = find_table(db, table_name);
    if (!table) {
        fprintf(stderr, "Error: Table not found\n");
        return -1;
    }

    if (table->record_count >= MAX_RECORDS_PER_TABLE) {
        fprintf(stderr, "Error: Maximum record count reached for table '%s'\n", table_name);
        return -1;
    }

    // 设置记录ID
    record->id = table->record_count;
    table->records[table->record_count] = *record;
    table->record_count++;

    return record->id;
}


// 评估条件
bool evaluate_condition(Condition *condition, Table *table, Record *record) {
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
                    return false; // 其他操作符不支持数值类型
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
            if (condition->op == COMPARISON_EQUAL) {
                return rec_value->bool_value == cond_value->bool_value;
            }
            return false;
    }

    return false;
}


// 删除记录
int delete_records(Database *db, const char *table_name, Query *query) {
    Table *table = find_table(db, table_name);
    if (!table) {
        fprintf(stderr, "Error: Table not found\n");
        return 0;
    }

    int deleted = 0;
    for (int i = 0; i < table->record_count; i++) {
        if (evaluate_condition(&query->condition, table, &table->records[i])) {
            // 移动记录填充空缺
            for (int j = i; j < table->record_count - 1; j++) {
                table->records[j] = table->records[j + 1];
            }
            table->record_count--;
            i--;  // 重新检查当前位置
            deleted++;
        }
    }

    return deleted;
}

// 更新记录
int update_records(Database *db, const char *table_name, Query *query,
                  const char *column_name, DataValue *new_value) {
    Table *table = find_table(db, table_name);
    if (!table) {
        fprintf(stderr, "Error: Table not found\n");
        return 0;
    }

    int col_index = find_column_index(table, column_name);
    if (col_index == -1) {
        fprintf(stderr, "Error: Column not found\n");
        return 0;
    }

    DataType expected_type = table->columns[col_index].type;
    if (new_value) {
        // 类型检查 (简单版本)
        // if ((expected_type == INT && new_value->int_value != new_value->int_value) ||
        //     (expected_type == FLOAT && new_value->float_value != new_value->float_value)) {
        if ((expected_type == INT) ||
            (expected_type == FLOAT)) {
            fprintf(stderr, "Type mismatch\n");
            return 0;
        }
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

// 查询记录
int query_records(Database *db, Query *query, Record *results, int max_results) {
    Table *table = find_table(db, query->table_name);
    if (!table) {
        fprintf(stderr, "Table not found\n");
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
// 文件操作函数
// ================================================

// 保存数据库到文件
bool save_database(Database *db) {
    FILE *meta_file = fopen(DB_DIRECTORY "database.meta", "wb");
    if (!meta_file) {
        perror("Error opening meta file");
        return false;
    }

    fwrite(&db->table_count, sizeof(int), 1, meta_file);

    for (int t = 0; t < db->table_count; t++) {
        Table *table = &db->tables[t];

        // 保存表元数据
        size_t name_len = strlen(table->name) + 1;
        fwrite(&name_len, sizeof(size_t), 1, meta_file);
        fwrite(table->name, sizeof(char), name_len, meta_file);

        fwrite(&table->column_count, sizeof(int), 1, meta_file);
        fwrite(&table->record_count, sizeof(int), 1, meta_file);

        // 保存列定义
        for (int c = 0; c < table->column_count; c++) {
            ColumnDef *col = &table->columns[c];
            fwrite(col->name, sizeof(char), MAX_NAME_LEN, meta_file);
            fwrite(&col->type, sizeof(DataType), 1, meta_file);
            fwrite(&col->is_indexed, sizeof(bool), 1, meta_file);
            fwrite(&col->is_primary, sizeof(bool), 1, meta_file);
        }

        // 保存数据到单独文件
        char table_file_path[256];
        snprintf(table_file_path, sizeof(table_file_path),
                DB_DIRECTORY "%s" DB_EXTENSION, table->name);

        FILE *table_file = fopen(table_file_path, "wb");
        if (!table_file) {
            perror("Error opening table file");
            fclose(meta_file);
            return false;
        }

        for (int r = 0; r < table->record_count; r++) {
            fwrite(&table->records[r].id, sizeof(int), 1, table_file);

            for (int c = 0; c < table->column_count; c++) {
                ColumnDef *col_def = &table->columns[c];
                switch (col_def->type) {
                    case INT:
                        fwrite(&table->records[r].values[c].int_value, sizeof(int), 1, table_file);
                        break;
                    case FLOAT:
                        fwrite(&table->records[r].values[c].float_value, sizeof(float), 1, table_file);
                        break;
                    case STRING:
                        fwrite(table->records[r].values[c].string_value, sizeof(char), MAX_STRING_LENGTH, table_file);
                        break;
                    case BOOL:
                        fwrite(&table->records[r].values[c].bool_value, sizeof(bool), 1, table_file);
                        break;
                }
            }
        }
        fclose(table_file);
    }

    fclose(meta_file);
    return true;
}

// 从文件加载数据库
bool load_database(Database *db) {
    FILE *meta_file = fopen(DB_DIRECTORY "database.meta", "rb");
    if (!meta_file) {
        perror("Error opening meta file");
        return false;
    }

    fread(&db->table_count, sizeof(int), 1, meta_file);

    for (int t = 0; t < db->table_count; t++) {
        Table *table = &db->tables[t];

        // 加载表元数据
        size_t name_len;
        fread(&name_len, sizeof(size_t), 1, meta_file);
        fread(table->name, sizeof(char), name_len, meta_file);

        fread(&table->column_count, sizeof(int), 1, meta_file);
        fread(&table->record_count, sizeof(int), 1, meta_file);

        // 加载列定义
        for (int c = 0; c < table->column_count; c++) {
            ColumnDef *col = &table->columns[c];
            fread(col->name, sizeof(char), MAX_NAME_LEN, meta_file);
            fread(&col->type, sizeof(DataType), 1, meta_file);
            fread(&col->is_indexed, sizeof(bool), 1, meta_file);
            fread(&col->is_primary, sizeof(bool), 1, meta_file);
        }

        // 从文件加载数据
        char table_file_path[256];
        snprintf(table_file_path, sizeof(table_file_path),
                DB_DIRECTORY "%s" DB_EXTENSION, table->name);

        FILE *table_file = fopen(table_file_path, "rb");
        if (!table_file) {
            perror("Error opening table file");
            fclose(meta_file);
            return false;
        }

        for (int r = 0; r < table->record_count; r++) {
            fread(&table->records[r].id, sizeof(int), 1, table_file);

            for (int c = 0; c < table->column_count; c++) {
                ColumnDef *col_def = &table->columns[c];
                switch (col_def->type) {
                    case INT:
                        fread(&table->records[r].values[c].int_value, sizeof(int), 1, table_file);
                        break;
                    case FLOAT:
                        fread(&table->records[r].values[c].float_value, sizeof(float), 1, table_file);
                        break;
                    case STRING:
                        fread(table->records[r].values[c].string_value, sizeof(char), MAX_STRING_LENGTH, table_file);
                        break;
                    case BOOL:
                        fread(&table->records[r].values[c].bool_value, sizeof(bool), 1, table_file);
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
// 数据库实用工具函数
// ================================================

void print_record(Table *table, Record *record) {
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

// 查询解析函数 (简化版)
int parse_query(const char *input, Query *query) {
    // 示例解析: "SELECT * FROM users WHERE age>25"
    // 简单实现 - 实际应用中需要完整的SQL解析器
    char table_name[64];
    char column_name[64];
    char op_str[16];
    char value_str[128];

    // 简单分词
    if (sscanf(input, "SELECT * FROM %63s WHERE %63s%15s%127[^\n]",
              table_name, column_name, op_str, value_str) != 4) {
        return -1;
    }

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

    // 解析值
    char *trimmed_value = value_str;
    while (*trimmed_value == ' ') trimmed_value++;

    if (strcmp(trimmed_value, "true") == 0) {
        query->condition.value.bool_value = true;
    } else if (strcmp(trimmed_value, "false") == 0) {
        query->condition.value.bool_value = false;
    } else if (strchr(trimmed_value, '.') != NULL) {
        query->condition.value.float_value = atof(trimmed_value);
    } else {
        query->condition.value.int_value = atoi(trimmed_value);
        strncpy(query->condition.value.string_value, trimmed_value, MAX_STRING_LENGTH);
    }

    return 0;
}

// ================================================
// 演示用例和测试函数
// ================================================

void demo_setup_database(Database *db) {
    // 创建users表
    ColumnDef users_columns[4] = {
        {"id", INT, true, true},
        {"name", STRING, true, false},
        {"age", INT, false, false},
        {"active", BOOL, false, false}
    };

    create_table(db, "users", users_columns, 4);

    // 添加用户记录
    Record user1 = {0};
    user1.values[0].int_value = 1;
    strcpy(user1.values[1].string_value, "John Doe");
    user1.values[2].int_value = 32;
    user1.values[3].bool_value = true;

    Record user2 = {0};
    user2.values[0].int_value = 2;
    strcpy(user2.values[1].string_value, "Jane Smith");
    user2.values[2].int_value = 28;
    user2.values[3].bool_value = true;

    Record user3 = {0};
    user3.values[0].int_value = 3;
    strcpy(user3.values[1].string_value, "Bob Johnson");
    user3.values[2].int_value = 45;
    user3.values[3].bool_value = false;

    insert_record(db, "users", &user1);
    insert_record(db, "users", &user2);
    insert_record(db, "users", &user3);

    // 创建products表
    ColumnDef products_columns[3] = {
        {"id", INT, true, true},
        {"name", STRING, true, false},
        {"price", FLOAT, false, false}
    };

    create_table(db, "products", products_columns, 3);

    // 添加产品记录
    Record product1 = {0};
    product1.values[0].int_value = 101;
    strcpy(product1.values[1].string_value, "Laptop");
    product1.values[2].float_value = 1299.99;

    Record product2 = {0};
    product2.values[0].int_value = 102;
    strcpy(product2.values[1].string_value, "Smartphone");
    product2.values[2].float_value = 799.99;

    insert_record(db, "products", &product1);
    insert_record(db, "products", &product2);
}

void run_demo_queries(Database *db) {
    printf("\n=== Running demo queries ===\n");

    // 查询所有活跃用户
    printf("\nQuery: All active users\n");
    Query active_users_query;
    strcpy(active_users_query.table_name, "users");
    strcpy(active_users_query.condition.column_name, "active");
    active_users_query.condition.op = COMPARISON_EQUAL;
    active_users_query.condition.value.bool_value = true;

    Record results[10];
    int count = query_records(db, &active_users_query, results, 10);

    Table *users_table = find_table(db, "users");
    for (int i = 0; i < count; i++) {
        print_record(users_table, &results[i]);
    }

    // 查询30岁以上用户
    printf("\nQuery: Users older than 30\n");
    Query age_query;
    strcpy(age_query.table_name, "users");
    strcpy(age_query.condition.column_name, "age");
    age_query.condition.op = COMPARISON_GREATER;
    age_query.condition.value.int_value = 30;

    count = query_records(db, &age_query, results, 10);
    for (int i = 0; i < count; i++) {
        print_record(users_table, &results[i]);
    }

    // 更新价格超过1000的产品
    printf("\nUpdating expensive products\n");
    Query expensive_query;
    strcpy(expensive_query.table_name, "products");
    strcpy(expensive_query.condition.column_name, "price");
    expensive_query.condition.op = COMPARISON_GREATER;
    expensive_query.condition.value.float_value = 1000.0;

    DataValue new_price;
    new_price.float_value = 1199.99;

    int updated = update_records(db, "products", &expensive_query, "price", &new_price);
    printf("Updated %d records\n", updated);

    // 验证更新
    Query all_products_query;
    strcpy(all_products_query.table_name, "products");
    strcpy(all_products_query.condition.column_name, "id");
    all_products_query.condition.op = COMPARISON_GREATER;
    all_products_query.condition.value.int_value = 0;

    Table *products_table = find_table(db, "products");
    count = query_records(db, &all_products_query, results, 10);
    for (int i = 0; i < count; i++) {
        print_record(products_table, &results[i]);
    }

    // 删除测试
    printf("\nDeleting inactive users\n");
    Query inactive_query;
    strcpy(inactive_query.table_name, "users");
    strcpy(inactive_query.condition.column_name, "active");
    inactive_query.condition.op = COMPARISON_EQUAL;
    inactive_query.condition.value.bool_value = false;

    int deleted = delete_records(db, "users", &inactive_query);
    printf("Deleted %d records\n", deleted);

    // 验证删除
    Query all_users_query = {{0}};
    strcpy(all_users_query.table_name, "users");
    strcpy(all_users_query.condition.column_name, "id");
    all_users_query.condition.op = COMPARISON_GREATER;
    all_users_query.condition.value.int_value = 0;

    count = query_records(db, &all_users_query, results, 10);
    for (int i = 0; i < count; i++) {
        print_record(users_table, &results[i]);
    }

    // 文件I/O测试
    printf("\nSaving database to files...\n");
    save_database(db);

    Database loaded_db;
    init_database(&loaded_db);
    printf("Loading database from files...\n");
    load_database(&loaded_db);

    printf("\nLoaded database contents:\n");
    Table *loaded_table = find_table(&loaded_db, "users");
    if (loaded_table) {
        print_table_structure(loaded_table);
        for (int i = 0; i < loaded_table->record_count; i++) {
            print_record(loaded_table, &loaded_table->records[i]);
        }
    }
}

// ================================================
// 性能测试函数
// ================================================

void performance_test(Database *db) {
    printf("\n=== Starting performance test ===\n");

    // 准备一个包含大量数据的表
    ColumnDef test_columns[5] = {
        {"id", INT, true, true},
        {"value1", INT, false, false},
        {"value2", FLOAT, false, false},
        {"text", STRING, true, false},
        {"flag", BOOL, false, false}
    };

    if (!find_table(db, "perf_test")) {
        create_table(db, "perf_test", test_columns, 5);
    }

    Table *test_table = find_table(db, "perf_test");

    // 填充1000条记录
    if (test_table->record_count == 0) {
        printf("Generating test data...\n");
        srand(time(NULL));
        for (int i = 0; i < 500; i++) {
            Record r = {0};
            r.values[0].int_value = test_table->record_count;
            r.values[1].int_value = rand() % 1000;
            r.values[2].float_value = (rand() % 10000) / 100.0;

            char text[50];
            sprintf(text, "Record_%d", r.values[0].int_value);
            strncpy(r.values[3].string_value, text, MAX_STRING_LENGTH);

            r.values[4].bool_value = (rand() % 2) == 0;

            insert_record(db, "perf_test", &r);
        }
    }

    // 测试查询性能
    printf("Running query performance test...\n");

    Query query;
    strcpy(query.table_name, "perf_test");
    strcpy(query.condition.column_name, "value1");
    query.condition.op = COMPARISON_GREATER;
    query.condition.value.int_value = 500;

    Record results[1000];

    clock_t start = clock();
    int found = query_records(db, &query, results, 1000);
    clock_t end = clock();

    double elapsed = (double)(end - start) / CLOCKS_PER_SEC * 1000.0;
    printf("Found %d records in %.2f ms\n", found, elapsed);
}

// ================================================
// 主函数 - 数据库测试入口
// ================================================

int main() {
    // 确保数据库目录存在
    system("mkdir -p " DB_DIRECTORY);

    Database db;
    init_database(&db);

    printf("Simple File-based Database System\n");
    printf("================================\n\n");

    // 设置演示数据
    demo_setup_database(&db);

    // 打印表结构
    Table *users = find_table(&db, "users");
    if (users) print_table_structure(users);

    Table *products = find_table(&db, "products");
    if (products) print_table_structure(products);

    // 运行演示查询
    run_demo_queries(&db);

    // 运行性能测试
    performance_test(&db);

    // 保存最终数据库状态
    save_database(&db);

    printf("\nDatabase saved successfully\n");
    return 0;
}