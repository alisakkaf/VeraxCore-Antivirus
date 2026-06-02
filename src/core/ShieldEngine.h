// ShieldEngine.h - top-level orchestrator (state machine)
// By Ali Sakkaf - https://alisakkaf.com
#pragma once
#include <QObject>
#include "Scanner.h"

namespace verax {

class ShieldEngine : public QObject {
    Q_OBJECT
public:
    enum State { Idle, Scanning, Updating, Repairing, Done, Error };
    Q_ENUM(State)

    static ShieldEngine& instance();

    void startScan(const ScanRequest &req);
    void stopScan();
    void pauseScan(bool paused);
    void updateSignatures();
    State state() const { return m_state; }

    Scanner* scanner() { return m_scanner; }

signals:
    void stateChanged(State s);

private:
    explicit ShieldEngine(QObject *parent = nullptr);
    void setState(State s);

    Scanner *m_scanner = nullptr;
    State    m_state   = Idle;
};

} // namespace verax
