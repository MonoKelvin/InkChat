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

enum ESpacing {
    Tiny = 5,
    Narrow = 10,
    Std = 15,
    Wide = 20,
    Large = 25,
    Extreme = 40,
};

enum class EBorderStyle
{
    Rectangle,
    Circle,
    Rounded
};

/** 主版本号 */
#define CHAT_MAJOR_VERSION 2
/** 次版本号 */
#define CHAT_MINOR_VERSION 0
/** 修订版本号 */
#define CHAT_REVERSION 0

/** 每次刷新时获取聊天消息的最大数量 */
#define CHAT_RECORD_FECTH_COUNT 30

// TODO: 以下数据全部放在类：AppTheme 中

/** 头像大小 */
#define AVATAR_SIZE 46

/** 通知消息的点大小 */
#define NOTICE_DOT_SIZE 8

/** 在线状态的点大小 */
#define ONLINE_STATE_DOT_SIZE 10

/** 在线状态的颜色 */
#define ONLINE_STATE_COLOR "#0bdd4b"
/** 离线状态的颜色 */
#define OFFLINE_STATE_COLOR "#b0b0b0"
/** 忙碌状态的颜色 */
#define BUSY_STATE_COLOR "#ddb80b"

#endif // CONFIGUATION_H
