#include "ContactModel.h"
#include <QColor>
#include <QPainter>
#include <QRandomGenerator>
#include <QDebug>

ContactModel::ContactModel(QObject *parent) : QAbstractListModel(parent) {
    generateDefaultAvatars();

    // 添加一些示例联系人
    addContact("张明", "13812345678", "zhangming@example.com", "北京市朝阳区", "大学同学");
    addContact("李思", "13987654321", "lisi@example.com", "上海市浦东新区", "同事", true);
    addContact("王武", "13755556666", "wangwu@example.com", "广州市天河区", "项目经理");
}

int ContactModel::rowCount(const QModelIndex &parent) const {
    if (parent.isValid()) return 0;
    return m_contacts.size();
}

QVariant ContactModel::data(const QModelIndex &index, int role) const {
    if (!index.isValid() || index.row() < 0 || index.row() >= m_contacts.size())
        return QVariant();

    const Contact &contact = m_contacts[index.row()];

    switch(role) {
    case NameRole: return contact.name;
    case PhoneRole: return contact.phone;
    case EmailRole: return contact.email;
    case AddressRole: return contact.address;
    case NotesRole: return contact.notes;
    case AvatarRole: return contact.avatar;
    case FavoriteRole: return contact.favorite;
    default: return QVariant();
    }
}

bool ContactModel::setData(const QModelIndex &index, const QVariant &value, int role) {
    if (!index.isValid() || role != FavoriteRole)
        return false;

    bool favorite = value.toBool();
    Contact &contact = m_contacts[index.row()];
    if (contact.favorite != favorite) {
        contact.favorite = favorite;
        emit dataChanged(index, index, {FavoriteRole});
        emit showMessage(QString("联系人 %1 的收藏状态已更新").arg(contact.name));
        return true;
    }
    return false;
}

QHash<int, QByteArray> ContactModel::roleNames() const {
    QHash<int, QByteArray> roles;
    roles[NameRole] = "name";
    roles[PhoneRole] = "phone";
    roles[EmailRole] = "email";
    roles[AddressRole] = "address";
    roles[NotesRole] = "notes";
    roles[AvatarRole] = "avatar";
    roles[FavoriteRole] = "favorite";
    return roles;
}

void ContactModel::addContact(const QString &name, const QString &phone, const QString &email,
                              const QString &address, const QString &notes, bool favorite) {
    beginInsertRows(QModelIndex(), rowCount(), rowCount());
    Contact contact;
    contact.name = name;
    contact.phone = phone;
    contact.email = email;
    contact.address = address;
    contact.notes = notes;
    contact.favorite = favorite;

    // 分配头像
    contact.avatar = m_defaultAvatars[m_nextAvatarIndex];
    m_nextAvatarIndex = (m_nextAvatarIndex + 1) % m_defaultAvatars.size();

    m_contacts.append(contact);
    endInsertRows();

    emit contactCountChanged(rowCount());
    emit showMessage(QString("添加联系人: %1").arg(name));
}

void ContactModel::updateContact(int index, const QString &name, const QString &phone, const QString &email,
                                 const QString &address, const QString &notes, bool favorite) {
    if (index < 0 || index >= m_contacts.size())
        return;

    Contact &contact = m_contacts[index];
    bool changed = false;

    if (contact.name != name) {
        contact.name = name;
        changed = true;
    }
    if (contact.phone != phone) {
        contact.phone = phone;
        changed = true;
    }
    if (contact.email != email) {
        contact.email = email;
        changed = true;
    }
    if (contact.address != address) {
        contact.address = address;
        changed = true;
    }
    if (contact.notes != notes) {
        contact.notes = notes;
        changed = true;
    }
    if (contact.favorite != favorite) {
        contact.favorite = favorite;
        changed = true;
    }

    if (changed) {
        QModelIndex modelIndex = createIndex(index, 0);
        emit dataChanged(modelIndex, modelIndex);
        emit showMessage(QString("联系人 %1 已更新").arg(name));
    }
}

void ContactModel::removeContact(int index) {
    if (index < 0 || index >= m_contacts.size())
        return;

    QString name = m_contacts[index].name;
    beginRemoveRows(QModelIndex(), index, index);
    m_contacts.remove(index);
    endRemoveRows();

    emit contactCountChanged(rowCount());
    emit showMessage(QString("已删除联系人: %1").arg(name));
}

void ContactModel::toggleFavorite(int index) {
    if (index < 0 || index >= m_contacts.size())
        return;

    QModelIndex modelIndex = createIndex(index, 0);
    setData(modelIndex, !m_contacts[index].favorite, FavoriteRole);
}

QVariantMap ContactModel::getContactDetails(int index) const {
    QVariantMap details;
    if (index < 0 || index >= m_contacts.size())
        return details;

    const Contact &contact = m_contacts[index];
    details["name"] = contact.name;
    details["phone"] = contact.phone;
    details["email"] = contact.email;
    details["address"] = contact.address;
    details["notes"] = contact.notes;
    details["avatar"] = contact.avatar;
    details["favorite"] = contact.favorite;
    details["index"] = index;

    return details;
}

void ContactModel::generateDefaultAvatars() {
    QStringList colors = {
        "#FF5252", "#FF4081", "#E040FB", "#7C4DFF",
        "#536DFE", "#448AFF", "#40C4FF", "#18FFFF",
        "#64FFDA", "#69F0AE", "#B2FF59", "#EEFF41"
    };

    for (const QString &color : colors) {
        QPixmap avatar(100, 100);
        avatar.fill(Qt::transparent);

        QPainter painter(&avatar);
        painter.setRenderHint(QPainter::Antialiasing);

        QLinearGradient gradient(0, 0, 100, 100);
        gradient.setColorAt(0, QColor(color));
        gradient.setColorAt(1, QColor(color).lighter(130));

        painter.setBrush(gradient);
        painter.setPen(Qt::NoPen);
        painter.drawEllipse(0, 0, 100, 100);

        // 添加首字母（暂时用空格）
        painter.setPen(Qt::white);
        painter.setFont(QFont("Arial", 40, QFont::Bold));

        m_defaultAvatars.append(avatar);
    }
}
