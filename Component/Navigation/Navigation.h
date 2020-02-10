#ifndef NAVIGATION_H
#define NAVIGATION_H

#include <QObject>

#define NAVIGATION_INITIALIZA                                                \
    {                                                                        \
        qmlRegisterType<Navigation>("Navigation", 1, 0, "NavigationManage"); \
    }

class Navigation : public QObject
{

};

#endif // NAVIGATION_H
