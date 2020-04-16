#ifndef INKCHATAPI_H
#define INKCHATAPI_H

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

/** 更新好友信息地址 */
#define UpdateFriendUrl QString(ApiBaseUrl + "/user/update_friend.php?")

#endif // INKCHATAPI_H
