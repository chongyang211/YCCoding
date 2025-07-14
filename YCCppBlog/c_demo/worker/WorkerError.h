//
// Created by 杨充 on 2025/7/14.
//

typedef enum {
    ERR_NONE = 0,
    ERR_FILE_OPEN,
    ERR_MEMORY,
    ERR_INVALID_INPUT,
    ERR_ID_EXISTS,
    ERR_ID_NOT_FOUND,
    ERR_LIST_FULL,
    ERR_DATA_CORRUPT
} ErrorCode;

const char* error_messages[] = {
    "操作成功",
    "无法打开文件",
    "内存分配失败",
    "无效输入",
    "职工ID已存在",
    "未找到该职工",
    "职工列表已满",
    "数据文件损坏"
};

