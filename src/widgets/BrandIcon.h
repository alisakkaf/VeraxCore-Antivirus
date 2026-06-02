// BrandIcon.h - inline SVG social icons (3 author brand links)
// By Ali Sakkaf - https://alisakkaf.com
#pragma once
#include <QWidget>

namespace verax {

class BrandIcon : public QWidget {
    Q_OBJECT
public:
    enum Kind { Website, Facebook, GitHub };
    Q_ENUM(Kind)

    explicit BrandIcon(Kind k, QWidget *parent = nullptr);
    void   setHovered(bool h);
    Kind   kind() const { return m_kind; }
    QSize  sizeHint() const override        { return QSize(32, 32); }
    QSize  minimumSizeHint() const override { return QSize(20, 20); }

signals:
    void clicked();

protected:
    void paintEvent(QPaintEvent *) override;
    void enterEvent(QEvent *) override;
    void leaveEvent(QEvent *) override;
    void mouseReleaseEvent(QMouseEvent *) override;

private:
    Kind m_kind = Website;
    bool m_hovered = false;
    QByteArray m_svg;
};

} // namespace verax
