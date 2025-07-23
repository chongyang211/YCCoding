#ifndef DATAMODEL_H
#define DATAMODEL_H

#include <QAbstractListModel>
#include <QStringList>
#include <QHash>

class DataModel : public QAbstractListModel {
    Q_OBJECT
public:
    // 角色枚举 - 使用枚举值定义数据角色
    enum ItemRoles {
        NameRole = Qt::UserRole + 1  // 第一个自定义角色从 Qt::UserRole + 1 开始
    };

    explicit DataModel(QObject *parent = nullptr);

    // 模型接口重载
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    // 必须重写的角色名称映射
    QHash<int, QByteArray> roleNames() const override;

    Q_INVOKABLE void addItem(const QString &name);
    Q_INVOKABLE void removeItem(int index);

private:
    QStringList m_data;  // 实际存储数据的容器
};

#endif // DATAMODEL_H
