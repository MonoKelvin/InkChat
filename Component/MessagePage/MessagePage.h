#ifndef MESSAGEPAGE_H
#define MESSAGEPAGE_H

#include <ChatView.h>
#include <MessageList.h>

#define MESSAGEPAGE_INITIALIZA                                                      \
    {                                                                               \
        qmlRegisterType<ChatView>("ChatPageManage", 1, 0, "ChatPageManage");        \
        qmlRegisterType<MessageList>("MessageListModel", 1, 0, "MessageListModel"); \
    }

#endif //MESSAGEPAGE_H
