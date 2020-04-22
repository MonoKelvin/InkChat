INCLUDEPATH *= $$PWD

HEADERS += \
    $$PWD/AbstractChatListItem.h \
    $$PWD/ChatItem.h \
    $$PWD/ChatList.h \
    $$PWD/NotificationItem.h \
    $$PWD/TextChatItem.h \
    $$PWD/UI/ChatInputBox.h \
    $$PWD/UI/ChatViewWidget.h

SOURCES += \
    $$PWD/AbstractChatListItem.cpp \
    $$PWD/ChatItem.cpp \
    $$PWD/ChatList.cpp \
    $$PWD/NotificationItem.cpp \
    $$PWD/TextChatItem.cpp \
    $$PWD/UI/ChatInputBox.cpp \
    $$PWD/UI/ChatViewWidget.cpp

FORMS += \
    $$PWD/UI/chatviewwidget.ui
