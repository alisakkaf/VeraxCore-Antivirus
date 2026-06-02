// Toaster.h - non-blocking toast notifications (info/warn/error)
// By Ali Sakkaf - https://alisakkaf.com
#pragma once
#include <QWidget>
class QLabel;
class QPropertyAnimation;

namespace verax {

class Toaster : public QWidget {
    Q_OBJECT
public:
    enum Kind { Info, Warn, Error, Success };
    Q_ENUM(Kind)

    static void show(QWidget *anchor, const QString &text, Kind k = Info,
                     int msec = 3000);

private:
    Toaster(QWidget *anchor, const QString &text, Kind k);
    void slideAndAutoClose(int msec);

    QLabel *m_label = nullptr;
};

} // namespace verax
