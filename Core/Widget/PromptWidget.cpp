#include "PromptWidget.h"

#include "../Utility.h"

#include <QLabel>
#include <QHBoxLayout>
#include <QPropertyAnimation>
#include <QSequentialAnimationGroup>

PromptWidget::PromptWidget(const QString &prompt, QWidget *parent)
    : QWidget(parent)
{
    buildUI(prompt);
}

PromptWidget::PromptWidget(const QString &prompt, QWidget *parent, PromptWidget::PromptType type)
    : QWidget(parent)
{
    buildUI(prompt);
    show(type);
}

void PromptWidget::show(PromptWidget::PromptType type)
{
    if (type == PromptType::Alert)
    {
        setObjectName(QStringLiteral("promptAlert"));
    }
    else if (type == PromptType::Prompt)
    {
        setObjectName(QStringLiteral("promptPrompt"));
    }

    QSize pwSize = parentWidget()->size();
    int xOffset = (pwSize.width() - this->width()) / 2;
    QRect oriRect = QRect(xOffset, pwSize.height() + 10, this->width(), this->height());

    QPropertyAnimation *showAni = new QPropertyAnimation(this, "geometry", this);
    showAni->setDuration(300);
    showAni->setStartValue(oriRect);

    int yOffset = this->height() + 20;
    oriRect.setTop(oriRect.top() - yOffset);
    oriRect.setBottom(oriRect.bottom() - yOffset);
    showAni->setEndValue(oriRect);
    showAni->setEasingCurve(QEasingCurve::OutBack);

    QPropertyAnimation *quitAni = new QPropertyAnimation(this, "geometry", this);
    quitAni->setDuration(300);
    quitAni->setStartValue(showAni->endValue());
    quitAni->setEndValue(showAni->startValue());
    quitAni->setEasingCurve(QEasingCurve::InBack);

    QSequentialAnimationGroup *seqAnimation = new QSequentialAnimationGroup(this);

    seqAnimation->addAnimation(showAni);
    seqAnimation->addPause(3000);
    seqAnimation->addAnimation(quitAni);
    seqAnimation->setLoopCount(1);

    seqAnimation->start();

    connect(seqAnimation, &QSequentialAnimationGroup::finished, this, &PromptWidget::deleteLater);

    QWidget::show();
}

void PromptWidget::buildUI(const QString &prompt)
{
    QLabel *lbContents = new QLabel(prompt, this);
    QHBoxLayout *hbLayout = new QHBoxLayout(this);

    QFontMetrics font(lbContents->font());
    lbContents->setAlignment(Qt::AlignCenter);
    lbContents->setWordWrap(true);

    hbLayout->addWidget(lbContents);
    hbLayout->setMargin(10);

    this->resize(100 + font.horizontalAdvance(prompt), font.height() + 50);
}
