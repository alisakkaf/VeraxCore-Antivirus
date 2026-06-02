// ThreatCard.cpp
// By Ali Sakkaf - https://alisakkaf.com
#include "ThreatCard.h"
#include "../utils/FileOps.h"

#include <QLabel>
#include <QPushButton>
#include <QCheckBox>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QPropertyAnimation>
#include <QGraphicsOpacityEffect>
#include <QFileInfo>
#include <QStyle>
#include <QSet>

namespace verax {

static QLabel *makeChip(QWidget *parent, const QString &text,
                        const QString &kind, const QString &objectName)
{
    auto *l = new QLabel(text, parent);
    l->setObjectName(objectName);
    l->setProperty("class", "Chip");
    l->setProperty("kind", kind);
    l->setAlignment(Qt::AlignCenter);
    return l;
}

ThreatCard::ThreatCard(const ThreatInfo &info, QWidget *parent)
    : SurfaceCard(parent), m_info(info)
{
    setObjectName("ThreatCard");
    setProperty("class", "ThreatCard");

    auto *outer = new QVBoxLayout(this);
    const int pad = fontMetrics().averageCharWidth();
    outer->setContentsMargins(pad * 2, pad * 2, pad * 2, pad * 2);
    outer->setSpacing(pad);

    // ── Header row: checkbox + detection name + family + severity + score ──
    auto *headerRow = new QHBoxLayout();
    headerRow->setSpacing(pad);

    // Multi-select checkbox
    m_chkSelect = new QCheckBox(this);
    m_chkSelect->setObjectName("ThreatCardSelect");
    m_chkSelect->setToolTip(tr("Select for bulk action"));
    connect(m_chkSelect, &QCheckBox::toggled, this, &ThreatCard::selectionChanged);

    m_lblName = new QLabel(this);
    m_lblName->setObjectName("ThreatCardName");
    m_lblName->setText(info.detectionName.isEmpty()
                       ? tr("Suspicious file") : info.detectionName);
    QFont nameFont = m_lblName->font();
    nameFont.setBold(true);
    m_lblName->setFont(nameFont);

    m_lblFamily = makeChip(this,
                           info.family.isEmpty() ? QStringLiteral("Generic") : info.family,
                           "family", "ThreatFamily");

    m_lblSeverity = makeChip(this, QString(), "low", "ThreatSeverity");
    if (info.severity >= 8 || info.score >= 100) {
        m_lblSeverity->setText(tr("High"));
        m_lblSeverity->setProperty("kind", "high");
    } else if (info.severity >= 5 || info.score >= 60) {
        m_lblSeverity->setText(tr("Medium"));
        m_lblSeverity->setProperty("kind", "medium");
    } else {
        m_lblSeverity->setText(tr("Low"));
        m_lblSeverity->setProperty("kind", "low");
    }

    m_lblScore = makeChip(this,
                          tr("Score: %1").arg(info.score),
                          "score", "ThreatScore");

    m_lblActionTag = makeChip(this, QString(), "action", "ThreatActionTag");
    m_lblActionTag->setVisible(false);

    headerRow->addWidget(m_chkSelect);
    headerRow->addWidget(m_lblName);
    headerRow->addWidget(m_lblFamily);
    headerRow->addStretch(1);
    headerRow->addWidget(m_lblScore);
    headerRow->addWidget(m_lblSeverity);
    headerRow->addWidget(m_lblActionTag);

    // ── Path ──
    m_lblPath = new QLabel(this);
    m_lblPath->setObjectName("ThreatCardPath");
    m_lblPath->setWordWrap(true);
    m_lblPath->setTextInteractionFlags(Qt::TextSelectableByMouse);
    m_lblPath->setToolTip(info.path);
    m_lblPath->setText(QStringLiteral("<b>%1:</b> %2")
                           .arg(tr("Path"), info.path.toHtmlEscaped()));

    // ── Reason ──
    m_lblReason = new QLabel(this);
    m_lblReason->setObjectName("ThreatCardReason");
    m_lblReason->setWordWrap(true);
    m_lblReason->setText(QStringLiteral("<b>%1:</b> %2")
        .arg(tr("Reason"),
             (info.reason.isEmpty() ? tr("Unknown") : info.reason).toHtmlEscaped()));

    // ── Hash + size ──
    auto *metaRow = new QHBoxLayout();
    metaRow->setSpacing(pad);

    const QString hashShort = info.sha256.left(16);
    m_lblHash = new QLabel(this);
    m_lblHash->setObjectName("ThreatCardHash");
    m_lblHash->setTextInteractionFlags(Qt::TextSelectableByMouse);
    m_lblHash->setToolTip(info.sha256.isEmpty() ? tr("Hash unavailable") : info.sha256);
    m_lblHash->setText(QStringLiteral("<b>%1:</b> %2%3")
                           .arg(tr("SHA-256"),
                                hashShort.isEmpty() ? tr("(unavailable)") : hashShort,
                                hashShort.isEmpty() ? QString() : QStringLiteral("...")));

    m_lblSize = new QLabel(this);
    m_lblSize->setObjectName("ThreatCardSize");
    m_lblSize->setText(QStringLiteral("<b>%1:</b> %2")
                           .arg(tr("Size"), FileOps::humanSize(info.size)));

    metaRow->addWidget(m_lblHash);
    metaRow->addWidget(m_lblSize);
    metaRow->addStretch(1);

    // ── Action buttons ──
    auto *btnRow = new QHBoxLayout();
    btnRow->setSpacing(pad);

    m_btnFolder = new QPushButton(tr("Open folder"), this);
    m_btnFolder->setProperty("kind", "ghost");
    m_btnFolder->setToolTip(tr("Open the folder that contains this file"));

    m_btnIgnore = new QPushButton(tr("Ignore"), this);
    m_btnIgnore->setProperty("kind", "ghost");

    // ── Clean Threat / Repair button ──
    m_btnRepair = new QPushButton(this);
    m_btnRepair->setProperty("kind", "primary");

    static const QSet<QString> infectorFamilies = {
        "floxif", "sality", "virut", "ramnit", "parite", "expiro",
        "polip", "mabezat", "tenga", "lamer", "jeefo", "hidrag",
        "mydoom", "bagle", "neshta", "viking", "alman", "induc",
        "vetor", "expanded", "patcher",
        "mikcer", "rmnet", "nimnul", "xpaj", "patched"
    };
    const QString fam = info.family.toLower();
    const QString reasonLower = info.reason.toLower();
    m_isRepairable =
        info.repairable ||
        infectorFamilies.contains(fam) ||
        info.detectionName.startsWith(QLatin1String("Win32.Infector")) ||
        info.detectionName.startsWith(QLatin1String("PE.Infector")) ||
        info.detectionName.startsWith(QLatin1String("Virus:Win32/Floxif")) ||
        info.detectionName.startsWith(QLatin1String("Virus:Win32/Mikcer")) ||
        info.detectionName.startsWith(QLatin1String("Virus:Win32/Sality")) ||
        info.detectionName.startsWith(QLatin1String("Virus:Win32/Ramnit")) ||
        info.detectionName.startsWith(QLatin1String("Virus:Win32/Virut")) ||
        info.detectionName.startsWith(QLatin1String("Virus:Win32/Neshta")) ||
        info.detectionName.startsWith(QLatin1String("Virus:Win32/Parite")) ||
        info.detectionName.startsWith(QLatin1String("Virus:Win32/Expiro")) ||
        info.detectionName.startsWith(QLatin1String("Virus:Win32/Nimnul")) ||
        info.detectionName.startsWith(QLatin1String("Virus:Win32/Xpaj")) ||
        info.detectionName.startsWith(QLatin1String("Virus:Win32/Rmnet")) ||
        reasonLower.contains(QLatin1String("malicious file-infector segment")) ||
        reasonLower.contains(QLatin1String("file-infector")) ||
        reasonLower.contains(QLatin1String("floxif")) ||
        reasonLower.contains(QLatin1String("mikcer")) ||
        reasonLower.contains(QLatin1String("ep hooks")) ||
        reasonLower.contains(QLatin1String("ep jmp")) ||
        reasonLower.contains(QLatin1String("push+ret redirect"));

    if (m_isRepairable) {
        m_btnRepair->setText(tr("Clean Threat"));
        m_btnRepair->setToolTip(tr("Remove the virus from this file and restore it to working state.\n"
                                   "The file will be cleaned in-place without creating a backup."));
    } else {
        m_btnRepair->setText(tr("Repair"));
        m_btnRepair->setProperty("kind", "secondary");
        m_btnRepair->setToolTip(tr("Try to disinfect the file in place "
                                   "(strips infector sections from PE binaries)"));
    }
    m_btnRepair->setVisible(m_isRepairable);

    m_btnDel  = new QPushButton(tr("Delete permanently"), this);
    m_btnDel->setProperty("kind", "danger");

    m_btnQuar = new QPushButton(tr("Quarantine"), this);
    m_btnQuar->setProperty("kind", m_isRepairable ? "secondary" : "primary");

    btnRow->addWidget(m_btnFolder);
    btnRow->addStretch(1);
    btnRow->addWidget(m_btnIgnore);
    if (m_isRepairable) btnRow->addWidget(m_btnRepair);
    btnRow->addWidget(m_btnDel);
    btnRow->addWidget(m_btnQuar);

    outer->addLayout(headerRow);
    outer->addWidget(m_lblPath);
    outer->addWidget(m_lblReason);
    outer->addLayout(metaRow);
    outer->addLayout(btnRow);

    connect(m_btnQuar,   &QPushButton::clicked, this,
            [this]{ emit quarantineRequested(m_info); });
    connect(m_btnDel,    &QPushButton::clicked, this,
            [this]{ emit deleteRequested(m_info); });
    connect(m_btnRepair, &QPushButton::clicked, this,
            [this]{ emit repairRequested(m_info); });
    connect(m_btnFolder, &QPushButton::clicked, this,
            [this]{ emit openFolderRequested(m_info); });
    connect(m_btnIgnore, &QPushButton::clicked, this,
            [this]{ emit ignoreRequested(m_info); });

    // Reveal animation
    auto *fx = new QGraphicsOpacityEffect(this);
    fx->setOpacity(0.0);
    setGraphicsEffect(fx);
    auto *anim = new QPropertyAnimation(fx, "opacity", this);
    anim->setDuration(220);
    anim->setStartValue(0.0);
    anim->setEndValue(1.0);
    anim->setEasingCurve(QEasingCurve::OutBack);
    anim->start(QAbstractAnimation::DeleteWhenStopped);
}

void ThreatCard::setActioned(const QString &actionLabel)
{
    if (!m_lblActionTag) return;
    m_lblActionTag->setText(actionLabel);
    m_lblActionTag->setVisible(!actionLabel.isEmpty());
    if (m_btnQuar)   m_btnQuar->setEnabled(false);
    if (m_btnDel)    m_btnDel->setEnabled(false);
    if (m_btnRepair) m_btnRepair->setEnabled(false);
    style()->unpolish(m_lblActionTag);
    style()->polish(m_lblActionTag);
}


void ThreatCard::retranslate()
{
    m_lblName->setText(m_info.detectionName.isEmpty()
                       ? tr("Suspicious file") : m_info.detectionName);
    m_lblPath->setText(QStringLiteral("<b>%1:</b> %2")
                           .arg(tr("Path"), m_info.path.toHtmlEscaped()));
    m_lblReason->setText(QStringLiteral("<b>%1:</b> %2")
        .arg(tr("Reason"),
             (m_info.reason.isEmpty() ? tr("Unknown") : m_info.reason).toHtmlEscaped()));
    const QString hashShort = m_info.sha256.left(16);
    m_lblHash->setText(QStringLiteral("<b>%1:</b> %2%3")
                           .arg(tr("SHA-256"),
                                hashShort.isEmpty() ? tr("(unavailable)") : hashShort,
                                hashShort.isEmpty() ? QString() : QStringLiteral("...")));
    m_lblSize->setText(QStringLiteral("<b>%1:</b> %2")
                           .arg(tr("Size"), FileOps::humanSize(m_info.size)));
    m_lblScore->setText(tr("Score: %1").arg(m_info.score));
    m_btnFolder->setText(tr("Open folder"));
    m_btnIgnore->setText(tr("Ignore"));
    if (m_isRepairable)
        m_btnRepair->setText(tr("Clean Threat"));
    else
        m_btnRepair->setText(tr("Repair"));
    m_btnDel->setText(tr("Delete permanently"));
    m_btnQuar->setText(tr("Quarantine"));
}

} // namespace verax
