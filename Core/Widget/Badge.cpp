﻿#include "Badge.h"

#include "../Utility.h"
#include "../Configuation.h"

Badge::Badge(QWidget *parent)
    : QLabel(parent)
{
    init();
}

Badge::Badge(int number, QWidget *parent)
    : QLabel(parent)
{
    setNum(number);
    init();
}

Badge::Badge(const QString &text, QWidget *parent)
    : QLabel(parent)
{
    setText(text);
    init();
}

void Badge::init()
{
    setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
    setMinimumSize(ESpacing::Wide, ESpacing::Wide);
    setAlignment(Qt::AlignCenter);
    setScaledContents(true);
    setObjectName("badge");
}

void Badge::turnIntoDotAndAttachWidget(QWidget *widget, const QPoint &point)
{
    setObjectName("dotBadge");
    setParent(widget);
    setMinimumSize(ESpacing::Narrow, ESpacing::Narrow);
    move(point);
}

Badge *Badge::GetDotAndAttachWidget(QWidget *widget, const QPoint &point)
{
    Badge *badge = new Badge(widget);
    badge->turnIntoDotAndAttachWidget(widget, point);

    return badge;
}

