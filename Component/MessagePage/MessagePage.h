#ifndef MESSAGEPAGE_H
#define MESSAGEPAGE_H

#include <ChatView.h>
#include <IChatItem.h>
#include <MessageList.h>

#define MESSAGEPAGE_INITIALIZA                                                      \
    {                                                                               \
        qmlRegisterType<ChatView>("ChatListModel", 1, 0, "ChatListModel");          \
        qmlRegisterType<MessageList>("MessageListModel", 1, 0, "MessageListModel"); \
    }

#endif //MESSAGEPAGE_H
