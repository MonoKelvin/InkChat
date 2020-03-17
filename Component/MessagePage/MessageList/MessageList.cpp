#include "MessageList.h"

#include <AppSettings.h>
#include <InkChatApi.h>
#include <MessageDatabase.h>
#include <MessageItem.h>

#include <QSqlError>

MessageList::MessageList(QObject* parent)
    : QAbstractListModel(parent)
{
    const auto error = MessageDatabase::Instance()->initDatabase();
    if (error.type() != QSqlError::NoError) {
        emit failed(tr("消息数据库打开失败，原因：") + error.text());
    }
}

MessageList::~MessageList()
{
    clearMessage();

    qDebug() << "MessageList Destroyed";
}

void MessageList::clearMessage()
{
    beginResetModel();
    qDeleteAll(mMessages.begin(), mMessages.end());
    mMessages.clear();
    endResetModel();

    if (mCurrentSelectedItem) {
        mCurrentSelectedItem->deleteLater();
    }
}

void MessageList::removeMessage(int index)
{
    if (index >= 0 && index < mMessages.size()) {
        beginRemoveRows(QModelIndex(), index, index);
        const auto it = mMessages.begin() + index;
        mMessages.erase(it, it + 1);
        endRemoveRows();
    }
}

void MessageList::insertMessage(int index, MessageItem* message)
{
    if (index >= 0 && index <= mMessages.size()) {
        beginInsertRows(QModelIndex(), index, index);
        mMessages.insert(index, message);
        endInsertRows();
    }
}

void MessageList::moveMessage(int from, int to)
{
    if (from < 0
        || from >= mMessages.size()
        || to < 0
        || to >= mMessages.size()) {
        return;
    }

    emit layoutAboutToBeChanged(QList<QPersistentModelIndex>(), VerticalSortHint);
    if (beginMoveRows(QModelIndex(), from, from, QModelIndex(), to)) {
        if (from != to) {
            mMessages.move(from, to);
        }
        endMoveRows();
    }
    emit layoutChanged(QList<QPersistentModelIndex>(), VerticalSortHint);
}

bool MessageList::isChatObjectExists(IChatObject* chatObj)
{
    for (int i = 0; i < mMessages.size(); i++) {
        if (mMessages.at(i)->mChatObject == chatObj) {
            return true;
        }
    }

    return false;
}

void MessageList::setMessageTop(MessageItem* message, bool isTop, bool)
{
    const int sourceIndex = getRow(message);

    if (isTop) {
        moveMessage(sourceIndex, 0);
    } else {
        moveMessage(sourceIndex, mMessages.size() - 1);
    }

    message->mChatObject->setIsTop(isTop);
}

MessageItem* MessageList::getCurrentSelectedItem() const
{
    return mCurrentSelectedItem.data();
}

MessageItem* MessageList::getMessage(int index) const
{
    if (index >= 0 && index < mMessages.size()) {
        return mMessages.at(index);
    }
    return nullptr;
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
            mMessages.replace(index.row(), value.value<MessageItem*>());
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

void MessageList::setCurrentSelectedIndex(int index)
{
    if (index >= 0 && index < mMessages.size()) {
        mCurrentSelectedItem = mMessages.at(index);
    } else {
        mCurrentSelectedItem.clear();
    }
}

int MessageList::getCurrentSelectedIndex()
{
    return mMessages.indexOf(mCurrentSelectedItem);
}

void MessageList::ariseMessage(MessageItem* message)
{
    /**
     * 如果消息列表中有目标消息项
     *  a.如果置顶：直接更新其相关数据
     *  b.如果未置顶：找到置顶的所有消息，把message移动到它下面，并更新其相关数据
     */
    const int sourceIndex = getRow(message);

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
    }

    moveMessage(sourceIndex, targetIndex);
}

void MessageList::fetchMore(const QModelIndex& index)
{
    Q_UNUSED(index)

    MessageDatabase::Instance()->detectLanEnvironment(this);
}

void MessageList::load(void)
{
    MessageDatabase::Instance()->loadMessageItems(this);
    MessageDatabase::Instance()->loadLanMessageItems(this);

    int pos = 0;
    for (int i = 0; i < mMessages.size(); i++) {
        if (mMessages.at(i)->mChatObject->getIsTop()) {
            mMessages.move(i, pos++);
        }
    }
}
