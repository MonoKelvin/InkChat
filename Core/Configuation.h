#ifndef CONFIGUATION_H
#define CONFIGUATION_H

/** 主版本号 */
#define CHAT_MAJOR_VERSION 2
/** 次版本号 */
#define CHAT_MINOR_VERSION 0
/** 修订版本号 */
#define CHAT_REVERSION 0

/** 每次刷新时获取聊天消息的最大数量 */
#define CHAT_RECORD_FECTH_COUNT 30

/**
 * @brief App状态
 */
enum class EAppState {
    Online,
    Offline
};

#endif // CONFIGUATION_H
