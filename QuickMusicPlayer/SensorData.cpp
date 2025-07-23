#include "SensorData.h"

// 初始化父对象
// 初始化值为0
SensorData::SensorData(QObject *parent) : QObject(parent), m_value(0) {
    // 创建定时器并设置父对象保证自动销毁
    m_timer = new QTimer(this);
    // 连接定时器信号到更新槽
    connect(m_timer, &QTimer::timeout, this, &SensorData::updateValue);
    // 启动1秒间隔的定时器
    m_timer->start(1000);
}

int SensorData::value() const{
    return m_value;
}

void SensorData::updateValue(){
    // 增加数值并触发更新
    m_value += 1;
    emit valueChanged ();

    // 实际项目中应替换为真实的传感器读取逻辑
    // 例如: m_value = readSensorValue();
}
