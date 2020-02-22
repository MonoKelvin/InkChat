INCLUDEPATH *= \
    $$PWD/

HEADERS += \
    $$PWD/AppSettings.h \
        $$PWD/Http/HttpRequest.h \
        $$PWD/Http/ILoginOperation.h \
    $$PWD/IChatObject.h \
        $$PWD/InkChatApi.h \
    $$PWD/LanObject.h \
        $$PWD/User.h \
        $$PWD/Utility.h \
    $$PWD/MyFriend.h \
    $$PWD/MessageDatabase.h\
    $$PWD/ChatManager.h

SOURCES += \
    $$PWD/AppSettings.cpp \
        $$PWD/Http/HttpRequest.cpp \
        $$PWD/Http/ILoginOperation.cpp \
    $$PWD/IChatObject.cpp \
    $$PWD/LanObject.cpp \
        $$PWD/User.cpp \
        $$PWD/Utility.cpp \
    $$PWD/MyFriend.cpp \
    $$PWD/MessageDatabase.cpp\
    $$PWD/ChatManager.cpp
