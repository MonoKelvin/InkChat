#ifndef AVATAR_H
#define AVATAR_H

#include <IChatObject.h>

#include <QWidget>

class Avatar : public QWidget
{
    Q_OBJECT
public:
    explicit Avatar(QWidget *parent = nullptr);
    explicit Avatar(const QPixmap &pixmap, QWidget *parent = nullptr);
    ~Avatar() override;

    inline void setAvatar(const QPixmap &pixmap)
    {
        mAvatar = pixmap;
        update();
    }

    void setOnlineState(IChatObject::EOnlineState state)
    {
        mOnlineState = state;
        update();
    }

    static void DrawAvatar(
        QPainter* painter,
        const QRect& rect,
        const QPixmap& pixmap,
        const QString& defalultText = QString(),
        IChatObject::EOnlineState state = IChatObject::NoneState);

    inline QPixmap getAvatar() const
    {
        return mAvatar;
    }

Q_SIGNALS:
    void onClick(void);

protected:
    void mousePressEvent(QMouseEvent *event) override;
    void paintEvent(QPaintEvent *event) override;

private:
    QPixmap mAvatar;

    IChatObject::EOnlineState mOnlineState;
};

#endif // AVATAR_H
