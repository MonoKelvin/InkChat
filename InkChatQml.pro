TEMPLATE = app

QT += qml quick network sql

DEFINES += QT_DEPRECATED_WARNINGS

CONFIG += c++11
CONFIG += c++14
CONFIG += resources_big

include( $$PWD/Element/Element.pri)
include( $$PWD/Component/Component.pri )
include( $$PWD/Core/Core.pri )

SOURCES += main.cpp

RESOURCES += \
    qml.qrc \
    Resource/Resource.qrc

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target