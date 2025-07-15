//
// Created by 杨充 on 2025/7/15.
//

#include "constants.h"

// 常数定义
static MathConstant constants[] = {
    {"pi", M_PI},
    {"e", M_E},
    {"inf", INFINITY},
    {"nan", NAN},
    {NULL, 0.0} // 哨兵
};

// 初始化常量表
void constants_init(void) {
    // 不需要初始化
}

// 获取数学常量
const MathConstant* constants_get(const char* name) {
    for (const MathConstant* c = constants; c->name != NULL; c++) {
        if (strncmp(c->name, name, strlen(c->name)) == 0) {
            return c;
        }
    }
    return NULL;
}
