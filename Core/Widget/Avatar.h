#ifndef AVATAR_H
#define AVATAR_H

#include "../User.h"

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

    inline void setDefaultAvatar()
    {
        mAvatar.load(":/App/Icons/default_avatar.png");
        update();
    }

    void setOnlineState(User::EOnlineState state)
    {
        mOnlineState = state;
        update();
    }

    static void DrawAvatar(QPainter *painter, const QRect &rect, const QPixmap &pixmap);

    static const QPixmap GetDefaultPixmap()
    {
        return QPixmap(":/App/Icons/default_avatar.png");
    }

    inline QPixmap getAvatar() const
    {
        return mAvatar;
    }

signals:
    void onClick(void);

protected:
    void mousePressEvent(QMouseEvent *event) override;
    void paintEvent(QPaintEvent *event) override;

private:
    QPixmap mAvatar;

    User::EOnlineState mOnlineState;
};

#endif // AVATAR_H
