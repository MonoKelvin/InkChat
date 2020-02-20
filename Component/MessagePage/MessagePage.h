#ifndef MESSAGEPAGE_H
#define MESSAGEPAGE_H

#include <ChatView.h>
#include <MessageList.h>

// Chat Item Classes
#include <TextChatItem.h>

#define MESSAGEPAGE_INITIALIZA                                                      \
    {                                                                               \
        qmlRegisterType<ChatView>("ChatListModel", 1, 0, "ChatListModel");          \
        qmlRegisterType<MessageList>("MessageListModel", 1, 0, "MessageListModel"); \
        ChatView::RegisterChatItemClass<TextChatItem>();                            \
    }

#endif //MESSAGEPAGE_H
