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
    Narrow = 10,
    Std = 15,
    Wide = 20,
    Large = 25,
};

enum class EBorderStyle
{
    Rectangle,
    Circle,
    Rounded
};

#define AVATAR_BASE_SIZE 45

#define ONLINE_STATE_DOT_SIZE 10

#define NOTICE_DOT_SIZE 8

#define ONLINE_STATE_COLOR "#0bdd4b"

#define OFFLINE_STATE_COLOR "#b0b0b0"

#define BUSY_STATE_COLOR "#ddb80b"

#endif // CONFIGUATION_H
