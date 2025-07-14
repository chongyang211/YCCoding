//
// Created by 杨充 on 2025/7/14.
//
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include <time.h>

#define LOG_FILE "system_log.txt"

typedef struct LogEntry {
    time_t timestamp;
    char message[256];
    struct LogEntry* next;
} LogEntry;

//void log_event(const char* message);