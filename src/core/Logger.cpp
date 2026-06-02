// Logger.cpp - rotating file logger + qInstallMessageHandler bridge
// By Ali Sakkaf - https://alisakkaf.com
#include "Logger.h"
#include "../../Version.h"

#include <QDateTime>
#include <QDir>
#include <QCoreApplication>
#include <QTextStream>
#include <QDebug>
#include <QMessageLogContext>

namespace verax {

QFile  Logger::s_file;
QMutex Logger::s_mutex;
bool   Logger::s_inited = false;

static constexpr qint64 kMaxSize = 5LL * 1024 * 1024; // 5 MB
static constexpr int    kKeep    = 5;

static void qtMsgHandler(QtMsgType type, const QMessageLogContext &,
                         const QString &msg)
{
    switch (type) {
    case QtDebugMsg:    Logger::debug(msg); break;
    case QtInfoMsg:     Logger::info (msg); break;
    case QtWarningMsg:  Logger::warn (msg); break;
    case QtCriticalMsg:
    case QtFatalMsg:    Logger::error(msg); break;
    }
}

QString Logger::userDataDir() {
    const QString dir = QCoreApplication::applicationDirPath() + QStringLiteral("/UserData");
    QDir().mkpath(dir);
    return dir;
}

QString Logger::logDir() {
    const QString dir = userDataDir() + QStringLiteral("/") + APP_LOG_SUBDIR;
    QDir().mkpath(dir);
    return dir;
}

QString Logger::logFile() {
    return logDir() + QStringLiteral("/verax.log");
}

void Logger::init()
{
    QMutexLocker lock(&s_mutex);
    if (s_inited) return;

    rotateIfNeeded();
    s_file.setFileName(logFile());
    s_file.open(QIODevice::WriteOnly | QIODevice::Append | QIODevice::Text);

    qInstallMessageHandler(qtMsgHandler);

    s_inited = true;

    QTextStream ts(&s_file);
    ts.setCodec("UTF-8");
    ts << "\n=== " << APP_NAME << " v" << APP_VERSION_STR
       << " - " << QDateTime::currentDateTime().toString(Qt::ISODate) << " ===\n";
    s_file.flush();
}

void Logger::shutdown()
{
    QMutexLocker lock(&s_mutex);
    if (s_file.isOpen()) s_file.close();
    s_inited = false;
}

void Logger::rotateIfNeeded()
{
    QFileInfo fi(logFile());
    if (!fi.exists() || fi.size() < kMaxSize) return;

    for (int i = kKeep - 1; i >= 1; --i) {
        const QString a = logFile() + QStringLiteral(".%1").arg(i);
        const QString b = logFile() + QStringLiteral(".%1").arg(i + 1);
        if (QFile::exists(b)) QFile::remove(b);
        if (QFile::exists(a)) QFile::rename(a, b);
    }
    QFile::rename(logFile(), logFile() + QStringLiteral(".1"));
}

void Logger::writeLine(const char *level, const QString &msg)
{
    QMutexLocker lock(&s_mutex);
    if (!s_inited || !s_file.isOpen()) return;

    if (s_file.size() >= kMaxSize) {
        s_file.close();
        rotateIfNeeded();
        s_file.open(QIODevice::WriteOnly | QIODevice::Append | QIODevice::Text);
    }

    QTextStream ts(&s_file);
    ts.setCodec("UTF-8");
    ts << QDateTime::currentDateTime().toString(Qt::ISODate)
       << " [" << level << "] " << msg << '\n';
    s_file.flush();
}

void Logger::info (const QString &m) { writeLine("INFO",  m); }
void Logger::warn (const QString &m) { writeLine("WARN",  m); }
void Logger::error(const QString &m) { writeLine("ERROR", m); }
void Logger::debug(const QString &m) { writeLine("DEBUG", m); }

} // namespace verax
