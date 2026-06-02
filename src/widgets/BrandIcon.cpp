#include "BrandIcon.h"
#include <QPainter>
#include <QSvgRenderer>
#include <QMouseEvent>
#include <QDesktopServices>
#include <QUrl>

namespace verax {

static const char *kWebsiteSvg =
    "<svg xmlns='http://www.w3.org/2000/svg' viewBox='0 0 24 24' fill='none' "
    "stroke='black' stroke-width='1.7' stroke-linecap='round' stroke-linejoin='round'>"
    "<circle cx='12' cy='12' r='9'/><line x1='3' y1='12' x2='21' y2='12'/>"
    "<path d='M12 3 a14 14 0 0 1 0 18 a14 14 0 0 1 0 -18'/>"
    "<path d='M3.5 8 h17 M3.5 16 h17'/></svg>";

static const char *kFacebookSvg =
    "<svg xmlns='http://www.w3.org/2000/svg' viewBox='0 0 24 24' fill='black'>"
    "<path d='M13.5 22v-8h2.7l.4-3.3h-3.1V8.6c0-.95.27-1.6 1.65-1.6H17V4.1 "
    " C16.65 4.05 15.7 4 14.6 4 c-2.3 0-3.85 1.4-3.85 3.97V10.7H8V14h2.75v8z'/>"
    "</svg>";

static const char *kGithubSvg =
    "<svg xmlns='http://www.w3.org/2000/svg' viewBox='0 0 24 24' fill='black'>"
    "<path d='M12 2a10 10 0 0 0-3.16 19.49c.5.09.68-.22.68-.48v-1.7"
    "c-2.78.6-3.37-1.34-3.37-1.34c-.46-1.16-1.11-1.47-1.11-1.47"
    "c-.91-.62.07-.61.07-.61c1 .07 1.53 1.03 1.53 1.03"
    "c.89 1.53 2.34 1.09 2.91.84c.09-.65.35-1.09.63-1.34"
    "c-2.22-.25-4.55-1.11-4.55-4.94c0-1.09.39-1.99 1.03-2.69"
    "c-.1-.25-.45-1.27.1-2.65c0 0 .84-.27 2.75 1.02"
    "c.8-.22 1.65-.33 2.5-.33s1.7.11 2.5.33"
    "c1.91-1.29 2.75-1.02 2.75-1.02c.55 1.38.2 2.4.1 2.65"
    "c.64.7 1.03 1.6 1.03 2.69c0 3.84-2.34 4.69-4.57 4.94"
    "c.36.31.68.92.68 1.85v2.74c0 .27.18.58.69.48A10 10 0 0 0 12 2z'/></svg>";

BrandIcon::BrandIcon(Kind k, QWidget *parent) : QWidget(parent), m_kind(k)
{
    setObjectName("BrandIcon");
    setCursor(Qt::PointingHandCursor);
    setAttribute(Qt::WA_Hover, true);

    // لضبط الحجم الافتراضي للأيقونة (يمكنك تغيير الـ 32 للحجم الذي تراه مناسباً)
    setFixedSize(45, 45);

    switch (k) {
    case Website:  m_svg = QByteArray(kWebsiteSvg);  break;
    case Facebook: m_svg = QByteArray(kFacebookSvg); break;
    case GitHub:   m_svg = QByteArray(kGithubSvg);   break;
    }
}

void BrandIcon::setHovered(bool h) { m_hovered = h; update(); }

static QColor brandColor(BrandIcon::Kind k, bool hovered)
{
    if (!hovered) return QColor("#64748B");
    switch (k) {
    case BrandIcon::Website:  return QColor("#008B8B");
    case BrandIcon::Facebook: return QColor("#1877F2");
    case BrandIcon::GitHub:   return QColor("#0F172A");
    }
    return QColor("#0F172A");
}

void BrandIcon::paintEvent(QPaintEvent *)
{
    QPainter p(this);
    p.setRenderHint(QPainter::Antialiasing);

    const QColor color = brandColor(m_kind, m_hovered);

    if (m_hovered) {
        p.setPen(Qt::NoPen);
        QColor halo = color;
        halo.setAlphaF(0.15);
        p.setBrush(halo);
        p.drawEllipse(rect().adjusted(1, 1, -1, -1));
    }

    QByteArray colored = m_svg;
    const QByteArray want = color.name(QColor::HexRgb).toLatin1();
    colored.replace("stroke='black'", QByteArray("stroke='") + want + "'");
    colored.replace("fill='black'",   QByteArray("fill='")   + want + "'");

    QSvgRenderer renderer(colored);

    qreal pad = m_hovered ? 2.0 : 4.0;
    QRectF availableRect = rect().adjusted(pad, pad, -pad, -pad);

    qreal size = qMin(availableRect.width(), availableRect.height());

    QRectF centeredInner(0, 0, size, size);
    centeredInner.moveCenter(rect().center());

    renderer.render(&p, centeredInner);
}

void BrandIcon::enterEvent(QEvent *)       { setHovered(true); }
void BrandIcon::leaveEvent(QEvent *)       { setHovered(false); }
void BrandIcon::mouseReleaseEvent(QMouseEvent *e)
{
    if (e->button() == Qt::LeftButton) emit clicked();
}

} // namespace verax
