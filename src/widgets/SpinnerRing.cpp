// SpinnerRing.cpp
// By Ali Sakkaf - https://alisakkaf.com
#include "SpinnerRing.h"
#include <QPainter>
#include <QTimer>

namespace verax {

SpinnerRing::SpinnerRing(QWidget *parent) : QWidget(parent)
{
    setObjectName("SpinnerRing");
    m_timer = new QTimer(this);
    m_timer->setInterval(30);
    connect(m_timer, &QTimer::timeout, this, [this]{
        m_angle = (m_angle + 7) % 360;
        update();
    });
}

void SpinnerRing::start() { if (!m_timer->isActive()) m_timer->start(); }
void SpinnerRing::stop()  { m_timer->stop(); }

void SpinnerRing::paintEvent(QPaintEvent *)
{
    QPainter p(this);
    p.setRenderHint(QPainter::Antialiasing);

    const int side = qMin(width(), height());
    QRectF box(QRectF(0, 0, side, side).adjusted(3, 3, -3, -3)
               .translated((width()-side)/2.0, (height()-side)/2.0));

    QPen base(QColor("#E2E8F0"));
    base.setWidthF(side * 0.10);
    base.setCapStyle(Qt::RoundCap);
    p.setPen(base);
    p.drawEllipse(box);

    QPen arc(QColor("#2563EB"));
    arc.setWidthF(side * 0.10);
    arc.setCapStyle(Qt::RoundCap);
    p.setPen(arc);
    p.drawArc(box, (90 - m_angle) * 16, -100 * 16);
}

} // namespace verax
