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

enum ESpacing
{
    Tiny = 5,
    Narrow = 8,
    Std = 10,
    Wide = 20,
    Large = 25,
};

enum class EBorderStyle
{
    Rectangle,
    Circle,
    Rounded
};

#define NAV_BUTTON_BASE_SIZE 45

#define AVATAR_BASE_SIZE 45

#define ONLINE_STATE_COLOR "#0bdd4b"

#define OFFLINE_STATE_COLOR "#b0b0b0"

#define BUSY_STATE_COLOR "#ddb80b"

#endif // CONFIGUATION_H
