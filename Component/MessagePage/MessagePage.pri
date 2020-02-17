include( $$PWD/ChatPage/ChatPage.pri )
include( $$PWD/MessageList/MessageList.pri )

INCLUDEPATH *= \
    $$PWD/

HEADERS += \
    $$PWD/MessagePage.h

RESOURCES += \
    $$PWD/MessagePage.qrc

SOURCES += \
    $$PWD/MessagePage.cpp
