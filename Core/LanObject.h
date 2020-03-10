#ifndef LANOBJECT_H
#define LANOBJECT_H

#include <IChatObject.h>

class QUdpSocket;

/**
 * @brief 局域网聊天类
 */
class LanObject : public IChatObject {
    Q_OBJECT
public:
    LanObject(QObject* parent = nullptr);

    void fromJson(const QJsonObject& json, bool cache) override;
    QJsonObject toJson() override;

    /**
     * @brief 检测局域网环境
     * @return LanObject* 如果检测到局域网环境则创建一个局域网对象，否则返回nullptr
     */
    static LanObject* DetectLanEnvironment();

private:
    // 物理地址，通过和IPv4地址来表示一个唯一局域网对象
    QString mMacAddress;
};

#endif // LANOBJECT_H
