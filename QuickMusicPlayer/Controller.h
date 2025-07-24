#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <QObject>
#include <QString>

class Controller : public QObject {
    Q_OBJECT
public:
    // 显式构造函数
    explicit Controller(QObject *parent = nullptr);

public slots:
    // 登录处理槽函数声明
    void handleLogin(const QString &username, const QString &password);

signals:
    // 可选：添加登录结果信号
    void loginResult(bool success, const QString &message);

private:
    // 私有成员变量和方法
    bool validateCredentials(const QString &username, const QString &password);
};

#endif // CONTROLLER_H
