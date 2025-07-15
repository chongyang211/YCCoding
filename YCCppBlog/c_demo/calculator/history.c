//
// Created by 杨充 on 2025/7/15.
//
#include "history.h"
#include <string.h>
#include <stdlib.h>

// 初始化历史记录
void history_init(History* history) {
    history->count = 0;
}

// 添加历史记录
void history_add_entry(History* history, const char* expression, double result) {
    if (history->count < MAX_HISTORY_ENTRIES) {
        HistoryEntry* entry = &history->entries[history->count];
        strncpy(entry->expression, expression, sizeof(entry->expression) - 1);
        entry->expression[sizeof(entry->expression) - 1] = '\0';
        entry->result = result;
        history->count++;
    } else {
        // 如果历史记录满了，移动所有条目
        for (int i = 0; i < MAX_HISTORY_ENTRIES - 1; i++) {
            history->entries[i] = history->entries[i+1];
        }
        // 添加新条目到最后
        HistoryEntry* entry = &history->entries[MAX_HISTORY_ENTRIES - 1];
        strncpy(entry->expression, expression, sizeof(entry->expression) - 1);
        entry->expression[sizeof(entry->expression) - 1] = '\0';
        entry->result = result;
    }
}

// 获取历史记录
void history_get_entries(History* history, HistoryEntry** entries, int* count) {
    *entries = history->entries;
    *count = history->count;
}

// 清除历史记录
void history_clear(History* history) {
    history->count = 0;
}