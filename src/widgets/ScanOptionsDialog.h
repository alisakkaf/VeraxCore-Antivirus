// ScanOptionsDialog.h — pre-scan customization popup for Quick / Full / Custom
// By Ali Sakkaf - https://alisakkaf.com
//
// Shown right when the user clicks Quick Scan or Full Scan so they can
// confirm scope, pick the action (quarantine / delete / report / repair),
// toggle individual engines, and add/remove extra paths before launching.
// Title and body strings are translated via tr() so the verax_ar / verax_en
// catalogs catch them.
#pragma once

#include <QDialog>
#include "../core/Scanner.h"

class QListWidget;
class QCheckBox;
class QComboBox;
class QSpinBox;
class QLabel;

namespace verax {

class ScanOptionsDialog : public QDialog {
    Q_OBJECT
public:
    enum Mode { Quick, Full, Custom };

    explicit ScanOptionsDialog(Mode mode,
                               const ScanRequest &initial,
                               QWidget *parent = nullptr);

    // The final ScanRequest the user has confirmed. Only valid when exec()
    // returned QDialog::Accepted.
    ScanRequest result() const { return m_result; }

private slots:
    void onAddPath();
    void onAddFile();
    void onRemoveSelected();
    void onAccept();

private:
    void buildUi();
    void seedFromInitial();
    void retranslate();

    Mode         m_mode;
    ScanRequest  m_initial;
    ScanRequest  m_result;

    QLabel      *m_title       = nullptr;
    QLabel      *m_subtitle    = nullptr;
    QListWidget *m_pathsList   = nullptr;

    QCheckBox   *m_engSig      = nullptr;
    QCheckBox   *m_engPe       = nullptr;
    QCheckBox   *m_engHeur     = nullptr;
    QCheckBox   *m_engCloud    = nullptr;

    QCheckBox   *m_extPe       = nullptr;
    QCheckBox   *m_extScripts  = nullptr;
    QCheckBox   *m_extDocs     = nullptr;
    QCheckBox   *m_extArchives = nullptr;
    QCheckBox   *m_extAll      = nullptr;

    QComboBox   *m_action      = nullptr;
    QSpinBox    *m_threshold   = nullptr;
};

} // namespace verax
