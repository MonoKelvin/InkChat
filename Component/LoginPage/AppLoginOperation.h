#ifndef APPLOGINOPERATION_H
#define APPLOGINOPERATION_H

#include <ILoginOperation.h>

/**
 * @brief App系统登录选项，不提供远程登录，一般为用户名+密码验证
 */
class AppLoginOperation : public ILoginOperation
{
    Q_OBJECT

public:
    explicit AppLoginOperation(QObject* parent = nullptr);
    ~AppLoginOperation() override;

    /**
     * @brief 登录请求，只需要密码锁即可
     * @param mapping : 验证的数据键值对
     */
    void loginRequest(const QVariantMap& mapping) override;

    /**
     * @brief 注册用户
     * @param mapping : 注册用到的数据键值对
     */
    void signupRequest(const QVariantMap& mapping) override;

    /**
     * @brief 检查给定用户名的用户是否存在
     * @param userName 要检查用户的用户名
     * @param except 去除指定用户名userName的检索方式
     * @return 如果存在则返回true，否则返回false
     */
    static bool IsUserExists(const QString& userName, bool except = false);
};

#endif // APPLOGINOPERATION_H
