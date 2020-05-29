#include "MessageList.h"

#include <AppSettings.h>
#include <MessageDatabase.h>
#include <MessageItem.h>

#include <QSqlError>

//QPointer<MessageItem> MessageList::mCurrentSelectedItem;

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

    /*if (mCurrentSelectedItem) {
        mCurrentSelectedItem->deleteLater();
    }*/
}

void MessageList::removeMessage(int index)
{
    if (index >= 0 && index < mMessages.size()) {
        beginRemoveRows(QModelIndex(), index, index);
        const auto& iter = mMessages.begin() + index;
        mMessages.erase(iter, iter + 1);
        endRemoveRows();
    }
}

void MessageList::insertMessage(int index, MessageItem* message)
{
    if (-1 == getIndexByChatObject(message->getChatObject())) {
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

int MessageList::getIndexByChatObject(IChatObject* chatObj)
{
    for (int i = 0; i < mMessages.size(); i++) {
        if (mMessages.at(i)->mChatObject.data() == chatObj) {
            return i;
        }
    }

    return -1;
}

void MessageList::setMessageTop(MessageItem* message, bool isTop, bool)
{
    const int sourceIndex = getRow(message);

    if (isTop) {
        moveMessage(sourceIndex, 0);
    } else {
        moveMessage(sourceIndex, mMessages.size() - 1);
    }

    message->setIsTop(isTop);
}

/*MessageItem* MessageList::getCurrentSelectedItem() const
{
    return mCurrentSelectedItem.data();
}

QSharedPointer<IChatObject> MessageList::GetCurrentChatObject()
{
    return mCurrentSelectedItem->mChatObject;
}*/

MessageItem* MessageList::getMessage(int index) const
{
    Q_ASSERT(index >= 0 && index < mMessages.size());
    return mMessages.at(index);
}

QVariant MessageList::data(const QModelIndex& index, int role) const
{
    if (!index.isValid())
        return QVariant();

    Q_UNUSED(role)

    if (role & IChatObject::AllUser) {
        return QVariant::fromValue(mMessages.at(index.row()));
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

/*void MessageList::setCurrentSelectedIndex(int index)
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
}*/

int MessageList::promoteMessage(MessageItem* message)
{
    /**
     * 如果消息列表中有目标消息项
     *  a.如果置顶：直接更新其相关数据
     *  b.如果未置顶：找到置顶的所有消息，把message移动到它下面，并更新其相关数据
     */
    const int sourceIndex = mMessages.indexOf(message);
    if (sourceIndex == 0) {
        return sourceIndex;
    }

    int targetIndex = 0;
    if (!message->isTop()) {
        for (; targetIndex < sourceIndex; targetIndex++) {
            // 找到第一个不是置顶的消息
            if (!mMessages.at(targetIndex)->isTop()) {
                break;
            }
        }
    }

    moveMessage(sourceIndex, targetIndex);

    return targetIndex;
}

int MessageList::loweredMessage(MessageItem* message, bool last)
{
    const int& sourceIndex = mMessages.indexOf(message);
    const int& s = mMessages.size() - 1;

    if (!message->mIsTop || sourceIndex == s) {
        return sourceIndex;
    }

    int targetIndex = last ? s : sourceIndex;
    for (; targetIndex < s; targetIndex++) {
        // 找到第一个不是置顶的消息
        if (!mMessages.at(targetIndex)->isTop()) {
            break;
        }
    }

    moveMessage(sourceIndex, targetIndex);

    return targetIndex;
}

void MessageList::fetchMore(const QModelIndex& index)
{
    Q_UNUSED(index)

    MessageDatabase::Instance()->detectLanEnvironment(this);
}

void MessageList::load(void)
{
    MessageDatabase::Instance()->refreshMessageList(this);

    int pos = 0;
    for (int i = 0; i < mMessages.size(); i++) {
        if (mMessages.at(i)->isTop()) {
            mMessages.move(i, pos++);
        }
    }
}
