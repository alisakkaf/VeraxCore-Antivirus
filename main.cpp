// ═══════════════════════════════════════════════════════════════════════
//  Verax - main entry point
//  By Ali Sakkaf  -  https://alisakkaf.com
// ═══════════════════════════════════════════════════════════════════════
#include "harden.h"
#include "Version.h"

#include "src/core/Settings.h"
#include "src/core/Translator.h"
#include "src/core/Logger.h"
#include "src/ui/MainWindow.h"

#include <QApplication>
#include <QCommandLineParser>
#include <QCommandLineOption>
#include <QFile>
#include <QFontDatabase>
#include <QDir>

int main(int argc, char *argv[])
{
    // 1) HARDEN FIRST - before any other init. Kills DLL planting.
    shield::harden();

    // 2) High-DPI before QApplication for crisp rendering
    QApplication::setAttribute(Qt::AA_EnableHighDpiScaling, true);
    QApplication::setAttribute(Qt::AA_UseHighDpiPixmaps, true);

    QApplication a(argc, argv);

    qRegisterMetaType<verax::DriveInfo>("verax::DriveInfo");
    qRegisterMetaType<QVector<verax::DriveInfo>>("QVector<verax::DriveInfo>");
    qRegisterMetaType<verax::ThreatInfo>("verax::ThreatInfo");
    qRegisterMetaType<verax::ScanReport>("verax::ScanReport");
    qRegisterMetaType<verax::ScanRequest>("verax::ScanRequest");
    qRegisterMetaType<QVector<verax::DriveInfo>>("QVector<verax::DriveInfo>");
    qRegisterMetaType<QVector<verax::ProcInfo>>("QVector<verax::ProcInfo>");
    qRegisterMetaType<verax::ScanReport>("ScanReport");
    qRegisterMetaType<verax::DriveInfo>("DriveInfo");
    qRegisterMetaType<QVector<verax::DriveInfo>>("DriveInfo>");
    qRegisterMetaType<verax::ThreatInfo>("ThreatInfo");
    qRegisterMetaType<verax::ScanReport>("ScanReport");
    qRegisterMetaType<verax::ScanRequest>("ScanRequest");
    qRegisterMetaType<QVector<verax::DriveInfo>>("QVector<DriveInfo>");
    qRegisterMetaType<QVector<verax::ProcInfo>>("QVector<ProcInfo>");


    QApplication::setOrganizationName(APP_VENDOR);
    QApplication::setOrganizationDomain("alisakkaf.com");
    QApplication::setApplicationName(APP_NAME);
    QApplication::setApplicationVersion(APP_VERSION_STR);
    QApplication::setQuitOnLastWindowClosed(false); // tray-aware

    // 3) Logger first so all subsequent failures are recorded
    verax::Logger::init();
    verax::Logger::info(QStringLiteral("=== %1 v%2 starting ===")
                        .arg(APP_NAME).arg(APP_VERSION_STR));

    // 4) Load fonts (best-effort - falls back to system fonts)
    const QStringList fontPaths = {
        QStringLiteral(":/fonts/Inter-Regular.ttf"),
        QStringLiteral(":/fonts/Inter-SemiBold.ttf"),
        QStringLiteral(":/fonts/Inter-Bold.ttf"),
        QStringLiteral(":/fonts/NotoNaskhArabic-Bold.ttf"),
    };
    for (const QString &p : fontPaths) {
        if (QFile::exists(p))
            QFontDatabase::addApplicationFont(p);
    }

    // 5) Settings + Translator (must come before any UI)
    verax::Settings::instance().load();
    verax::Translator::instance().install(
        verax::Settings::instance().language());

    // 6) Compose stylesheet with Version.h token substitution
    QFile qss(QStringLiteral(":/styles/Daylight.qss"));
    if (qss.open(QIODevice::ReadOnly)) {
        QString css = QString::fromUtf8(qss.readAll());
        css.replace(QStringLiteral("%APP_NAME%"),
                    QString::fromLatin1(APP_NAME));
        css.replace(QStringLiteral("%APP_VERSION%"),
                    QString::fromLatin1(APP_VERSION_STR));
        a.setStyleSheet(css);
    }

    // 7) CLI parser - silent scan, tray-only mode
    QCommandLineParser parser;
    parser.setApplicationDescription(QString::fromLatin1(APP_DESCRIPTION));
    parser.addHelpOption();
    parser.addVersionOption();
    QCommandLineOption optScan(QStringList{"s","scan"},
        QObject::tr("Run silent scan and exit"));
    QCommandLineOption optTray(QStringList{"t","tray"},
        QObject::tr("Start minimized to system tray"));
    parser.addOption(optScan);
    parser.addOption(optTray);
    parser.process(a);

    // 8) Build main window
    verax::Logger::info("main: about to construct MainWindow");
    verax::MainWindow w;
    verax::Logger::info("main: MainWindow constructed");
    const bool installed = verax::MainWindow::isInstalledPath();
    verax::Logger::info(QStringLiteral("main: installed=%1").arg(installed ? "yes" : "no"));

    if (parser.isSet(optScan))      w.runSilentScanAndExit();
    else if (parser.isSet(optTray)) w.startInTray();
    else if (!installed)            w.showInstaller();
    else                            w.show();
    verax::Logger::info("main: window shown, entering exec()");

    return a.exec();
}
