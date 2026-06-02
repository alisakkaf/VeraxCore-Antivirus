// Logger.h - Rotating log to UserData/Logs/verax.log
// By Ali Sakkaf - https://alisakkaf.com
#pragma once
#include <QString>
#include <QMutex>
#include <QFile>

namespace verax {

class Logger {
public:
    static void init();
    static void shutdown();

    static void info   (const QString &msg);
    static void warn   (const QString &msg);
    static void error  (const QString &msg);
    static void debug  (const QString &msg);

    static QString userDataDir();  // Central data directory (next to exe)
    static QString logDir();
    static QString logFile();

private:
    static void writeLine(const char *level, const QString &msg);
    static void rotateIfNeeded();
    static QFile  s_file;
    static QMutex s_mutex;
    static bool   s_inited;
};

} // namespace verax

