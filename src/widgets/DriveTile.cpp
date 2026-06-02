// DriveTile.cpp
#include "DriveTile.h"
#include "../utils/FileOps.h"

#include <QLabel>
#include <QCheckBox>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QMouseEvent>
#include <QCoreApplication>

#ifdef _WIN32
#  include <windows.h>
#endif

namespace verax {

static QString iconCharForType(int t)
{
#ifdef _WIN32
    switch (t) {
    case DRIVE_FIXED:     return QStringLiteral("\u26C1");
    case DRIVE_REMOVABLE: return QStringLiteral("\u26C3");
    case DRIVE_REMOTE:    return QStringLiteral("\u29C9");
    case DRIVE_CDROM:     return QStringLiteral("\u25C9");
    default:              return QStringLiteral("\u25A1");
    }
#else
    Q_UNUSED(t); return QStringLiteral("\u25A1");
#endif
}

DriveTile::DriveTile(const DriveInfo &d, QWidget *parent)
    : SurfaceCard(parent), m_info(d)
{
    setObjectName("DriveTile");
    setProperty("class", "DriveTile");
    setInteractive(true);

    auto *outer = new QHBoxLayout(this);

    int pad = fontMetrics().averageCharWidth();
    if (pad <= 0) pad = 8;

    outer->setContentsMargins(pad * 2, pad * 2, pad * 2, pad * 2);
    outer->setSpacing(pad * 2);

    m_lblIcon = new QLabel(iconCharForType(d.typeCode), this);
    m_lblIcon->setObjectName("DriveTileIcon");
    QFont iconFont = m_lblIcon->font();

    if (iconFont.pointSizeF() > 0) {
        iconFont.setPointSizeF(iconFont.pointSizeF() * 2.4);
    } else if (iconFont.pixelSize() > 0) {
        iconFont.setPixelSize(int(iconFont.pixelSize() * 2.4));
    } else if (iconFont.pointSize() > 0) {
        iconFont.setPointSize(int(iconFont.pointSize() * 2.4));
    } else {
        iconFont.setPointSize(24);
    }

    m_lblIcon->setFont(iconFont);
    m_lblIcon->setAlignment(Qt::AlignCenter);

    auto *col = new QVBoxLayout();
    col->setSpacing(0);

    QString letterText = d.letter;
    if (!d.label.isEmpty()) letterText += QStringLiteral("  ") + d.label;
    m_lblLetter = new QLabel(letterText, this);
    m_lblLetter->setObjectName("DriveTileLetter");
    QFont nameFont = m_lblLetter->font();
    nameFont.setBold(true);
    m_lblLetter->setFont(nameFont);

    const QByteArray typeKey = d.typeName.toUtf8();
    const QString typeLocalized = QCoreApplication::translate("DriveType", typeKey.constData());
    m_lblType = new QLabel(typeLocalized +
                               (d.fileSystem.isEmpty() ? QString()
                                                       : QStringLiteral("  ·  ") + d.fileSystem), this);
    m_lblType->setObjectName("DriveTileType");

    QString cap;
    if (d.totalBytes > 0) {
        cap = tr("%1 free of %2").arg(FileOps::humanSize(d.freeBytes), FileOps::humanSize(d.totalBytes));
    } else {
        cap = tr("Capacity unknown");
    }
    m_lblCapacity = new QLabel(cap, this);
    m_lblCapacity->setObjectName("DriveTileCapacity");

    col->addWidget(m_lblLetter);
    col->addWidget(m_lblType);
    col->addWidget(m_lblCapacity);

    m_check = new QCheckBox(this);
    m_check->setObjectName("DriveTileCheck");
    connect(m_check, &QCheckBox::toggled, this, &DriveTile::toggled);

    outer->addWidget(m_lblIcon);
    outer->addLayout(col, 1);
    outer->addWidget(m_check);
}

bool DriveTile::isChecked() const   { return m_check->isChecked(); }
void DriveTile::setChecked(bool v)  { m_check->setChecked(v); }

void DriveTile::mousePressEvent(QMouseEvent *e)
{
    if (e->button() == Qt::LeftButton)
        m_check->toggle();
    SurfaceCard::mousePressEvent(e);
}

void DriveTile::retranslate()
{
    const QByteArray typeKey = m_info.typeName.toUtf8();
    const QString typeLocalized = QCoreApplication::translate("DriveType", typeKey.constData());
    m_lblType->setText(typeLocalized +
                       (m_info.fileSystem.isEmpty() ? QString()
                                                    : QStringLiteral("  ·  ") + m_info.fileSystem));
    if (m_info.totalBytes > 0)
        m_lblCapacity->setText(tr("%1 free of %2")
                                   .arg(FileOps::humanSize(m_info.freeBytes),
                                        FileOps::humanSize(m_info.totalBytes)));
    else
        m_lblCapacity->setText(tr("Capacity unknown"));
}

} // namespace verax
