#include "Utility.h"

#include <QWidget>
#include <QGraphicsDropShadowEffect>

void attachShadowEffect(QWidget *widget, const QColor &color, qreal radius, qreal xoffset, qreal yoffset)
{
    QGraphicsDropShadowEffect *effect = new QGraphicsDropShadowEffect(widget);
    effect->setOffset(xoffset, yoffset);
    effect->setColor(color);
    effect->setBlurRadius(radius);
    widget->setGraphicsEffect(effect);
}

const QString getElidedText(QString text, const QFont &font, int maxWidth)
{
    QFontMetrics fontWidth(font);

    if (fontWidth.horizontalAdvance(text) >= maxWidth)
    {
        text = fontWidth.elidedText(text, Qt::ElideRight, maxWidth);
    }

    return text;
}
