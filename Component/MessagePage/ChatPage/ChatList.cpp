#include "ChatList.h"

#include <AppSettings.h>
#include <ChatItem.h>
#include <MessageDatabase.h>
#include <User.h>

ChatList::ChatList(QObject* parent)
    : QAbstractListModel(parent)
{
}

ChatList::~ChatList()
{
    clear();

    qDebug() << "ChatList Destroyed: " << this;
}

AbstractChatListItem* ChatList::getChatItem(int index, bool onlyChat) const
{
    // TODO: achieve it
    Q_UNUSED(onlyChat)

    if (index >= 0 && index < mChats.size()) {
        return mChats.at(index);
    }
    return nullptr;
}

void ChatList::removeChatItem(int row, bool cascade)
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

void ChatList::insertChat(int row, AbstractChatListItem* item)
{
    beginInsertRows(QModelIndex(), row, row);
    mChats.insert(row, item);
    endInsertRows();

    if (mChats.size() > 100) {
        removeChatItem(0);
    }

    emit chatAdded(row);
}

int ChatList::rowCount(const QModelIndex& parent) const
{
    Q_UNUSED(parent)

    return mChats.size();
}

IChatObject* ChatList::getChatObject() const noexcept
{
    return mChatObject;
}

void ChatList::clear()
{
    beginResetModel();
    qDeleteAll(mChats.begin(), mChats.end());
    mChats.clear();
    endResetModel();
}

void ChatList::fetchMore(const QModelIndex& parent)
{
    Q_UNUSED(parent)

    IsFileExists(AppSettings::MessageDBFile(), true);
    MessageDatabase::Instance()->loadChatItems(this);
}

void ChatList::initLoad(IChatObject* chatObj)
{
    Q_ASSERT(nullptr != chatObj);

    mChatObject = chatObj;

    fetchMore();
}

QVariant ChatList::data(const QModelIndex& index, int role) const
{
    if (!index.isValid())
        return QVariant();

    Q_UNUSED(role)

    return QVariant::fromValue(mChats.at(index.row()).data());
}

bool ChatList::setData(const QModelIndex& index, const QVariant& value, int role)
{
    if (index.row() >= 0 && index.row() < mChats.size()) {
        if (role >= 0) {
            mChats.replace(index.row(), value.value<ChatItem*>());
            return true;
        }
    }
    return false;
}
