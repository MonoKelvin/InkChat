#ifndef MESSAGELIST_H
#define MESSAGELIST_H

#include <QObject>

#define MESSAGELIST_INITIALIZA \
{                                                                                           \
    qmlRegisterType<MessageList>("MessageList", 1, 0, "MessageList");   \
}

class MessageList : public QObject
{

};

#endif // MESSAGELIST_H
