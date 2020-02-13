#include "MessageList.h"

#include <InkChatApi.h>
#include <MessageItem.h>
#include <MyFriend.h>
#include <Utility.h>

#include <QDir>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QMutex>

MessageList::MessageList(QObject* parent)
    : QAbstractListModel(parent)
{
    mMutex = new QMutex(QMutex::Recursive);
}

MessageList::~MessageList()
{
    qDeleteAll(mMessages.begin(), mMessages.end());
    mMessages.clear();

    if (mMutex) {
        delete mMutex;
        mMutex = nullptr;
    }
}

bool MessageList::ariseMessage(MessageItem* message, bool ignoreTop)
{
    Q_ASSERT(message != nullptr);

    const int sourceIndex = getRow(message);
    if (sourceIndex == -1) {
        return false;
    }

    int targetIndex = 0;
    if (!message->mChatObject->getIsTop() && !ignoreTop) {
        for (; targetIndex < mMessages.size(); targetIndex++) {
            if (!mMessages.at(targetIndex)->mChatObject->getIsTop()) {
                break;
            }
        }
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

bool MessageList::refresh()
{
    return true;
}

void MessageList::load(const QString& crFolder /*, MessageList::EMessageFilter filter*/)
{
    isDirExists(crFolder, true);

    QJsonParseError jsonError;
    QJsonDocument jsonDoc;
    QJsonObject json;

    //    QFile crFile;
    const QDir dir(crFolder);
    auto list = dir.entryInfoList(QStringList() << "*.cr", QDir::Files | QDir::Hidden);
    for (auto i : list) {
        QFile crFile(i.filePath());
        try {
            if (!crFile.open(QFile::ReadOnly | QFile::Text)) {
                crFile.close();
                throw QStringLiteral("FILE_OPEN_FAILED: ") + i.filePath();
            }

            // TODO: 解密

            jsonDoc = QJsonDocument::fromJson(crFile.readAll(), &jsonError);
            crFile.close();

            if (jsonError.error != QJsonParseError::NoError || !jsonDoc.isObject()) {
                throw QStringLiteral("DATA_FORMAT_ERROR: ") + jsonError.errorString();
            }

            json = jsonDoc.object();

            if (!json.value(QStringLiteral("chat")).toBool(false)) {
                continue;
            }

            // ID数据是否和文件名匹配，匹配说明数据可能为未篡改
            const auto id = unsigned(json.value(QStringLiteral("id")).toDouble(0));
            if (i.baseName() != encryptTextByMD5(QString::number(id), true)) {
                throw QStringLiteral("DATA_MISMATCH: VALUE=id");
            }

            // 读取发送者
            IChatObject* chatObj = nullptr;
            const auto roleType = json.value(QStringLiteral("roleType")).toInt(-1);
            switch (roleType) {
            case IChatObject::Friend:
                chatObj = new MyFriend;
                chatObj->setID(id);
                // TODO: 获取头像和名字
                break;
            default:
                throw QStringLiteral("CHAT_OBJECT_NOT_FOUND: VALUE=%1").arg(roleType);
            }

            // 生成聊天消息数据
            MessageItem* message = new MessageItem(chatObj, new QJsonObject(json));
            message->update();

            appendMessage(message);
            emit loaded();
        } catch (const QString& err) {
            // todo: log
            qDebug() << err;
        }
    }
}

void MessageList::save(const QString& crFolder, bool onlyDirty)
{
}

void MessageList::receiveMessage(IChatObject* chatObj, const QString& message, const QString& time)
{
    /**
     * 1.如果消息列表中有目标消息项
     *  a.如果置顶：直接更新其相关数据
     *  b.如果未置顶：找到置顶的所有消息，把message移动到它下面，并更新其相关数据
     * 2.如果消息列表中没有目标消息项
     *  a.如果置顶：直接插入到最上方，设置其相关数据
     *  b.如果未置顶：找到置顶的所有消息，new一个消息到它下面，并设置其相关数据
     */
    int i = 0;
    for (; i < mMessages.size(); i++) {
        if (mMessages[i]->mChatObject == chatObj) {
            break;
        }
    }
    auto msgItem = (i == mMessages.size()) ? mMessages[i] : nullptr;

    if (!msgItem)
        msgItem = new MessageItem(chatObj);

    ariseMessage(msgItem);
}
