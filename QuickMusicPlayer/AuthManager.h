#ifndef AUTHMANAGER_H
#define AUTHMANAGER_H

#include <QObject>
#include <QString>

class AuthManager : public QObject{
    Q_OBJECT
public:
    // 构造函数
    explicit AuthManager(QObject *parent = nullptr);

    // 登录验证接口
    Q_INVOKABLE bool login(const QString &username, const QString &password);
};

#endif // AUTHMANAGER_H
