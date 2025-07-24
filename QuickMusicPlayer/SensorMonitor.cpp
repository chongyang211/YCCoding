#include "SensorMonitor.h"

SensorMonitor::SensorMonitor(QObject *parent)
    : QObject(parent) {
    // 构造函数初始化代码
    // 此处可添加传感器初始化或配置
}

Q_INVOKABLE void SensorMonitor::update(){
    // 生产环境应替换为实际传感器读取逻辑
    const double temp = 25.3; // 模拟温度值
    // 发出温度变化信号
    emit temperatureChanged(temp);
}
