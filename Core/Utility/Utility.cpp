#include "Utility.h"

#include <QLabel>
#include <QGraphicsDropShadowEffect>

void attachShadowEffect(QWidget *widget, double xoffset, double yoffset, double radius, const QColor &color)
{
    QGraphicsDropShadowEffect *effect = new QGraphicsDropShadowEffect(widget);
    effect->setOffset(xoffset, yoffset);
    effect->setColor(color);
    effect->setBlurRadius(radius);
    widget->setGraphicsEffect(effect);
}

void getElidedText(QLabel *label, int maxWidth, Qt::TextElideMode mode)
{
    label->setFixedWidth(maxWidth);

    const auto fw = label->fontMetrics();
    const auto et = fw.elidedText(label->text(), mode, maxWidth - 3*fw.averageCharWidth());

    label->setText(et);
    label->adjustSize();
}
