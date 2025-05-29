//
// Created by 杨充 on 2024/4/10.
//

#ifndef YCAPPHELPER_SERIAL_PORT_LOG_H
#define YCAPPHELPER_SERIAL_PORT_LOG_H

#include <android/log.h>
// Log标记
#define  LOG_TAG "SerialPortLog"
// 各个优先级的宏定义
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO, LOG_TAG, __VA_ARGS__)
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__)
#define LOGD(...) __android_log_print(ANDROID_LOG_DEBUG, LOG_TAG, __VA_ARGS__)
#define LOGW(...) __android_log_write(ANDROID_LOG_WARN, LOG_TAG, __VA_ARGS__)
#define LOGV(...) __android_log_print(ANDROID_LOG_VERBOSE, LOG_TAG, __VA_ARGS__)

#endif //YCAPPHELPER_SERIAL_PORT_LOG_H
