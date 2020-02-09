#ifndef INKCHATAPI_H
#define INKCHATAPI_H

#include <QCoreApplication>
#include <QCryptographicHash>
#include <QDebug>

/** 基本Url地址 */
#define ApiBaseUrl QStringLiteral("http://inkchat.com/api")
//#define BaseUrl       "http://localhost:8080";

/** 用户账号密码登录地址，post请求方式 */
#define LoginByPasswordUrl QString(ApiBaseUrl + "/user/login.php?verify_method=password")

/** 用户账号密码登录地址，post请求方式 */
#define LoginByIdUrl QString(ApiBaseUrl + "/user/login.php?verify_method=id")

/** 用户登出Url地址，post请求方式 */
#define LogOutUrl QString(ApiBaseUrl + "/user/logout.php")

/** 用户注册Url地址，post请求方式 */
#define SignupUrl QString(ApiBaseUrl + "/user/signup.php")

/** 获取好友信息 */
#define MyFriendUrl QString(ApiBaseUrl + "/user/friend.php");

#define CHAT_MAJOR_VERSION 1
#define CHAT_MINOR_VERSION  0
#define CHAT_REVERSION 0

/********************/
/** 文件数据文件夹路径 */
/********************/

// APP应用程序文件夹
#define APP_DIR QCoreApplication::applicationDirPath()

// APP数据文件夹
#define APP_DATA_DIR (APP_DIR + QStringLiteral("/data/"))

// 用户文件夹
#define USER_DIR(_ID_) (APP_DATA_DIR + QString::number(_ID_))
// 用户数据文件夹
#define USER_DATA_DIR(_ID_) (USER_DIR(_ID_) + QStringLiteral("/user/"))
// 用户数据文件
#define USER_DATA_FILE(_ID_)                                            \
    ([&]() noexcept->QString {                                          \
        QCryptographicHash hash(QCryptographicHash::Md5);               \
        hash.addData(QString::number(_ID_).toLatin1());                 \
        auto result = QString(hash.result().toHex());                   \
        return USER_DATA_DIR(_ID_) + result + QStringLiteral(".udata"); \
    }())

// 用户缩略头像
#define USER_THUMB_URL(_ID_) (QStringLiteral("http://inkchat.com/api/image.php?user=user&type=thumb&id=") + QString::number(_ID_))
// 用户完整头像
#define USER_AVATAR_URL(_ID_) (QStringLiteral("http://inkchat.com/api/image.php?user=user&type=avatar&id=") + QString::number(_ID_))
// 群聊缩略头像
#define GROUP_THUMB_URL(_ID_) (QStringLiteral("http://inkchat.com/api/image.php?user=group&type=thumb&id=") + QString::number(_ID_))
// 群聊完整头像
#define GROUP_AVATAR_URL(_ID_) (QStringLiteral("http://inkchat.com/api/image.php?user=group&type=avatar&id=") + QString::number(_ID_))

// 用户聊天记录文件夹
#define MESSAGE_CACHE_DIR(_ID_) (USER_DIR(_ID_) + QStringLiteral("/cache/message/"))
// 用户缓存头像文件夹
#define AVATAR_CACHE_DIR(_ID_) (USER_DIR(_ID_) + "/cache/avatar/")
// 聊天缓存图片文件夹
#define IMAGE_CACHE_DIR(_ID_) (USER_DIR(_ID_) + "/cache/image/")
// 聊天缓存文件文件夹
#define FILE_CACHE_DIR(_ID_) (USER_DIR(_ID_) + "/cache/file/")
// 聊天缓存视频文件夹
#define VIDEO_CACHE_DIR(_ID_) (USER_DIR(_ID_) + "/cache/video/")
// 聊天缓存音频文件夹
#define AUDIO_CACHE_DIR(_ID_) (USER_DIR(_ID_) + "/cache/audio/")

#endif // INKCHATAPI_H
