#ifndef SENSORMONITOR_H
#define SENSORMONITOR_H

#include <QObject>

class SensorMonitor : public QObject{
    Q_OBJECT
public:
    // 显式构造函数
    explicit SensorMonitor(QObject *parent = nullptr);
    // 触发更新方法
    Q_INVOKABLE void update();
signals:
    // 温度变化信号
    void temperatureChanged(double value);
};

#endif // SENSORMONITOR_H
