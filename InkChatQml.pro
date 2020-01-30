TEMPLATE = app

QT += qml quick widgets

DEFINES += QT_DEPRECATED_WARNINGS

CONFIG += c++11
CONFIG += c++14

include( $$PWD/Element/Element.pri)
include( $$PWD/Component/Component.pri )

SOURCES += \
        Core/User.cpp \
        Core/Utility.cpp \
        main.cpp

RESOURCES += \
    qml.qrc \
    Resource/Resource.qrc

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

HEADERS += \
    Core/Configuation.h \
    Core/InkChatApi.h \
    Core/User.h \
    Core/Utility.h
