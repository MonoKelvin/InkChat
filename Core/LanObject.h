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
};

#endif // LANOBJECT_H
