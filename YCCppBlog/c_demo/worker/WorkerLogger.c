//
// Created by 杨充 on 2025/7/14.
//
#include "WorkerLogger.h"

/*-------------------------------------------
 *  日志记录系统
 *------------------------------------------*/

void log_event(const char* message) {
    FILE* log_file = fopen(LOG_FILE, "a");
    if (!log_file) return;

    time_t now = time(NULL);
    char* time_str = ctime(&now);
    time_str[strlen(time_str)-1] = '\0'; // Remove newline

    fprintf(log_file, "[%s] %s\n", time_str, message);
    fclose(log_file);
}
