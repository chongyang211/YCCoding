#include "AuthManager.h"

AuthManager::AuthManager(QObject *parent)
    : QObject(parent){
    // 初始化操作（实际项目中可加载配置等）
}

bool AuthManager::login(const QString &username, const QString &password){
    // 生产环境应替换为以下实现：
    // 1. 数据库查询
    // 2. 加密验证
    // 3. 网络API调用
    // 当前保持示例代码的模拟验证
    return username == "admin" && password == "123456";
}
