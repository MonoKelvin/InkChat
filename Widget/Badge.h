#ifndef BADGE_H
#define BADGE_H

#include <QLabel>

class Badge : public QLabel
{
    Q_OBJECT
public:
    explicit Badge(QWidget *parent = nullptr);
    explicit Badge(int number, QWidget *parent = nullptr);
    explicit Badge(const QString &text, QWidget *parent = nullptr);

    /**
     * @brief 转变成一个小点，即不包含任何文字内容，并附加在控件上
     * @param widget 要附加到的控件
     * @param point 要附加到控件的相对坐标点，即badge左上角的位置
     * @see GetDotAndAttachWidget();
     */
    void turnIntoDotAndAttachWidget(QWidget *widget, const QPoint &point);

    static Badge *GetDotAndAttachWidget(QWidget *widget, const QPoint &point);

private:
    void init(void);
};

#endif // BADGE_H
