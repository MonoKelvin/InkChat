#ifndef APPSETTINGS_H
#define APPSETTINGS_H

#include <InkChatApi.h>

#include <QSettings>

class AppSettings : public QSettings {
    Q_OBJECT

    Q_DISABLE_COPY_MOVE(AppSettings)

    explicit AppSettings(QObject* parent = nullptr);

public:
    // APP应用程序文件夹
    inline static const QString AppDir()
    {
        return QCoreApplication::applicationDirPath();
    }

    // APP数据文件夹
    inline static const QString AppDataDir()
    {
        return AppDir() + QStringLiteral("/Data/");
    }

    // APP配置文件
    inline static const QString AppConfigFile()
    {
        return AppDataDir() + QStringLiteral("Config.ini");
    }

    // 用户数据文件夹
    inline static const QString UserDataDir()
    {
        return UserDir() + QStringLiteral("/User/");
    }

    // 用户数据文件
    inline static const QString UserDataFile() noexcept
    {
        QCryptographicHash hash(QCryptographicHash::Md5);
        hash.addData(QString::number(Instance()->mCurrentUser).toLatin1());
        auto result = QString(hash.result().toHex());
        return UserDataDir() + result + QStringLiteral(".udata");
    }

    /**
     * @brief 用户聊天记录文件
     * @param fileName 文件名，不包含路径和后缀
     * @return 返回文件全称名，包含路径和后缀
     */
    inline static const QString MessageCacheFile()
    {
        return UserDataDir() + QStringLiteral("Message.db");
    }

    /**
     * @brief 用户头像地址
     * @param id 用户ID
     * @param size 要获取的大小
     * @return 返回头像http地址
     */
    inline static const QString UserAvatarUrl(unsigned int id, int size = 45)
    {
        return QString("http://inkchat.com/api/image.php?user=user&size=%1&id=%2").arg(size).arg(id);
    }

    // 用户缓存头像文件夹
    inline static const QString AvatarCacheFile(int roleType, unsigned int id)
    {
        return UserDir() + QStringLiteral("/Avatar/%1-%2.thumb").arg(roleType).arg(id);
    }

    // 聊天缓存图片文件夹
    inline static const QString ImageCacheDir()
    {
        return UserDir() + QStringLiteral("/Image/");
    }

    // 聊天缓存文件文件夹
    inline static const QString FileCacheDir()
    {
        return UserDir() + QStringLiteral("/File/");
    }

    // 聊天缓存视频文件夹
    inline static const QString VideoCacheDir()
    {
        return UserDir() + QStringLiteral("/Video/");
    }

    // 聊天缓存音频文件夹
    inline static const QString AudioCacheDir()
    {
        return UserDir() + QStringLiteral("/Audio/");
    }

    inline void setCurrentUserId(unsigned int id)
    {
        mCurrentUser = id;
        Instance()->setValue("user/currentUser", id);
    }

    inline unsigned int getCurrentUserId()
    {
        return mCurrentUser;
    }

    static void LoadAppTheme(const QString& themeFile);

    inline static QVariant Value(const QString& key, const QVariant& defaultValue = QVariant())
    {
        return Instance()->value(key, defaultValue);
    }

    inline static void SetValue(const QString& key, const QVariant& defaultValue = QVariant())
    {
        Instance()->setValue(key, defaultValue);
    }

    inline static QSharedPointer<AppSettings> Instance()
    {
        static QSharedPointer<AppSettings> instance(new AppSettings);
        return instance;
    }

Q_SIGNALS:
    void onAppThemeChanged();

private:
    // 用户文件夹
    inline static const QString UserDir()
    {
        return AppDataDir() + QString::number(Instance()->mCurrentUser);
    }

    unsigned int mCurrentUser;
};

#endif // APPSETTINGS_H
