#ifndef CHATVIEW_H
#define CHATVIEW_H

#include <QObject>

#define CHATVIEW_INITIALIZA \
{ \
    qmlRegisterType<ChatView>("ChatView", 1, 0, "ChatView"); \
}

class ChatView : public QObject
{

};

#endif // CHATVIEW_H
