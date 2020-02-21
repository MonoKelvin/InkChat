#include "ChatView.h"

#include <AppSettings.h>
#include <MessageDatabase.h>
#include <Utility.h>

QHash<int, QByteArray> ChatView::mRegistryChatClasses;

ChatView::ChatView(QObject* parent)
    : QAbstractListModel(parent)
{
}

ChatView::~ChatView()
{
    clear();
}

IChatItem* ChatView::buildChatItem(int chatType)
{
    const auto className = mRegistryChatClasses.value(chatType);
    const auto id = QMetaType::type(className);

    // 通过反射机制创建实例
    IChatItem* chat = nullptr;
    if (id != QMetaType::UnknownType) {
        chat = static_cast<IChatItem*>(QMetaType::create(id));
    }

    return chat;
}

void ChatView::load(unsigned int id)
{
    isFileExists(AppSettings::MessageCacheFile(), true);

    if (!MessageDatabase::Instance()->loadChatMessages(this, id)) {
        emit failed(tr("聊天记录加载失败，请重新刷新"));
    }
}

void ChatView::clearChatRecord()
{
    clear();

    // todo: 删除数据库中对应的记录
}

void ChatView::clear()
{
    beginResetModel();
    qDeleteAll(mChats.begin(), mChats.end());
    mChats.clear();
    endResetModel();
}

IChatItem* ChatView::getChatItem(int index) const
{
    if (index >= 0 && index < mChats.size()) {
        const auto msg = mChats.at(index);
        return msg;
    }
    return nullptr;
}

void ChatView::removeChatItem(int row, bool cascade)
{
    if (row >= 0 && row < mChats.size()) {
        beginRemoveRows(QModelIndex(), row, row);
        const auto object = mChats.at(row);
        mChats.removeAt(row);
        object->deleteLater();
        endRemoveRows();

        if (cascade) {
            // todo: 删除数据库的相关数据
        }
    }
}

bool ChatView::insertChat(int row, IChatItem* chat)
{
    if (row >= 0 && row <= mChats.size()) {
        beginInsertRows(QModelIndex(), row, row);
        mChats.insert(row, chat);
        endInsertRows();
        return true;
    }

    return false;
}

int ChatView::rowCount(const QModelIndex& parent) const
{
    Q_UNUSED(parent)

    return mChats.size();
}

bool ChatView::sendChat(IChatItem* chat)
{
    // todo: 更新数据库

    return insertChat(mChats.size(), chat);
}

QVariant ChatView::data(const QModelIndex& index, int role) const
{
    Q_UNUSED(role)

    if (index.row() >= 0 && index.row() < mChats.size()) {
        if (role >= 0) {
            return QVariant::fromValue(mChats.at(index.row()).data());
        }
    }

    return QVariant();
}

bool ChatView::setData(const QModelIndex& index, const QVariant& value, int role)
{
    if (index.row() >= 0 && index.row() < mChats.size()) {
        if (role >= 0) {
            mChats.replace(index.row(), value.value<IChatItem*>());
            return true;
        }
    }
    return false;
}
