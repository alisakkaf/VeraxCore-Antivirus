// SurfaceCard.cpp - styling driven by QSS via objectName/property
// By Ali Sakkaf - https://alisakkaf.com
#include "SurfaceCard.h"
#include <QGraphicsDropShadowEffect>
#include <QEvent>
#include <QStyle>
#include <QVariant>

#include "src/core/Settings.h"

namespace verax {

SurfaceCard::SurfaceCard(QWidget *parent) : QFrame(parent)
{
    setObjectName("SurfaceCard");
    setProperty("class", "SurfaceCard");
    setFrameShape(QFrame::NoFrame);
    setAttribute(Qt::WA_StyledBackground, true);

    if (verax::Settings::instance().language() != "ar") {
        auto *shadow = new QGraphicsDropShadowEffect(this);
        shadow->setBlurRadius(12);
        shadow->setOffset(0, 2);
        shadow->setColor(QColor(15, 23, 42, 18));
        setGraphicsEffect(shadow);
    }
}

void SurfaceCard::setInteractive(bool v)
{
    m_interactive = v;
    setProperty("interactive", v);
    if (auto *e = qobject_cast<QGraphicsDropShadowEffect*>(graphicsEffect())) {
        e->setBlurRadius(v ? 16 : 12);
    }
    style()->unpolish(this); style()->polish(this);
}

void SurfaceCard::enterEvent(QEvent *e)
{
    if (m_interactive)
        if (auto *fx = qobject_cast<QGraphicsDropShadowEffect*>(graphicsEffect())) {
            fx->setBlurRadius(20);
            fx->setColor(QColor(15, 23, 42, 26));
        }
    QFrame::enterEvent(e);
}
void SurfaceCard::leaveEvent(QEvent *e)
{
    if (m_interactive)
        if (auto *fx = qobject_cast<QGraphicsDropShadowEffect*>(graphicsEffect())) {
            fx->setBlurRadius(12);
            fx->setColor(QColor(15, 23, 42, 18));
        }
    QFrame::leaveEvent(e);
}

} // namespace verax
