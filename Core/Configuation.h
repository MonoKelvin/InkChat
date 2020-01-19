#ifndef CONFIGUATION_H
#define CONFIGUATION_H

#include <QDebug>

enum class EAlignment
{
    Top,
    Bottom,
    Left,
    Right,
    Center,
};

enum class EOrientation
{
    Vertical,
    Horizontal,
};

#define NAV_BUTTON_BASE_SIZE 45

enum ESpacing
{
    Tiny = 5,
    Narrow = 10,
    Std = 15,
    Wide = 20,
    Large = 25,
};

#endif // CONFIGUATION_H
