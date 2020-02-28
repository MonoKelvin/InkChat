﻿#ifndef INKCHATAPI_H
#define INKCHATAPI_H

#include <QCoreApplication>
#include <QCryptographicHash>
#include <QDebug>

#define CHAT_MAJOR_VERSION 1
#define CHAT_MINOR_VERSION 0
#define CHAT_REVERSION 0

/** 每次刷新时获取聊天消息的最大数量 */
#define CHAT_RECORD_FECTH_COUNT 30

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
#define MyFriendsUrl QString(ApiBaseUrl + "/user/friend.php")

/** 更新好友信息地址，气候参数需自己制定 */
#define UpdateFriendUrl QString(ApiBaseUrl + "user/update_friend.php?")

// 解析消息出错时发送失败信号
#define EMIT_FAILED_MESSAGE(_JsonDoc_, _FailSignal_)                        \
    do                                                                      \
    {                                                                       \
        const auto obj = _JsonDoc_.object();                                \
        if (obj.contains(QStringLiteral("msg")))                            \
            emit _FailSignal_(obj.value(QStringLiteral("msg")).toString()); \
        else                                                                \
            emit _FailSignal_(QStringLiteral("SERVER_NO_RESPONSE"));        \
    } while (0)

#endif // INKCHATAPI_H
