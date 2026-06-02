// FileOps.cpp
// By Ali Sakkaf - https://alisakkaf.com
#include "FileOps.h"
#include <QFile>
#include <QDir>
#include <QSaveFile>
#include <QFileInfo>
#include <QStorageInfo>

namespace verax {

bool FileOps::atomicWrite(const QString &path, const QByteArray &data)
{
    QSaveFile sf(path);
    if (!sf.open(QIODevice::WriteOnly)) return false;
    if (sf.write(data) != data.size()) { sf.cancelWriting(); return false; }
    return sf.commit();
}

bool FileOps::safeCopy(const QString &src, const QString &dst)
{
    mkpath(QFileInfo(dst).absolutePath());
    if (QFile::exists(dst)) QFile::remove(dst);
    return QFile::copy(src, dst);
}

bool FileOps::safeMove(const QString &src, const QString &dst)
{
    mkpath(QFileInfo(dst).absolutePath());
    if (QFile::rename(src, dst)) return true;
    if (!safeCopy(src, dst)) return false;
    return QFile::remove(src);
}

bool FileOps::mkpath(const QString &dir) { return QDir().mkpath(dir); }

qint64 FileOps::freeSpaceBytes(const QString &anyPathOnVolume)
{
    QStorageInfo si(QFileInfo(anyPathOnVolume).absolutePath());
    return si.isValid() ? si.bytesAvailable() : 0;
}

QString FileOps::humanSize(qint64 bytes)
{
    static const char *u[] = { "B", "KB", "MB", "GB", "TB" };
    double v = double(bytes);
    int i = 0;
    while (v >= 1024.0 && i < 4) { v /= 1024.0; ++i; }
    return QString::number(v, 'f', i == 0 ? 0 : 1) + " " + u[i];
}

} // namespace verax
