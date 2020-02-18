include( $$PWD/ChatPage/ChatPage.pri )
include( $$PWD/MessageList/MessageList.pri )

INCLUDEPATH *= \
    $$PWD/

HEADERS += \
    $$PWD/MessageDatabase.h \
    $$PWD/MessagePage.h

RESOURCES += \
    $$PWD/MessagePage.qrc

SOURCES += \
    $$PWD/MessageDatabase.cpp
