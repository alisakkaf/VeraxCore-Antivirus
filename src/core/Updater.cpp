// ═══════════════════════════════════════════════════════════════════════
//  Updater.cpp — implementation.
// ═══════════════════════════════════════════════════════════════════════
#include "Updater.h"
#include "Logger.h"
#include "../../Version.h"

#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QTimer>
#include <QUrl>
#include <QDialog>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QTextBrowser>
#include <QGraphicsOpacityEffect>
#include <QPropertyAnimation>
#include <QEasingCurve>
#include <QDesktopServices>

namespace verax {

Updater& Updater::instance() {
    static Updater u;
    return u;
}

Updater::Updater(QObject *parent) : QObject(parent) {}

// ────────────────────────────────────────────────────────────────────────
//  Version comparison — left-pads with 0 so "1.1" vs "1.1.0.0" compare equal.
// ────────────────────────────────────────────────────────────────────────
int Updater::compareVersions(const QString &a, const QString &b)
{
    const QStringList ax = a.split('.', Qt::SkipEmptyParts);
    const QStringList bx = b.split('.', Qt::SkipEmptyParts);
    const int n = std::max(ax.size(), bx.size());
    for (int i = 0; i < n; ++i) {
        const int ai = (i < ax.size() ? ax[i].toInt() : 0);
        const int bi = (i < bx.size() ? bx[i].toInt() : 0);
        if (ai != bi) return ai - bi;
    }
    return 0;
}

// ────────────────────────────────────────────────────────────────────────
//  Body parser. Contract documented in Updater.h.
// ────────────────────────────────────────────────────────────────────────
UpdateInfo Updater::parseBody(const QString &body)
{
    UpdateInfo info;
    info.downloadUrl = QString::fromLatin1(APP_DOWNLOAD_URL);

    const QString trimmed = body.trimmed();
    if (trimmed.isEmpty()) return info;

    // First non-empty line is the version. Anything after the literal
    // "Changelog=>" marker is the changelog (free-form).
    const int marker = trimmed.indexOf(QLatin1String("Changelog=>"));
    QString verLine;
    QString changelog;
    if (marker >= 0) {
        verLine   = trimmed.left(marker).trimmed();
        changelog = trimmed.mid(marker + int(strlen("Changelog=>"))).trimmed();
    } else {
        // No marker → assume the whole body is the version
        verLine   = trimmed;
    }

    // Pick the FIRST line of the version block in case there's whitespace
    const QStringList vlines = verLine.split(QRegExp("[\\r\\n]"), Qt::SkipEmptyParts);
    if (!vlines.isEmpty()) info.latestVersion = vlines.first().trimmed();
    info.changelog = changelog;

    // Validate: must look like a dotted-decimal version, e.g. "1.2.3" or "1.2.3.4"
    static const QRegExp rxVer("^\\d+(\\.\\d+){0,3}$");
    info.valid = rxVer.exactMatch(info.latestVersion);
    if (info.valid) {
        info.newer = compareVersions(info.latestVersion,
                                     QString::fromLatin1(APP_VERSION_STR)) > 0;
    }
    return info;
}

// ────────────────────────────────────────────────────────────────────────
//  Network check
// ────────────────────────────────────────────────────────────────────────
void Updater::checkSilently(QWidget *uiOwner)
{
    if (m_inFlight) return;
    m_inFlight = true;
    m_owner = uiOwner;

    if (!m_nam) m_nam = new QNetworkAccessManager(this);

    QNetworkRequest req(QUrl(QString::fromLatin1(APP_VERSION_CHECK_URL)));
    req.setHeader(QNetworkRequest::UserAgentHeader,
                  QStringLiteral("%1/%2").arg(APP_NAME, APP_VERSION_STR));
    req.setAttribute(QNetworkRequest::FollowRedirectsAttribute, true);
    req.setRawHeader("Accept", "text/plain");

    QNetworkReply *r = m_nam->get(req);

    auto *to = new QTimer(this);
    to->setSingleShot(true);
    to->setInterval(8000);
    connect(to, &QTimer::timeout, r, &QNetworkReply::abort);
    to->start();

    connect(r, &QNetworkReply::finished, this, [this, r, to]{
        to->stop();
        to->deleteLater();
        m_inFlight = false;

        if (r->error() != QNetworkReply::NoError) {
            const QString err = r->errorString();
            r->deleteLater();
            Logger::warn(QStringLiteral("Updater silent check failed: %1").arg(err));
            emit checkFailed(err);
            return;
        }

        const QString body = QString::fromUtf8(r->readAll());
        r->deleteLater();

        const UpdateInfo info = parseBody(body);
        if (!info.valid) {
            Logger::warn(QStringLiteral("Updater: malformed body (head=%1)")
                         .arg(body.left(64).replace('\n', ' ')));
            emit checkFailed(QStringLiteral("Malformed update payload"));
            return;
        }
        if (!info.newer) {
            Logger::info(QStringLiteral("Updater: already on latest version (%1).")
                         .arg(info.latestVersion));
            emit noUpdate();
            return;
        }

        Logger::info(QStringLiteral("Updater: new version available (%1 → %2)")
                     .arg(QString::fromLatin1(APP_VERSION_STR), info.latestVersion));
        emit updateAvailable(info);

        if (m_owner) showUpdateDialog(m_owner, info);
    });
}

// ────────────────────────────────────────────────────────────────────────
//  Modal dialog — frameless, branded, with a smooth fade-in animation
//  and three actions: Update now / Remind me later / Skip this version.
// ────────────────────────────────────────────────────────────────────────
void Updater::showUpdateDialog(QWidget *parent, const UpdateInfo &info)
{
    auto *dlg = new QDialog(parent);
    dlg->setObjectName("UpdaterDialog");
    dlg->setWindowFlag(Qt::Dialog);
    dlg->setWindowFlag(Qt::FramelessWindowHint);
    dlg->setAttribute(Qt::WA_DeleteOnClose, true);
    dlg->setAttribute(Qt::WA_TranslucentBackground, false);
    dlg->setMinimumSize(520, 360);
    dlg->setWindowTitle(tr("Update available"));

    auto *root = new QVBoxLayout(dlg);
    root->setContentsMargins(28, 24, 28, 24);
    root->setSpacing(14);

    // ── Header row: badge + title + version chip ─────────────────────────
    auto *headerRow = new QHBoxLayout();
    headerRow->setSpacing(12);

    auto *badge = new QLabel(dlg);
    badge->setObjectName("UpdaterBadge");
    badge->setText(QStringLiteral("\u2728")); // sparkles
    badge->setAlignment(Qt::AlignCenter);
    badge->setMinimumSize(48, 48);
    badge->setMaximumSize(48, 48);
    headerRow->addWidget(badge);

    auto *titleCol = new QVBoxLayout();
    titleCol->setSpacing(2);
    auto *title = new QLabel(tr("A new version of %1 is available")
                                 .arg(QString::fromLatin1(APP_NAME)), dlg);
    title->setObjectName("UpdaterTitle");
    title->setWordWrap(true);
    auto *subtitle = new QLabel(dlg);
    subtitle->setObjectName("UpdaterSubtitle");
    subtitle->setText(tr("You have version %1. Version %2 is now available.")
                          .arg(QString::fromLatin1(APP_VERSION_STR), info.latestVersion));
    subtitle->setWordWrap(true);
    titleCol->addWidget(title);
    titleCol->addWidget(subtitle);
    headerRow->addLayout(titleCol, 1);
    root->addLayout(headerRow);

    // ── Version chip row ─────────────────────────────────────────────────
    auto *chipRow = new QHBoxLayout();
    chipRow->setSpacing(8);
    auto *chipNew = new QLabel(QStringLiteral("v%1").arg(info.latestVersion), dlg);
    chipNew->setObjectName("UpdaterChipNew");
    chipNew->setProperty("class", "Chip");
    auto *chipOld = new QLabel(tr("Current v%1").arg(QString::fromLatin1(APP_VERSION_STR)), dlg);
    chipOld->setObjectName("UpdaterChipOld");
    chipOld->setProperty("class", "Chip");
    chipRow->addWidget(chipNew);
    chipRow->addWidget(chipOld);
    chipRow->addStretch(1);
    root->addLayout(chipRow);

    // ── Changelog area ───────────────────────────────────────────────────
    auto *changeTitle = new QLabel(tr("What's new"), dlg);
    changeTitle->setObjectName("UpdaterChangelogTitle");
    root->addWidget(changeTitle);

    auto *changeView = new QTextBrowser(dlg);
    changeView->setObjectName("UpdaterChangelog");
    changeView->setOpenExternalLinks(true);
    const QString changelog = info.changelog.isEmpty()
        ? tr("No changelog provided.")
        : info.changelog;
    changeView->setPlainText(changelog);
    root->addWidget(changeView, 1);

    // ── Buttons row ──────────────────────────────────────────────────────
    auto *btnRow = new QHBoxLayout();
    btnRow->setSpacing(8);
    btnRow->addStretch(1);

    auto *btnLater = new QPushButton(tr("Remind me later"), dlg);
    btnLater->setProperty("kind", "ghost");
    auto *btnSkip = new QPushButton(tr("Skip this version"), dlg);
    btnSkip->setProperty("kind", "ghost");
    auto *btnUpdate = new QPushButton(tr("Update now"), dlg);
    btnUpdate->setProperty("kind", "primary");

    btnRow->addWidget(btnSkip);
    btnRow->addWidget(btnLater);
    btnRow->addWidget(btnUpdate);
    root->addLayout(btnRow);

    connect(btnLater, &QPushButton::clicked, dlg, &QDialog::reject);
    connect(btnSkip,  &QPushButton::clicked, dlg, &QDialog::reject);
    connect(btnUpdate, &QPushButton::clicked, dlg, [dlg, info]{
        QDesktopServices::openUrl(QUrl(info.downloadUrl));
        dlg->accept();
    });

    // ── Smooth fade-in animation ─────────────────────────────────────────
    auto *fx = new QGraphicsOpacityEffect(dlg);
    fx->setOpacity(0.0);
    dlg->setGraphicsEffect(fx);
    auto *anim = new QPropertyAnimation(fx, "opacity", dlg);
    anim->setStartValue(0.0);
    anim->setEndValue(1.0);
    anim->setDuration(220);
    anim->setEasingCurve(QEasingCurve::OutCubic);
    QTimer::singleShot(0, dlg, [anim]{ anim->start(QAbstractAnimation::DeleteWhenStopped); });

    dlg->show();
    dlg->raise();
    dlg->activateWindow();
}

} // namespace verax
