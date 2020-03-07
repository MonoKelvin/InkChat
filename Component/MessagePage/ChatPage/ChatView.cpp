﻿#include "ChatView.h"

#include <AppSettings.h>
#include <ChatManager.h>
#include <MessageDatabase.h>
#include <MyFriend.h>
#include <User.h>
#include <Utility.h>

QHash<int, QByteArray> ChatView::mRegistryChatClasses;

ChatView::ChatView(QObject* parent)
    : QAbstractListModel(parent)
{
}

ChatView::~ChatView()
{
    clear();
    qDebug() << "Chat View Destroy: " << this;
}

IChatItem* ChatView::BuildChatItem(int chatType, bool isMe, unsigned int uid)
{
    const auto className = mRegistryChatClasses.value(chatType);
    const auto id = QMetaType::type(className);

    // 通过反射机制创建实例
    IChatItem* chat = nullptr;
    if (id != QMetaType::UnknownType) {
        chat = static_cast<IChatItem*>(QMetaType::create(id));

        if (isMe) {
            chat->mChatObject = User::Instance();
        } else {
            chat->mChatObject = User::Instance()->getFriendById(uid);
        }
    }

    return chat;
}

IChatItem* ChatView::BuildChatItem(int chatType, const QDateTime& time, const QVariant& data)
{
    const auto item = BuildChatItem(chatType, true, User::Instance()->getID());
    if (nullptr == item) {
        return nullptr;
    }

    item->mTime = time;
    item->praseData(data);

    return item;
}

IChatItem* ChatView::BuildChatItem(int chatType, unsigned int uid, const QDateTime& time, const QVariant& data)
{
    const auto item = BuildChatItem(chatType, false, uid);
    if (nullptr == item) {
        return nullptr;
    }

    item->mTime = time;
    item->praseData(data);

    return item;
}

void ChatView::load(unsigned int uid)
{
    isFileExists(AppSettings::MessageCacheFile(), true);

    if (!MessageDatabase::Instance()->loadChatMessages(this, uid)) {
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

void ChatView::sendChat(unsigned int uid, const QString& msg)
{
    const auto time = QDateTime::currentDateTime();
    IChatItem* item = BuildChatItem(IChatItem::Text, time, msg);
    if (nullptr == item) {
        emit failed(tr("消息项无效"));
        return;
    }

    if (!insertChat(mChats.size(), item)) {
        emit failed(tr("消息发送失败"));
        delete item;
        item = nullptr;
    }

    if (!MessageDatabase::Instance()->saveAChatRecord(uid, item)) {
        emit failed(tr("聊天记录保存到本地失败"));
    }
}
