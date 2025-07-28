#ifndef CONTACTMODEL_H
#define CONTACTMODEL_H

#include <QObject>
#include <QAbstractListModel>
#include <QVector>
#include <QPixmap>

// 联系人数据结构
struct Contact {
    QString name;
    QString phone;
    QString email;
    QString address;
    QString notes;
    QPixmap avatar;
    bool favorite;
};

class ContactModel : public QAbstractListModel {
    Q_OBJECT
public:
    enum ContactRoles {
        NameRole = Qt::UserRole + 1,
        PhoneRole,
        EmailRole,
        AddressRole,
        NotesRole,
        AvatarRole,
        FavoriteRole
    };

    explicit ContactModel(QObject *parent = nullptr);

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole) override;
    QHash<int, QByteArray> roleNames() const override;

    // 添加联系人
    Q_INVOKABLE void addContact(const QString &name,
                                const QString &phone,
                                const QString &email,
                                const QString &address = "",
                                const QString &notes = "",
                                bool favorite = false);

    // 更新联系人
    Q_INVOKABLE void updateContact(int index,
                                   const QString &name,
                                   const QString &phone,
                                   const QString &email,
                                   const QString &address,
                                   const QString &notes,
                                   bool favorite);

    // 删除联系人
    Q_INVOKABLE void removeContact(int index);

    // 切换收藏状态
    Q_INVOKABLE void toggleFavorite(int index);

    // 获取联系人详情
    Q_INVOKABLE QVariantMap getContactDetails(int index) const;

signals:
    // 通知QML模型发生变化
    void contactCountChanged(int count);
    // 通用消息信号
    void showMessage(const QString &message);

private:
    void generateDefaultAvatars();
    QVector<Contact> m_contacts;
    QVector<QPixmap> m_defaultAvatars;
    int m_nextAvatarIndex = 0;
};

#endif // CONTACTMODEL_H
