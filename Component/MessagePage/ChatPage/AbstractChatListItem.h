#ifndef ABSTRACTCHATLISTITEM_H
#define ABSTRACTCHATLISTITEM_H

#include <QSize>
#include <QVariant>

class QPainter;
class QStyleOptionViewItem;

/** 继承自 AbstractChatListItem 的子类必须使用该宏，否则无法注册到聊天视图中 */
#define CHAT_ITEM_OBJECT(_ClassName_, _ChatType_)                        \
public:                                                                  \
    unsigned int getChatType(void) const override { return _ChatType_; } \
                                                                         \
private:                                                                 \
    enum { ChatType = _ChatType_ };

/**
 * @brief 抽象聊天列表的item
 */
class AbstractChatListItem : public QObject {
    Q_OBJECT
public:
    explicit AbstractChatListItem(QObject* parent)
        : QObject(parent)
    {
    }
    virtual ~AbstractChatListItem();

    /**
     * @brief 系统定义的基本聊天视图中内容的类型
     * @note 用户不可重定义
     */
    enum EBaseChatType {
        /** 用户有关 */
        Text = 1, // 普通文本
        RichText, // 富文本
        File, // 文件
        Image, // 图片
        MultiImages, // 多张图片
        Video, // 视频
        Audio, // 音频

        /** 用户无关 */
        Notification, // 系统通知项
    };

    virtual unsigned int getChatType(void) const { return ChatType; }

    /**
     * @brief 绘制内容。针对于QWidget作为界面开发使用
     * @param painter 画笔，需要使用save、restore来暂存设置
     * @param availableRect 可用矩形，即聊天内容可以使用范围，其中可以忽略高度参数
     * @return const QRect 内容矩形
     * @warning 使用availableRect时请注意使用相对位置，比如在矩形中绘制(0,0)点，即左上角，
     * 务必使用(availableRect.left(), availableRect.top())替代，否则视图中无法正常显示
     */
    virtual void paintContent(QPainter* painter, const QRect& availableRect) = 0;

    /**
     * @brief 更新内容区域尺寸
     * @param const QStyleOptionViewItem& 样式
     * @return 返回内容区域尺寸
     */
    virtual void updateContentSize(const QStyleOptionViewItem&) = 0;

    /**
     * @brief 获得相对与聊天视图的区域大小
     * @return const QSize 内容大小
     */
    Q_DECL_CONSTEXPR const QSize getContentSize(void) const noexcept
    {
        return mContentSize;
    }

    /**
     * @brief 是否是自身绘制方法
     * @return 是否自身绘制，如果返回true，则代理中不会显示默认的头像、姓名等，否则只需绘制由
     * @see paintContent 方法实现的消息内容
     */
    virtual bool selfPaint(void)
    {
        return false;
    }

    inline void setData(const QVariant& data) { mData = data; }
    inline const QVariant& getData(void) const { return mData; }

protected:
    /**
     * @brief 内容尺寸，子类需要在 @see updateContentSize 方法内更新该值
     */
    QSize mContentSize;

    /** 发送者数据或其他可用数据 */
    QVariant mData;

private:
    enum { ChatType = 0 };
};

#endif // ABSTRACTCHATLISTITEM_H
