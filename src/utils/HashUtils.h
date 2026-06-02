// HashUtils.h - streamed SHA256 / CRC32
// By Ali Sakkaf - https://alisakkaf.com
#pragma once
#include <QString>
#include <QByteArray>

namespace verax {
class HashUtils {
public:
    static QString sha256Hex(const QString &path);
    static QString sha256Hex(const QByteArray &data);
    static quint32 crc32(const QByteArray &data);
};
} // namespace verax
