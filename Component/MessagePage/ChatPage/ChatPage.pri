INCLUDEPATH *= $$PWD

HEADERS += \
    $$PWD/AbstractChatListItem.h \
    $$PWD/ChatItem.h \
    $$PWD/ChatList.h \
    $$PWD/FileChatItem.h \
    $$PWD/MemberList.h \
    $$PWD/NotificationItem.h \
    $$PWD/TextChatItem.h \
    $$PWD/UI/ChatInputBox.h \
    $$PWD/UI/ChatViewWidget.h \
    $$PWD/UI/MemberViewWidget.h

SOURCES += \
    $$PWD/AbstractChatListItem.cpp \
    $$PWD/ChatItem.cpp \
    $$PWD/ChatList.cpp \
    $$PWD/FileChatItem.cpp \
    $$PWD/MemberList.cpp \
    $$PWD/NotificationItem.cpp \
    $$PWD/TextChatItem.cpp \
    $$PWD/UI/ChatInputBox.cpp \
    $$PWD/UI/ChatViewWidget.cpp \
    $$PWD/UI/MemberViewWidget.cpp

FORMS += \
    $$PWD/UI/chatviewwidget.ui
