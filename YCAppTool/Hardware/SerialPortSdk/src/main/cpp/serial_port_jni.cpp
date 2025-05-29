//
// Created by 杨充 on 2024/4/10.
//

#include "serial_port_log.h"
#include "serial_port_c.h"
#include <jni.h>

SerialPort serialPort;

extern "C"
JNIEXPORT jint JNICALL
Java_com_yc_serialport_SerialPortJNI_openPort(
        JNIEnv *env, jclass jclazz, jstring path,
        jint baudRate, jint dataBits, jint stopBits, jchar parity) {
    try {
        SerialPortConfig config = SerialPortConfig();;
        config.baudrate = baudRate;
        config.databits = dataBits;
        config.stopbits = stopBits;
        config.parity = parity;
        serialPort = SerialPort(env->GetStringUTFChars(path, 0));
        //打开串口通信
        int openStatus = serialPort.openSerialPort(config);
        if (openStatus) {
            LOGD("开始打开串口通信成功");
        } else {
            LOGD("开始打开串口通信失败");
        }
        return openStatus;
    } catch (char *exception) {
        LOGE("Open device is error! Message:%s", exception);
        return FALSE;
    }
}


extern "C"
JNIEXPORT jint JNICALL
Java_com_yc_serialport_SerialPortJNI_setMode(JNIEnv *env, jclass clazz, jint mode) {
    //jint是Java Native Interface (JNI) 中表示Java中的int类型的数据类型。
    // 将 jint 转换为 int
    int intValue = static_cast<int>(mode);
    LOGD("设置串口通信mode类型 %i" , intValue);
    return serialPort.setMode(mode);
}


extern "C"
JNIEXPORT jbyteArray JNICALL
Java_com_yc_serialport_SerialPortJNI_readPort(JNIEnv *env, jclass clazz, jint max_size) {
    BYTE buf[max_size];
    int len;
    len = serialPort.readData(buf, max_size);
    //LOGD("开始读数据 %i", len);
    if (len < 1) return NULL;
    jbyteArray byteArray;
    jbyte *bytes = reinterpret_cast<jbyte *>(buf);
    byteArray = env->NewByteArray(len);
    env->SetByteArrayRegion(byteArray, 0, len, bytes);
    return byteArray;
}


extern "C"
JNIEXPORT void JNICALL
Java_com_yc_serialport_SerialPortJNI_writePort(JNIEnv *env, jclass clazz, jbyteArray data) {
    jbyte *array = env->GetByteArrayElements(data, 0);
    BYTE *bytes = reinterpret_cast<BYTE *>(array);
    jsize arrayLength = env->GetArrayLength(data);
    //LOGD("开始写数据 %i", (int) arrayLength);
    serialPort.writeData(bytes, (int) arrayLength);
}


extern "C"
JNIEXPORT void JNICALL
Java_com_yc_serialport_SerialPortJNI_closePort(JNIEnv *env, jclass clazz) {
    serialPort.closePort();
    serialPort = NULL;
    LOGD("关闭串口通信");
}