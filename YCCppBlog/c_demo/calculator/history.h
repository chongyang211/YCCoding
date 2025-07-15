//
// Created by 杨充 on 2025/7/15.
//

#ifndef HISTORY_H
#define HISTORY_H

#define MAX_HISTORY_ENTRIES 100

// 历史记录条目
typedef struct {
    char expression[128];
    double result;
} HistoryEntry;

// 历史记录
typedef struct {
    HistoryEntry entries[MAX_HISTORY_ENTRIES];
    int count;
} History;

// 初始化历史记录
void history_init(History* history);

// 添加历史记录
void history_add_entry(History* history, const char* expression, double result);

// 获取历史记录
void history_get_entries(History* history, HistoryEntry** entries, int* count);

// 清除历史记录
void history_clear(History* history);

#endif // HISTORY_H
