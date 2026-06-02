// ═══════════════════════════════════════════════════════════════════════
//  Updater.h — silent on-startup version-check + elegant modal prompt.
//
//  Pulls APP_VERSION_CHECK_URL (a plain-text endpoint) every launch,
//  parses the body with the contract:
//
//      <new_version>\nChangelog=>\n<changelog text...>
//
//  ...and only surfaces a modal dialog when the new version is strictly
//  greater than APP_VERSION_STR. Designed to be quiet by default — no
//  toasts on no-update, no toasts on network errors. Single warn-level
//  log on first failure.
//
//  By Ali Sakkaf — https://alisakkaf.com
// ═══════════════════════════════════════════════════════════════════════
#pragma once
#include <QObject>
#include <QString>
#include <QPointer>

class QNetworkAccessManager;
class QWidget;

namespace verax {

struct UpdateInfo {
    QString  latestVersion;     // e.g. "1.1.0"
    QString  changelog;         // free-form, possibly multi-line
    QString  downloadUrl;       // resolved from APP_DOWNLOAD_URL
    bool     valid = false;     // true only if the body parsed cleanly
    bool     newer = false;     // true if latestVersion > APP_VERSION_STR
};

class Updater : public QObject {
    Q_OBJECT
public:
    static Updater& instance();

    // Kicks off a silent network check. Owner = the main window so the
    // modal dialog (if shown) parents to it. Idempotent — repeated calls
    // are coalesced.
    void checkSilently(QWidget *uiOwner);

    // Compare two dotted version strings ("1.0.0.1", "1.1.0"). Returns:
    //   > 0  if `a` is newer than `b`
    //   < 0  if `a` is older than `b`
    //   = 0  if equal (after trailing-zero normalisation)
    static int compareVersions(const QString &a, const QString &b);

    // Public for unit-testing the parser without hitting the network.
    static UpdateInfo parseBody(const QString &body);

signals:
    void updateAvailable(const UpdateInfo &info);
    void noUpdate();
    void checkFailed(const QString &error);

private:
    explicit Updater(QObject *parent = nullptr);
    void showUpdateDialog(QWidget *parent, const UpdateInfo &info);

    QNetworkAccessManager *m_nam = nullptr;
    QPointer<QWidget>      m_owner;
    bool                   m_inFlight = false;
};

} // namespace verax
