#include "LoadingIndicator.h"

#include <AppTheme.h>
#include <Utility.h>

LoadingIndicator::LoadingIndicator(QWidget* parent)
    : QWidget(parent)
    , mStartAngle(90)
{
    resize(INDICATOR_SIZE, INDICATOR_SIZE);
    move((parent->width() - INDICATOR_SIZE) / 2, -INDICATOR_SIZE - 2);
    setStyleSheet("background:white; border-radius: 14px;");
    attachShadowEffect(this, 0, 4.0, 15.0);

    mTimer = new QTimer(this);
    connect(mTimer, &QTimer::timeout, this, &LoadingIndicator::updateIndicator);
}

void LoadingIndicator::paintEvent(QPaintEvent*)
{
    if (y() <= -height()) {
        return;
    }

    USE_STYLE_SHEET
    QPainter painter(this);

    painter.setRenderHint(QPainter::Antialiasing);
    painter.setPen(QPen(XTheme.PrimayColor1, INDICATOR_RING_WIDTH));

    if (mTimer->isActive()) {
        painter.drawArc(rect().adjusted(6, 6, -6, -6), mStartAngle * 16, 4480);
    } else {
        constexpr int l = 4480 / INDICATOR_PULLDOWN_LIMIT;
        painter.setOpacity(qMin(1.0, y() / 36.0));
        painter.drawArc(rect().adjusted(6, 6, -6, -6), mStartAngle * 16, y() * l);
    }
}
