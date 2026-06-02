// MainWindow.h - frameless window + sidebar + QStackedWidget pages
// All pages live in the single mainwindow.ui. This class owns the logic.
// By Ali Sakkaf - https://alisakkaf.com
#pragma once

#include <QMainWindow>
#include <QPointer>
#include <QSystemTrayIcon>
#include <QVector>
#include "../core/Scanner.h"
#include "../core/SystemEnum.h"

class QMenu;
class QAction;
class QTimer;
class QComboBox;
class QCheckBox;
class QPushButton;
template <class T> class QFutureWatcher;

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

namespace verax {

class PageTransition;
class ThreatCard;

class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    enum PageIndex {
        PageDashboard = 0,
        PageScanConfig,
        PageScan,
        PageQuarantine,
        PageRepair,
        PageSettings,
        PageAbout,
        PageInstaller
    };
    Q_ENUM(PageIndex)

    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override;

    static bool isInstalledPath();
    void showInstaller();
    void startInTray();
    void runSilentScanAndExit();

protected:
    void changeEvent(QEvent *e) override;
    void closeEvent(QCloseEvent *e) override;

private slots:
    void onNavClicked();
    void onLanguageChanged(const QString &code);

    // Dashboard
    void onQuickScan();
    void onFullScan();
    void onUpdateSignatures();

    // Scan config
    void onAddFolder();
    void onAddFile();
    void onStartScanFromConfig();

    // Scan page
    void onStopScan();
    void onPauseToggled(bool paused);

    // Scanner signals
    void onScannerStarted();
    void onScannerProgress(int pct, qint64 done, qint64 total);
    void onScannerFileScanned(const QString &path);
    void onScannerThreatFound(verax::ThreatInfo info);
    void onScannerFinished(verax::ScanReport report);

    // Quarantine
    void onQuarantineRefresh();
    void onQuarantineRestoreSelected();
    void onQuarantineDeleteSelected();
    void onQuarantineExportReport();

    // Repair
    void onRepairCheck(const QString &card);
    void onRepairFix(const QString &card);
    void onRepairFixAll();
    void onRepairBrowseAppFolder();

    // Settings
    void onSettingsSaved();
    void onSettingsReset();
    void onCheckUpdatesNow();
    void onLanguageSelectionChanged(int idx);

    // About
    void onSelfTest();
    void onBrandClicked(int kind);   // BrandIcon::Kind

    // Installer
    void onInstallNow();
    void onInstallBrowse();
    void onInstallCancel();

    // Tray
    void onTrayActivated(QSystemTrayIcon::ActivationReason r);

    // ThreatList smart filters + bulk actions
    void applyThreatFilters();
    void onBulkAction();
    void onSelectAllThreats(bool checked);

private:
    void wireUi();
    void wireSignals();
    void retranslateRuntime();
    void populateDrivesOnConfig();
    void applyDrivesToUi(const QVector<DriveInfo> &drives);
    void populateQuarantineTable();
    void populateRepairCards();
    void populateAboutPage();
    void setActiveNav(PageIndex idx);
    void applyDaylightStylesheet();
    void setupTrayIcon();
    void buildThreatFilterToolbar();
    QString signaturesInfoHtml() const;
    void updateChromeStatus(const QString &kind, const QString &text);
    void primeScanUi(const QString &phaseLabel);
    QStringList collectScanTargets() const;
    ScanRequest buildScanRequest() const;

    Ui::MainWindow *ui = nullptr;
    PageTransition *m_transition = nullptr;
    QSystemTrayIcon *m_tray = nullptr;
    QTimer *m_watchdog = nullptr;
    QFutureWatcher<QVector<DriveInfo>> *m_drivesWatcher = nullptr;

    int  m_currentPage = PageDashboard;
    bool m_silentMode  = false;
    bool m_populatingDrives = false;
    qint64 m_lastScanThreats = 0;
    QVector<ThreatInfo> m_pendingReport;

    // ThreatList smart UI
    QList<ThreatCard*> m_threatCards;
    QComboBox   *m_filterSeverity = nullptr;
    QComboBox   *m_filterFamily   = nullptr;
    QComboBox   *m_filterRepairable = nullptr;
    QCheckBox   *m_selectAll      = nullptr;
    QComboBox   *m_bulkActionCombo = nullptr;
    QPushButton *m_bulkApplyBtn   = nullptr;
};

} // namespace verax
