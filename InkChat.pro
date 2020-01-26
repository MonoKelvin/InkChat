QT  += core gui
QT  += network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    Core/Widget/ChatInputBox.cpp \
    Core/Widget/Navigation.cpp \
    Core/LoginWithQQMail.cpp \
    Core/User.cpp \
    Core/Utility/HttpRequest.cpp \
    Core/Utility/Utility.cpp \
    Core/Widget/Avatar.cpp \
    Core/Widget/Badge.cpp \
    Core/Widget/ChatItem.cpp \
    Core/Widget/ChatListView.cpp \
    Core/Widget/IChatWidget.cpp \
    Core/Widget/MessageItem.cpp \
    Core/Widget/MessageList.cpp \
    Core/Widget/PromptWidget.cpp \
    Core/Widget/SearchableList.cpp \
    main.cpp \
    MainWindow.cpp

HEADERS += \
    Core/Configuation.h \
    Core/Widget/ChatInputBox.h \
    Core/Widget/Navigation.h \
    Core/InkChatApi.h \
    Core/LoginWithQQMail.h \
    Core/User.h \
    Core/Utility/HttpRequest.h \
    Core/Utility/ILoginOperation.h \
    Core/Utility/Utility.h \
    Core/Widget/Avatar.h \
    Core/Widget/Badge.h \
    Core/Widget/ChatItem.h \
    Core/Widget/ChatListView.h \
    Core/Widget/IChatWidget.h \
    Core/Widget/MessageItem.h \
    Core/Widget/MessageList.h \
    Core/Widget/PromptWidget.h \
    Core/Widget/SearchableList.h \
    MainWindow.h

FORMS += \
    UI/mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    Resource/resources.qrc

DISTFILES += \
    UI/QML/SearchableList.qml
