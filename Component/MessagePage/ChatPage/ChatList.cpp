#include "ChatList.h"

#include <AppSettings.h>
#include <MessageManager.h>
#include <MyFriend.h>
#include <User.h>

QHash<int, QByteArray> ChatList::mRegistryChatClasses;

ChatList::ChatList(QObject* parent)
    : QAbstractListModel(parent)
{
    connect(MessageManager::Instance().data(), &MessageManager::received, this, &ChatList::onReceived);
}

ChatList::~ChatList()
{
    if (mChatObject) {
        mChatObject.clear();
    }

    clear();

    qDebug() << "ChatView Destroyed: " << this;
}

IChatItem* ChatList::BuildChatItem(int chatType, bool isMe, unsigned int uid)
{
    const auto className = mRegistryChatClasses.value(chatType);
    const auto id = QMetaType::type(className);

    // 通过反射机制创建实例
    IChatItem* chat = nullptr;
    if (id != QMetaType::UnknownType) {
        chat = static_cast<IChatItem*>(QMetaType::create(id));
        if (!chat) {
            return nullptr;
        }

        if (isMe) {
            chat->mChatObject = User::Instance();
        } else {
            chat->mChatObject = User::Instance()->getFriendById(uid);
        }
    }

    return chat;
}

IChatItem* ChatList::BuildChatItem(int chatType, const QDateTime& time, const QVariant& data)
{
    const auto item = BuildChatItem(chatType, true, User::Instance()->getID());
    if (nullptr == item) {
        return nullptr;
    }

    item->mTime = time;
    item->praseData(data);

    return item;
}

IChatItem* ChatList::BuildChatItem(int chatType, unsigned int uid, const QDateTime& time, const QVariant& data)
{
    const auto item = BuildChatItem(chatType, false, uid);
    if (nullptr == item) {
        return nullptr;
    }

    item->mTime = time;
    item->praseData(data);

    return item;
}

IChatItem* ChatList::getChatItem(int index) const
{
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

bool ChatList::insertChat(int row, IChatItem* chat)
{
    if (row >= 0 && row <= mChats.size()) {
        beginInsertRows(QModelIndex(), row, row);
        mChats.insert(row, chat);
        endInsertRows();
        return true;
    }

    return false;
}

int ChatList::rowCount(const QModelIndex& parent) const
{
    Q_UNUSED(parent)

    return mChats.size();
}

void ChatList::sendChat(int chatType, const QVariant& data)
{
    MessageManager::Instance()->sendMessage(this, chatType, data);
}

void ChatList::initLoad(IChatObject* chatObj)
{
    if (mChatObject) {
        return;
    } else {
        mChatObject = chatObj;
    }

    fetchMore();
}

void ChatList::clearChatRecord()
{
    clear();

    // TODO: 删除数据库中对应的记录
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

    isFileExists(AppSettings::MessageCacheFile(mChatObject->getRoleType()), true);
    MessageManager::Instance()->loadChatRecords(this);
}

void ChatList::onReceived(IChatItem* item, const QVariantMap& sourceData)
{
    bool receive = false;
    int roleType = sourceData.value(QStringLiteral("roleType")).toInt();

    if (roleType & IChatObject::MultiPerson) {
        const QString md5 = sourceData.value(QStringLiteral("md5")).toString();

        // 如果数据中的MD5和该聊天对象匹配就接收数据
        if (md5 == mChatObject->getMD5()) {
            receive = true;
        }
    } else if (roleType & IChatObject::SinglePerson) {

        // 如果数据中的单用户id和该聊天对象的用户id匹配就接收数据
        if (item->mChatObject->getID() == mChatObject->getID()) {
            receive = true;
        }
    }

    if (!receive) {
        return;
    }

    appendChat(item);
    MessageManager::Instance()->saveAChatRecord(this, item);
}

QVariant ChatList::data(const QModelIndex& index, int role) const
{
    Q_UNUSED(role)

    if (index.row() >= 0 && index.row() < mChats.size()) {
        if (role >= 0) {
            return QVariant::fromValue(mChats.at(index.row()).data());
        }
    }

    return QVariant();
}

bool ChatList::setData(const QModelIndex& index, const QVariant& value, int role)
{
    if (index.row() >= 0 && index.row() < mChats.size()) {
        if (role >= 0) {
            mChats.replace(index.row(), value.value<IChatItem*>());
            return true;
        }
    }
    return false;
}
