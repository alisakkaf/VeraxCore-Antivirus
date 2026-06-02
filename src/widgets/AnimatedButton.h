// AnimatedButton.h - QPushButton with hover lift + accent shadow
// By Ali Sakkaf - https://alisakkaf.com
#pragma once
#include <QPushButton>
class QGraphicsDropShadowEffect;
class QPropertyAnimation;

namespace verax {

class AnimatedButton : public QPushButton {
    Q_OBJECT
public:
    explicit AnimatedButton(QWidget *parent = nullptr);
    explicit AnimatedButton(const QString &text, QWidget *parent = nullptr);

protected:
    void enterEvent(QEvent *e) override;
    void leaveEvent(QEvent *e) override;

private:
    void init();
    QGraphicsDropShadowEffect *m_shadow = nullptr;
};

} // namespace verax
