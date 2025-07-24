#include "Controller.h"
#include <QDebug>

Controller::Controller(QObject *parent) : QObject(parent){

}

void Controller::handleLogin(const QString &username, const QString &password){
    // 验证凭证
    bool success = validateCredentials(username, password);
    // 发出登录结果信号
    QString message = success ? "登录成功" : "用户名或密码错误";
    emit loginResult(success, message);
    // 可选：记录登录尝试
    qDebug() << "登录尝试:" << username << (success ? "成功" : "失败");
}

bool Controller::validateCredentials(const QString &username, const QString &password){
    // 当前保持示例代码的简单验证
    return username == "admin" && password == "123456";
}
