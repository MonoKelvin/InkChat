#include "MessageList.h"

#include <AppSettings.h>
#include <InkChatApi.h>
#include <MessageDatabase.h>
#include <MessageItem.h>
#include <Utility.h>

#include <QDir>
#include <QMutex>
#include <QSqlError>

MessageList::MessageList(QObject* parent)
    : QAbstractListModel(parent)
{
    mMutex = new QMutex(QMutex::Recursive);

    const auto error = MessageDatabase::Instance()->initDatabase();
    if (error.type() != QSqlError::NoError) {
        emit failed(tr("消息数据库打开失败，原因：") + error.text());
    }
}

MessageList::~MessageList()
{
    clearMessage();

    SAFE_DELETE(mMutex);

    qDebug() << "MessageList Destroyed";
}

void MessageList::clearMessage()
{
    mMutex->lock();
    beginResetModel();
    qDeleteAll(mMessages.begin(), mMessages.end());
    mMessages.clear();
    endResetModel();
    mMutex->unlock();
}

void MessageList::removeMessage(int index)
{
    if (index >= 0 && index < mMessages.size()) {
        mMutex->lock();
        beginRemoveRows(QModelIndex(), index, index);
        const auto object = mMessages.at(index);
        mMessages.removeAt(index);
        object->deleteLater();
        endRemoveRows();
        mMutex->unlock();
    }
}

bool MessageList::insertMessage(int index, MessageItem* message)
{
    if (index >= 0 && index <= mMessages.size()) {
        mMutex->lock();
        beginInsertRows(QModelIndex(), index, index);
        mMessages.insert(index, message);
        endInsertRows();
        mMutex->unlock();
        return true;
    }

    return false;
}

MessageItem* MessageList::getMessage(int index) const
{
    if (index >= 0 && index < mMessages.size()) {
        mMutex->lock();
        const auto msg = mMessages.at(index);
        mMutex->unlock();
        return msg;
    }
    return nullptr;
}

int MessageList::getRow(MessageItem* message) const
{
    mMutex->lock();
    int row = mMessages.indexOf(message);
    mMutex->unlock();

    return row;
}

int MessageList::rowCount(const QModelIndex& parent) const
{
    Q_UNUSED(parent)
    return mMessages.size();
}

QVariant MessageList::data(const QModelIndex& index, int role) const
{
    Q_UNUSED(role)

    if (index.row() >= 0 && index.row() < mMessages.size()) {
        if (role & IChatObject::AllUser) {
            return QVariant::fromValue(mMessages.at(index.row()));
        }
    }

    return QVariant();
}

bool MessageList::setData(const QModelIndex& index, const QVariant& value, int role)
{
    if (index.row() >= 0 && index.row() < mMessages.size()) {
        if (role & IChatObject::AllUser) {
            mMutex->lock();
            mMessages.replace(index.row(), value.value<MessageItem*>());
            mMutex->unlock();
            return true;
        }
    }
    return false;
}

QHash<int, QByteArray> MessageList::roleNames() const
{
    QHash<int, QByteArray> names;
    names[IChatObject::Me] = QByteArrayLiteral("me");
    names[IChatObject::Friend] = QByteArrayLiteral("friend");
    names[IChatObject::Stranger] = QByteArrayLiteral("stranger");
    names[IChatObject::LAN] = QByteArrayLiteral("lan");
    names[IChatObject::Group] = QByteArrayLiteral("group");
    names[IChatObject::AllUser] = QByteArrayLiteral("msgObject");
    return names;
}

bool MessageList::ariseMessage(MessageItem* message)
{
    /**
     * 如果消息列表中有目标消息项
     *  a.如果置顶：直接更新其相关数据
     *  b.如果未置顶：找到置顶的所有消息，把message移动到它下面，并更新其相关数据
     */
    const int sourceIndex = getRow(message);
    if (sourceIndex == -1) {
        return false;
    }

    int targetIndex = 0;
    if (!message->mChatObject->getIsTop()) {
        for (auto iter = mMessages.cbegin(); iter != mMessages.cend(); ++iter) {
            if (!(*iter)->mChatObject->getIsTop()) {
                break;
            }
            targetIndex++;
        }
        // for (; targetIndex < mMessages.size(); targetIndex++) {
        //     if (!mMessages.at(targetIndex)->mChatObject->getIsTop()) {
        //         break;
        //     }
        // }
        if (targetIndex == mMessages.size()) {
            return false;
        }
    }

    mMutex->lock();
    beginMoveRows(QModelIndex(), sourceIndex, sourceIndex, QModelIndex(), targetIndex);
    mMessages.move(sourceIndex, targetIndex);
    endMoveRows();
    mMutex->unlock();

    return true;
}

void MessageList::adjustMessageOrder()
{
    for (int i = 0; i < mMessages.size(); i++) {
        if (mMessages.at(i)->mChatObject->getIsTop()) {
            mMutex->lock();
            beginMoveRows(QModelIndex(), i, i, QModelIndex(), 0);
            mMessages.move(i, 0);
            endMoveRows();
            mMutex->unlock();
        }
    }
}

bool MessageList::refresh()
{
    return true;
}

void MessageList::load(void)
{
    isFileExists(AppSettings::MessageCacheFile(), true);

    if (!MessageDatabase::Instance()->loadMessageItems(this)) {
        emit failed(tr("消息加载失败，请重新刷新"));
    }
}
