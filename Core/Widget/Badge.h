#ifndef BADGE_H
#define BADGE_H

#include <QLabel>

class Badge : public QLabel
{
    Q_OBJECT
public:
    explicit Badge(QWidget *parent = nullptr);
    explicit Badge(int number, QWidget *parent = nullptr, const QColor &color = QColor("#dd0b33"));
    explicit Badge(const QString &text, QWidget *parent = nullptr, const QColor &color = QColor("#dd0b33"));

    inline void setColor(const QColor &color)
    {
        mColor = color;
        setStyleSheet("background-color:" + mColor.name());
    }

    inline const QColor getColor(void) const { return mColor; }

    /**
     * @brief 设置最大值，只有当内容为数字是才有效
     * @param max 要设置的最大值
     * @note 设置了最大值后，每当重新设置更大值时将会以 max+ 的方式显示
     * @default 默认最大为99
     */
    virtual void setMaxValue(int max);

    /**
     * @brief 转变成一个小点，即不包含任何文字内容，并附加在控件上
     * @param widget 要附加到的控件
     * @param point 要附加到控件的相对坐标点
     * @see GetDotAndAttachWidget();
     */
    void turnIntoDotAndAttachWidget(QWidget *widget, const QPoint &point);

    static Badge *GetDotAndAttachWidget(QWidget *widget, const QPoint &point);

private:
    void init(void);

private:
    QColor mColor;
};

#endif // BADGE_H
