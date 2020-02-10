INCLUDEPATH *= \
    $$PWD/

HEADERS += \
    $$PWD/AppSettings.h \
        $$PWD/Http/HttpRequest.h \
        $$PWD/Http/ILoginOperation.h \
        $$PWD/IPerson.h \
        $$PWD/InkChatApi.h \
        $$PWD/User.h \
        $$PWD/Utility.h \
    $$PWD/MyFriend.h

SOURCES += \
    $$PWD/AppSettings.cpp \
        $$PWD/Http/HttpRequest.cpp \
        $$PWD/Http/ILoginOperation.cpp \
        $$PWD/IPerson.cpp \
        $$PWD/User.cpp \
        $$PWD/Utility.cpp \
    $$PWD/MyFriend.cpp
