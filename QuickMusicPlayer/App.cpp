#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QIcon>
#include "AuthManager.h"
#include "DataModel.h"
#include "SensorData.h"
#include "SensorMonitor.h"
#include "Controller.h"


int main(int argc, char *argv[]){

    qputenv("QT_IM_MODULE", QByteArray("qtvirtualkeyboard"));

    QGuiApplication app(argc, argv);

    //设置ICON
    app.setWindowIcon(QIcon(":/images/music.png"));

    // 必须在加载QML前注册！
    QQmlApplicationEngine engine;
    // 创建对象
    AuthManager authManager;
    // 设置上下文属性
    engine.rootContext()->setContextProperty("authManager", &authManager);
    DataModel dataModel;
    engine.rootContext()->setContextProperty("dataModel", &dataModel);
    SensorData sensorData;
    engine.rootContext()->setContextProperty("sensorData", &sensorData);
    SensorMonitor sensorMonitor;
    engine.rootContext()->setContextProperty("sensorMonitor", &sensorMonitor);
    Controller controller;
     engine.rootContext()->setContextProperty("controller", &controller);

    QObject::connect(
        &engine,
        &QQmlApplicationEngine::objectCreationFailed,
        &app,
        []() { QCoreApplication::exit(-1); },
        Qt::QueuedConnection);

    engine.loadFromModule("QuickMusicPlayer", "App");

    return app.exec();
}
