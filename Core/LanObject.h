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

    void fromJson(const QJsonObject& json, bool cache = true) override;
    QJsonObject toJson() override;
    bool updateLocalData() override;

    /**
     * @brief 检测局域网环境
     * @return LanObject* 如果检测到局域网环境则创建一个局域网对象，否则返回nullptr
     * @note 新建的局域网实例没有设置id，所以需要手动设置，否则无法从数据库中进行索引，而且不
     * 会缓存此时获取的相关数据，所以一般设置好id后立即调用 @see LanObject::updataLocalData() 方法
     */
    static LanObject* DetectLanEnvironment();

    inline const QString getMacAddress(void) const { return mMacAddress; }

private:
    // 物理地址，通过和IPv4地址来表示一个唯一局域网对象
    QString mMacAddress;
};

#endif // LANOBJECT_H
