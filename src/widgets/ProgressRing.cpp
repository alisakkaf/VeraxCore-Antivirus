#include "ProgressRing.h"
#include <QPainter>
#include <QPropertyAnimation>

namespace verax {

ProgressRing::ProgressRing(QWidget *parent) : QWidget(parent)
{
    setObjectName("ProgressRing");
    setMinimumSize(160, 160);
}

QSize ProgressRing::sizeHint() const { return QSize(220, 220); }

void ProgressRing::setValue(qreal v)
{
    v = qBound(0.0, v, 1.0);
    if (qFuzzyCompare(m_value, v)) return;
    m_value = v;

    QPropertyAnimation *anim = new QPropertyAnimation(this, "displayed", this);
    anim->setDuration(250);
    anim->setStartValue(m_displayed);
    anim->setEndValue(v);
    anim->setEasingCurve(QEasingCurve::OutQuad);
    anim->start(QAbstractAnimation::DeleteWhenStopped);
}

void ProgressRing::setCenterText(const QString &t) { m_centerText = t; update(); }
void ProgressRing::setMode(const QString &m)        { m_mode = m; update(); }

void ProgressRing::paintEvent(QPaintEvent *)
{
    const int side = qMin(width(), height());
    const QRectF box(QRectF(0, 0, side, side).adjusted(8, 8, -8, -8)
                         .translated((width()-side)/2.0, (height()-side)/2.0));

    QPainter p(this);
    p.setRenderHint(QPainter::Antialiasing);

    QPen bg(QColor("#E2E8F0"));
    bg.setWidthF(side * 0.06);
    bg.setCapStyle(Qt::RoundCap);
    p.setPen(bg);
    p.drawEllipse(box);

    QColor fg;
    if (m_mode == "threat")        fg = QColor("#E11D48");
    else if (m_mode == "done")     fg = QColor("#10B981");
    else if (m_mode == "scanning") fg = QColor("#2563EB");
    else                           fg = QColor("#CBD5E1");

    QPen pen(fg);
    pen.setWidthF(side * 0.06);
    pen.setCapStyle(Qt::RoundCap);
    p.setPen(pen);

    const int span = int(m_displayed * 360.0 * 16.0);
    p.drawArc(box, 90 * 16, -span);

    QFont f = font();
    f.setPointSizeF(f.pointSizeF() * 1.8);
    f.setBold(true);
    p.setFont(f);
    p.setPen(QColor("#0F172A"));

    const QString text = m_centerText.isEmpty()
                             ? QString::number(int(m_displayed * 100)) + "%" : m_centerText;
    p.drawText(rect(), Qt::AlignCenter, text);
}

} // namespace verax
