#include "DataModel.h"

// 构造函数：初始化数据
DataModel::DataModel(QObject *parent) : QAbstractListModel(parent){
    // 初始化模拟数据
    m_data << "Apple" << "Banana" << "Orange";
}

// 返回模型行数
int DataModel::rowCount(const QModelIndex &parent) const{
    Q_UNUSED(parent); // 对于列表模型不需要parent参数
    return m_data.size();
}

// 获取特定索引的数据
QVariant DataModel::data(const QModelIndex &index, int role) const{
    // 检查索引有效性
    if (!index.isValid() || index.row() < 0 || index.row() >= m_data.size())
        return QVariant();

    // 返回请求的角色数据
    if (role == NameRole || role == Qt::DisplayRole) {
        return m_data.at(index.row());
    }

    return QVariant();
}

// 返回角色名称映射
QHash<int, QByteArray> DataModel::roleNames() const {
    QHash<int, QByteArray> roles;
    roles[NameRole] = "name";  // 映射为 QML 可访问的 "name" 属性

    // 可选：添加标准角色映射提高兼容性
    roles[Qt::DisplayRole] = "display";

    return roles;
}

void DataModel::addItem(const QString &name) {
    beginInsertRows(QModelIndex(), rowCount(), rowCount());
    m_data.append(name);
    endInsertRows();
}

void DataModel::removeItem(int index){
    if(index < 0 || index >= m_data.size()) return;

    beginRemoveRows(QModelIndex(), index, index);
    m_data.removeAt(index);
    endRemoveRows();
}
