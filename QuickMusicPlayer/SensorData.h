#ifndef SENSORDATA_H
#define SENSORDATA_H

#include <QObject>
#include <QTimer>

class SensorData : public QObject{
    Q_OBJECT
    // QML可访问的属性
    Q_PROPERTY(int value READ value NOTIFY valueChanged)

public:
    explicit SensorData(QObject *parent = nullptr);
    // 属性访问器
    int value() const;

signals:
    // 属性变化通知信号
    void valueChanged();

private slots:
    // 定时更新数据的槽函数
    void updateValue();

private:
    int m_value;        // 存储的数值
    QTimer *m_timer;    // 定时器指针
};

#endif // SENSORDATA_H
