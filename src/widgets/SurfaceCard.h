// SurfaceCard.h - white card with border + soft shadow + radius
// By Ali Sakkaf - https://alisakkaf.com
#pragma once
#include <QFrame>

namespace verax {

class SurfaceCard : public QFrame {
    Q_OBJECT
    Q_PROPERTY(bool interactive READ interactive WRITE setInteractive)
public:
    explicit SurfaceCard(QWidget *parent = nullptr);
    bool interactive() const           { return m_interactive; }
    void setInteractive(bool v);

protected:
    void enterEvent(QEvent *e) override;
    void leaveEvent(QEvent *e) override;

private:
    bool m_interactive = false;
};

} // namespace verax
