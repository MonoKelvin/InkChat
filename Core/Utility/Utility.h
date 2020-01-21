#ifndef UTILITY_H
#define UTILITY_H

class QWidget;
class QColor;

extern void attachShadowEffect(QWidget *widget, const QColor &color, double radius = 20.0, double xoffset = 0.0, double yoffset = 0.0);

#define Ceil(x) int(float(x)+0.500000f)

#endif // UTILITY_H
