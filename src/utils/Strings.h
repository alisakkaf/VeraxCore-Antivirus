// Strings.h - inline helpers + obfuscated URL constants (mild)
// By Ali Sakkaf - https://alisakkaf.com
#pragma once
#include <QString>
#include <QObject>

namespace verax {

// Helper for tr() outside QObject contexts
inline QString T(const char *key) { return QObject::tr(key); }

// Obfuscated URL builder - XOR with a constant to avoid trivial string-scan.
// Only used for slightly sensitive URLs; nothing protects from real reversers.
inline QString deobf(const char *xored, int len, char key = 0x5A) {
    QByteArray buf;
    buf.resize(len);
    for (int i = 0; i < len; ++i) buf[i] = char(xored[i] ^ key);
    return QString::fromUtf8(buf);
}

} // namespace verax
