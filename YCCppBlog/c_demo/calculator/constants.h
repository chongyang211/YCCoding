//
// Created by 杨充 on 2025/7/15.
//

#ifndef CONSTANTS_H
#define CONSTANTS_H

typedef struct {
    const char* name;
    double value;
} MathConstant;

// 获取数学常量
const MathConstant* constants_get(const char* name);

// 初始化常量表
void constants_init(void);

#endif // CONSTANTS_H
