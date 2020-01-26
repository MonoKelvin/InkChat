#ifndef UTILITY_H
#define UTILITY_H

#include <QColor>

class QWidget;
class QFont;
class QLabel;

extern void attachShadowEffect(QWidget *widget, double xoffset = 0.0, double yoffset = 0.0, double radius = 30.0, const QColor& color = QColor("#f5f6fa"));

extern const QString getElidedText(const QString &text, const QFont &font, int maxWidth, Qt::TextElideMode mode = Qt::ElideRight);

extern void getElidedText(QLabel *label, int maxWidth, Qt::TextElideMode mode = Qt::ElideRight);

#define Ceil(x) int(float(x)+0.500000f)

#endif // UTILITY_H
