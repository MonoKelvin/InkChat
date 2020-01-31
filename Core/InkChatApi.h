#ifndef INKCHATAPI_H
#define INKCHATAPI_H

/** 基本Url地址 */
#define BaseUrl         QLatin1String("http://api.inkchat.com")
//#define BaseUrl       "http://localhost:8080";

/** 用户信息Url地址 */
#define UserUrl     QString(BaseUrl + "/user")

/** 用户验证登录（自动登录）Url地址，post请求方式 */
#define UserVerifyUrl   QString(UserUrl+"/login.php?type=verify")

/** 用户账号密码登录地址，post请求方式 */
#define UserLoginUrl    QString(UserUrl+"/login.php?type=login")

/** 用户登出Url地址，post请求方式 */
#define UserLogOutUrl  QString(UserUrl+"/logout.php")

/** 用户注册Url地址，post请求方式 */
#define UserSignupUrl       QString(UserUrl + "/signup.php")

#define CHAT_MAJOR_VERSION  1
#define CHAT_MINOR_VERSION  0
#define CHAT_REVERSION      0


#endif // INKCHATAPI_H
