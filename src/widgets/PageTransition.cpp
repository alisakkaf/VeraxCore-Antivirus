// PageTransition.cpp - 280ms OutCubic slide+fade
// By Ali Sakkaf - https://alisakkaf.com
#include "PageTransition.h"
#include <QStackedWidget>
#include <QPropertyAnimation>
#include <QParallelAnimationGroup>
#include <QGraphicsOpacityEffect>

namespace verax {

PageTransition::PageTransition(QStackedWidget *stack, QObject *parent)
    : QObject(parent), m_stack(stack) {}

void PageTransition::slideTo(int index)
{
    if (!m_stack || index < 0 || index >= m_stack->count()) return;
    if (m_stack->currentIndex() == index) return;

    QWidget *next = m_stack->widget(index);
    if (!next) return;

    m_stack->setCurrentIndex(index);

    auto *fx = new QGraphicsOpacityEffect(next);
    fx->setOpacity(0.0);
    next->setGraphicsEffect(fx);

    auto *group = new QParallelAnimationGroup(next);

    auto *fade = new QPropertyAnimation(fx, "opacity");
    fade->setDuration(280);
    fade->setStartValue(0.0);
    fade->setEndValue(1.0);
    fade->setEasingCurve(QEasingCurve::OutCubic);

    auto *slide = new QPropertyAnimation(next, "pos");
    const QPoint target = next->pos();
    slide->setDuration(280);
    slide->setStartValue(target + QPoint(24, 0));
    slide->setEndValue(target);
    slide->setEasingCurve(QEasingCurve::OutCubic);

    group->addAnimation(fade);
    group->addAnimation(slide);
    QObject::connect(group, &QParallelAnimationGroup::finished, [next]{
        next->setGraphicsEffect(nullptr);
    });
    group->start(QAbstractAnimation::DeleteWhenStopped);
}

} // namespace verax
