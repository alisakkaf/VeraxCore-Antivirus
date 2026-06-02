// SystemEnum.h - disks + USB + processes + installed-AV enumeration
// By Ali Sakkaf - https://alisakkaf.com
#pragma once
#include <QObject>
#include <QString>
#include <QStringList>
#include <QVector>

namespace verax {

struct DriveInfo {
    QString letter;         // "C:"
    QString label;          // volume label
    QString fileSystem;     // NTFS / FAT32 / exFAT
    QString typeName;       // Fixed / Removable / Network / CD / Unknown
    int     typeCode = 0;   // GetDriveType return
    quint64 totalBytes = 0;
    quint64 freeBytes  = 0;
};

struct ProcInfo {
    quint32 pid = 0;
    QString name;
    QString path;
};

class SystemEnum {
public:
    // Disks / volumes
    static QVector<DriveInfo> listDrives();
    static DriveInfo          driveOf(const QString &letter);

    // Processes (top-level snapshot via psapi)
    static QVector<ProcInfo>  listProcesses();

    // Antivirus product names from SecurityCenter2 WMI
    static QStringList        installedAntivirus();

    // Friendly drive-type translation key (for tr() lookup)
    static QString            driveTypeKey(int code);
};

} // namespace verax
