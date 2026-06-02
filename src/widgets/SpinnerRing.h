// SpinnerRing.h - indeterminate spinner arc (7°/30ms)
// By Ali Sakkaf - https://alisakkaf.com
#pragma once
#include <QWidget>
class QTimer;

namespace verax {

class SpinnerRing : public QWidget {
    Q_OBJECT
public:
    explicit SpinnerRing(QWidget *parent = nullptr);
    QSize sizeHint() const override            { return QSize(48, 48); }
    QSize minimumSizeHint() const override     { return QSize(24, 24); }

public slots:
    void start();
    void stop();

protected:
    void paintEvent(QPaintEvent *) override;

private:
    int     m_angle = 0;
    QTimer *m_timer = nullptr;
};

} // namespace verax
