// AnimatedButton.cpp - hover lift via shadow blur animation (140ms)
// By Ali Sakkaf - https://alisakkaf.com
#include "AnimatedButton.h"
#include <QGraphicsDropShadowEffect>
#include <QPropertyAnimation>
#include <QEvent>
#include <QFontMetrics>

namespace verax {

AnimatedButton::AnimatedButton(QWidget *parent) : QPushButton(parent) { init(); }
AnimatedButton::AnimatedButton(const QString &text, QWidget *parent)
    : QPushButton(text, parent) { init(); }

void AnimatedButton::init()
{
    setObjectName("AnimatedButton");
    setCursor(Qt::PointingHandCursor);
    setAttribute(Qt::WA_StyledBackground, true);

    m_shadow = new QGraphicsDropShadowEffect(this);
    m_shadow->setBlurRadius(6);
    m_shadow->setOffset(0, 1);
    m_shadow->setColor(QColor(15, 23, 42, 20));
    setGraphicsEffect(m_shadow);

    // Font-metric driven minimum size
    QFontMetrics fm(font());
    const int pad = fm.averageCharWidth() * 3;
    setMinimumHeight(fm.height() + fm.height() / 2);
    if (!text().isEmpty())
        setMinimumWidth(fm.horizontalAdvance(text()) + pad * 2);
}

void AnimatedButton::enterEvent(QEvent *e)
{
    auto *a = new QPropertyAnimation(m_shadow, "blurRadius", this);
    a->setDuration(140);
    a->setStartValue(m_shadow->blurRadius());
    a->setEndValue(14.0);
    a->setEasingCurve(QEasingCurve::OutCubic);
    a->start(QAbstractAnimation::DeleteWhenStopped);
    QPushButton::enterEvent(e);
}

void AnimatedButton::leaveEvent(QEvent *e)
{
    auto *a = new QPropertyAnimation(m_shadow, "blurRadius", this);
    a->setDuration(140);
    a->setStartValue(m_shadow->blurRadius());
    a->setEndValue(6.0);
    a->setEasingCurve(QEasingCurve::OutCubic);
    a->start(QAbstractAnimation::DeleteWhenStopped);
    QPushButton::leaveEvent(e);
}

} // namespace verax
