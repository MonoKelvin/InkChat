#ifndef CONFIGUATION_H
#define CONFIGUATION_H

/** 主版本号 */
#define CHAT_MAJOR_VERSION 2
/** 次版本号 */
#define CHAT_MINOR_VERSION 0
/** 修订版本号 */
#define CHAT_REVERSION 0
/** 版本号字符串标识 */
#define CHAT_VERSION_STR "2.0.0"

/** 每次刷新时获取聊天消息的最大数量 */
#define CHAT_RECORD_FECTH_COUNT 20

/** 定义端口号类型 */
typedef unsigned short Port;

/** 定义默认局域网UDP协议端口号为2020 */
#define LAN_UDP_PORT 2020

/** 定义默认TCP连接的端口号为2021 */
#define LAN_TCP_PORT 2021

/** 最大文件传输块（字节） */
#define MAX_FILE_TRANSFER_BLOCK 65536

/**
 * @brief App状态
 */
enum class EAppState {
    Online,
    Offline
};

#endif // CONFIGUATION_H
