// Settings.cpp - QSettings facade with Windows-startup wiring
// By Ali Sakkaf - https://alisakkaf.com
#include "Settings.h"
#include "Logger.h"
#include "../../Version.h"
#include "SignatureDb.h"

#include <QSettings>
#include <QCoreApplication>
#include <QDir>
namespace verax {

Settings& Settings::instance() {
    static Settings s;
    return s;
}

Settings::Settings(QObject *parent) : QObject(parent) {
    m_updateUrl = QString::fromLatin1(APP_UPDATE_URL);
}

static QString regPath() {
    return QStringLiteral("HKEY_CURRENT_USER\\") + QString::fromLatin1(APP_REG_KEY);
}

void Settings::load()
{
    QSettings s(QSettings::NativeFormat, QSettings::UserScope,
                QString::fromLatin1(APP_VENDOR),
                QString::fromLatin1(APP_NAME));

    m_language          = s.value("general/language",        m_language).toString();
    m_startWithWindows  = s.value("general/startWithWindows",m_startWithWindows).toBool();
    m_trayOnClose       = s.value("general/trayOnClose",     m_trayOnClose).toBool();
    m_showNotifications = s.value("general/showNotifications", m_showNotifications).toBool();

    m_scanUsbOnInsert   = s.value("scan/usbOnInsert",        m_scanUsbOnInsert).toBool();
    m_scheduledScan     = s.value("scan/scheduled",          m_scheduledScan).toString();
    m_scheduledTime     = s.value("scan/scheduledTime",      m_scheduledTime).toString();

    m_autoUpdate        = s.value("updates/auto",            m_autoUpdate).toBool();
    m_updateUrl         = s.value("updates/url",             m_updateUrl).toString();

    m_useSigDb          = s.value("engine/sigDb",            m_useSigDb).toBool();
    m_usePe             = s.value("engine/pe",               m_usePe).toBool();
    m_useHeur           = s.value("engine/heuristics",       m_useHeur).toBool();
    m_useCloud          = s.value("engine/cloud",            m_useCloud).toBool();

    m_heurThreshold     = s.value("engine/heurThreshold",    m_heurThreshold).toInt();
    m_detectionAction   = s.value("engine/action",           m_detectionAction).toString();
}

void Settings::save()
{
    QSettings s(QSettings::NativeFormat, QSettings::UserScope,
                QString::fromLatin1(APP_VENDOR),
                QString::fromLatin1(APP_NAME));

    s.setValue("general/language",          m_language);
    s.setValue("general/startWithWindows",  m_startWithWindows);
    s.setValue("general/trayOnClose",       m_trayOnClose);
    s.setValue("general/showNotifications", m_showNotifications);

    s.setValue("scan/usbOnInsert",          m_scanUsbOnInsert);
    s.setValue("scan/scheduled",            m_scheduledScan);
    s.setValue("scan/scheduledTime",        m_scheduledTime);

    s.setValue("updates/auto",              m_autoUpdate);
    s.setValue("updates/url",               m_updateUrl);

    s.setValue("engine/sigDb",              m_useSigDb);
    s.setValue("engine/pe",                 m_usePe);
    s.setValue("engine/heuristics",         m_useHeur);
    s.setValue("engine/cloud",              m_useCloud);
    s.setValue("engine/heurThreshold",      m_heurThreshold);
    s.setValue("engine/action",             m_detectionAction);
    s.sync();
}

void Settings::setLanguage(const QString &v) {
    if (m_language == v) return;
    m_language = v;
    save();
    emit languageChanged(v);
}

void Settings::setStartWithWindows(bool v) {
    m_startWithWindows = v;
    save();
    applyStartupRegistry();
}

void Settings::applyStartupRegistry()
{
    QSettings run("HKEY_CURRENT_USER\\Software\\Microsoft\\Windows"
                  "\\CurrentVersion\\Run", QSettings::NativeFormat);
    const QString exe = QDir::toNativeSeparators(
                QCoreApplication::applicationFilePath());
    if (m_startWithWindows) {
        run.setValue(QString::fromLatin1(APP_NAME),
                     QStringLiteral("\"%1\" --tray").arg(exe));
        Logger::info(QStringLiteral("Startup registered: %1").arg(exe));
    } else {
        run.remove(QString::fromLatin1(APP_NAME));
        Logger::info(QStringLiteral("Startup deregistered"));
    }
}

void Settings::resetAll()
{
    // 1. Reset standard configuration settings
    QSettings s(QSettings::NativeFormat, QSettings::UserScope,
                QString::fromLatin1(APP_VENDOR),
                QString::fromLatin1(APP_NAME));
    s.clear();
    s.sync();

    // 2. Clear registry startup
    m_startWithWindows = false;
    applyStartupRegistry();

    // 3. Close the active database connection
    verax::SignatureDb::instance().close();

    // 4. Physical deletion of state files
    const QString appData = Logger::userDataDir();
    QFile::remove(appData + QStringLiteral("/db/verax.sqlite"));
    QFile::remove(appData + QStringLiteral("/db/verax.sqlite-shm"));
    QFile::remove(appData + QStringLiteral("/db/verax.sqlite-wal"));
    QFile::remove(appData + QStringLiteral("/db/verax_signatures.json"));

    // Remove logs
    QDir logDir(appData + QStringLiteral("/Logs"));
    if (logDir.exists()) {
        logDir.removeRecursively();
    }

    // 5. Reset member variables to defaults
    m_language          = QStringLiteral("en");
    m_trayOnClose       = true;
    m_showNotifications = true;
    m_scanUsbOnInsert   = true;
    m_scheduledScan     = QStringLiteral("off");
    m_scheduledTime     = QStringLiteral("03:00");
    m_autoUpdate        = true;
    m_updateUrl         = QString::fromLatin1(APP_UPDATE_URL);
    m_useSigDb = m_usePe = m_useHeur = true;
    m_useCloud          = false;
    m_heurThreshold     = 60;
    m_detectionAction   = QStringLiteral("report");

    // 6. Reinitialize the fresh database from seed
    verax::SignatureDb::instance().initSchema();

    emit languageChanged(m_language);
}

} // namespace verax
