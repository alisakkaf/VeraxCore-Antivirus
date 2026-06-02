// FileOps.h - safe copy/move/atomic write/mkpath/free space
// By Ali Sakkaf - https://alisakkaf.com
#pragma once
#include <QString>
#include <QByteArray>

namespace verax {
class FileOps {
public:
    static bool   atomicWrite(const QString &path, const QByteArray &data);
    static bool   safeCopy   (const QString &src, const QString &dst);
    static bool   safeMove   (const QString &src, const QString &dst);
    static bool   mkpath     (const QString &dir);
    static qint64 freeSpaceBytes(const QString &anyPathOnVolume);
    static QString humanSize (qint64 bytes);
};
} // namespace verax
