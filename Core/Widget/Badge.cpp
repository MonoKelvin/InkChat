#include "Badge.h"

#include "../Utility/Utility.h"
#include "../Configuation.h"

Badge::Badge(QWidget *parent)
    : QLabel(parent)
    , mColor("#dd0b33")
{
    init();
}

Badge::Badge(int number, QWidget *parent, const QColor &color)
    : QLabel(parent)
    , mColor(color)
{
    setText(QString::number(number));
    init();
}

Badge::Badge(const QString &text, QWidget *parent, const QColor &color)
    : QLabel(parent)
    , mColor(color)
{
    setText(text);
    init();
}

void Badge::init()
{
    setStyleSheet("background-color:" + mColor.name());
    setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
    setMinimumSize(ESpacing::Wide, ESpacing::Wide);
    setAlignment(Qt::AlignCenter);
    setScaledContents(true);
    setMaxValue(99);
}

void Badge::setMaxValue(int max)
{
    bool ok;
    int result = text().toInt(&ok);
    if (ok)
    {
        if (result > max)
        {
            setText(QString::number(max) + "+");
            setStyleSheet(styleSheet() + ";padding:0 5px;");
        }

        setObjectName("numberNotice");
        return;
    }

    setObjectName("badge");
}

void Badge::turnIntoDotAndAttachWidget(QWidget *widget, const QPoint &point)
{
    setObjectName("dotNotice");
    setParent(widget);
    setMinimumSize(ESpacing::Narrow, ESpacing::Narrow);
    move(point);
}

