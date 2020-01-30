#ifndef NAVIGATION_H
#define NAVIGATION_H

#include <QObject>

#define NAVIGATION_INITIALIZA \
{ \
    qmlRegisterType<ChatView>("Navigation", 1, 0, "Navigation"); \
}

class Navigation : public QObject
{

};

#endif // NAVIGATION_H
