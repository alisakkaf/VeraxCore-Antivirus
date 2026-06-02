// Quarantine.cpp - AES-256-CBC vault. Uses Windows CryptoAPI (BCrypt) +
// HWID-derived key from MachineGuid registry value.
// By Ali Sakkaf - https://alisakkaf.com
#include "Quarantine.h"
#include "Logger.h"
#include "SignatureDb.h"
#include "../../Version.h"
#include "qthread.h"

#include <QFile>
#include <QFileInfo>
#include <QDir>
#include <QSqlQuery>
#include <QSqlDatabase>
#include <QSqlError>
#include <QStandardPaths>
#include <QCryptographicHash>
#include <QDateTime>
#include <QSettings>
#include <QRandomGenerator>

#ifdef _WIN32
#  include <windows.h>
#  include <bcrypt.h>
#  ifndef NT_SUCCESS
#    define NT_SUCCESS(s) (((NTSTATUS)(s)) >= 0)
#  endif
#  pragma comment(lib, "bcrypt.lib")
#endif

namespace verax {

Quarantine& Quarantine::instance() {
    static Quarantine q;
    return q;
}

Quarantine::Quarantine(QObject *parent) : QObject(parent) {
    QDir().mkpath(vaultDir());
}

QString Quarantine::vaultDir() const {
    return Logger::userDataDir() + QStringLiteral("/") + APP_VAULT_SUBDIR;
}

QByteArray Quarantine::hwid() const
{
#ifdef _WIN32
    QSettings cr("HKEY_LOCAL_MACHINE\\SOFTWARE\\Microsoft\\Cryptography",
                 QSettings::NativeFormat);
    const QString mg = cr.value("MachineGuid").toString();
    if (!mg.isEmpty()) return mg.toUtf8();
#endif
    // Fallback: a stable per-user salt (weaker - documented in README)
    return QStringLiteral("verax-fallback-hwid").toUtf8();
}

QByteArray Quarantine::deriveKey() const
{
    return QCryptographicHash::hash(hwid() + QByteArrayLiteral("Verax-vault-v1"),
                                    QCryptographicHash::Sha256);
}

#ifdef _WIN32
static bool runAesCbc(bool encrypt, const QByteArray &key,
                      const QByteArray &iv, const QByteArray &in,
                      QByteArray &out)
{
    BCRYPT_ALG_HANDLE alg = nullptr;
    BCRYPT_KEY_HANDLE kh  = nullptr;
    NTSTATUS st = BCryptOpenAlgorithmProvider(&alg, BCRYPT_AES_ALGORITHM, nullptr, 0);
    if (!NT_SUCCESS(st)) return false;

    st = BCryptSetProperty(alg, BCRYPT_CHAINING_MODE,
                           (PUCHAR)BCRYPT_CHAIN_MODE_CBC,
                           sizeof(BCRYPT_CHAIN_MODE_CBC), 0);
    if (!NT_SUCCESS(st)) { BCryptCloseAlgorithmProvider(alg, 0); return false; }

    st = BCryptGenerateSymmetricKey(alg, &kh, nullptr, 0,
                                    (PUCHAR)key.constData(), key.size(), 0);
    if (!NT_SUCCESS(st)) { BCryptCloseAlgorithmProvider(alg, 0); return false; }

    QByteArray ivCopy = iv;
    DWORD got = 0;
    auto crypt = encrypt ? &BCryptEncrypt : &BCryptDecrypt;
    st = crypt(kh, (PUCHAR)in.constData(), in.size(), nullptr,
               (PUCHAR)ivCopy.data(), ivCopy.size(),
               nullptr, 0, &got, BCRYPT_BLOCK_PADDING);
    if (!NT_SUCCESS(st)) {
        BCryptDestroyKey(kh);
        BCryptCloseAlgorithmProvider(alg, 0);
        return false;
    }

    out.resize(int(got));
    ivCopy = iv;
    st = crypt(kh, (PUCHAR)in.constData(), in.size(), nullptr,
               (PUCHAR)ivCopy.data(), ivCopy.size(),
               (PUCHAR)out.data(), got, &got, BCRYPT_BLOCK_PADDING);

    BCryptDestroyKey(kh);
    BCryptCloseAlgorithmProvider(alg, 0);
    return NT_SUCCESS(st);
}
#endif

bool Quarantine::aesCbcEncryptFile(const QString &src, const QString &dst,
                                   const QByteArray &key) const
{
    QFile in(src);
    if (!in.open(QIODevice::ReadOnly)) return false;
    const QByteArray plain = in.readAll();
    in.close();

    QByteArray iv(16, 0);
    for (int i = 0; i < 16; ++i) iv[i] = char(QRandomGenerator::global()->bounded(256));

    QByteArray cipher;
#ifdef _WIN32
    if (!runAesCbc(true, key, iv, plain, cipher)) return false;
#else
    cipher = plain; // fallback (no encryption on non-Windows)
#endif

    QFile out(dst);
    if (!out.open(QIODevice::WriteOnly | QIODevice::Truncate)) return false;
    out.write(iv);
    out.write(cipher);
    out.close();
    return true;
}

bool Quarantine::aesCbcDecryptFile(const QString &src, const QString &dst,
                                   const QByteArray &key) const
{
    QFile in(src);
    if (!in.open(QIODevice::ReadOnly)) return false;
    const QByteArray iv     = in.read(16);
    const QByteArray cipher = in.readAll();
    in.close();
    if (iv.size() != 16) return false;

    QByteArray plain;
#ifdef _WIN32
    if (!runAesCbc(false, key, iv, cipher, plain)) return false;
#else
    plain = cipher;
#endif

    QFile out(dst);
    if (!out.open(QIODevice::WriteOnly | QIODevice::Truncate)) return false;
    out.write(plain);
    out.close();
    return true;
}

bool Quarantine::secureDelete(const QString &path) const
{
    QFile f(path);
    if (!f.exists()) return true;
    if (!f.open(QIODevice::ReadWrite)) return QFile::remove(path);

    const qint64 sz = f.size();
    constexpr int chunk = 65536;
    QByteArray pat0(chunk, '\0');
    QByteArray pat1(chunk, '\xFF');
    QByteArray patR(chunk, '\0');

    auto pass = [&](const QByteArray &p) {
        f.seek(0);
        qint64 rem = sz;
        while (rem > 0) {
            const qint64 w = qMin<qint64>(rem, p.size());
            f.write(p.constData(), w);
            rem -= w;
        }
        f.flush();
    };

    pass(pat0);
    pass(pat1);
    for (int i = 0; i < patR.size(); ++i)
        patR[i] = char(QRandomGenerator::global()->bounded(256));
    pass(patR);
    f.close();
    return QFile::remove(path);
}

// QString Quarantine::moveToVault(const QString &originalPath,
//                                 const QString &sha256,
//                                 const QString &detectionName)
// {
//     QFileInfo fi(originalPath);
//     if (!fi.exists() || !fi.isFile()) return {};

//     QDir().mkpath(vaultDir());
//     const QString vault = vaultDir() + QStringLiteral("/") + sha256 +
//                           QStringLiteral(".qvault");

//     const QByteArray key = deriveKey();
//     if (!aesCbcEncryptFile(originalPath, vault, key)) {
//         Logger::error(QStringLiteral("Vault encrypt failed: %1").arg(originalPath));
//         return {};
//     }

//     QSqlDatabase db = QSqlDatabase::database("verax_main");
//     if (!db.isOpen()) {
//         Logger::error("DB not open during quarantine move");
//         return {};
//     }


//     QSqlQuery q(db);
//     q.prepare("INSERT INTO quarantine "
//               "(original_path, vault_path, sha256, detection_name, size, quarantined_at) "
//               "VALUES (?, ?, ?, ?, ?, ?)");
//     q.bindValue(0, originalPath);
//     q.bindValue(1, vault);
//     q.bindValue(2, sha256);
//     q.bindValue(3, detectionName);
//     q.bindValue(4, fi.size());
//     q.bindValue(5, QDateTime::currentSecsSinceEpoch());
//     if (!q.exec()) {
//         Logger::error(QStringLiteral("Quarantine DB insert failed: %1")
//                       .arg(q.lastError().text()));
//         QFile::remove(vault);
//         return {};
//     }

//     if (!secureDelete(originalPath)) {
//         Logger::warn(QStringLiteral("Source not securely deleted: %1").arg(originalPath));
//     }

//     QuarantineEntry e;
//     e.id            = q.lastInsertId().toInt();
//     e.originalPath  = originalPath;
//     e.vaultPath     = vault;
//     e.sha256        = sha256;
//     e.detectionName = detectionName;
//     e.size          = fi.size();
//     e.quarantinedAt = QDateTime::currentSecsSinceEpoch();
//     emit itemAdded(e);
//     emit changed();
//     return vault;
// }

QString Quarantine::moveToVault(const QString &originalPath,
                                const QString &sha256,
                                const QString &detectionName)
{
    QFileInfo fi(originalPath);
    if (!fi.exists() || !fi.isFile()) return {};

    QDir().mkpath(vaultDir());
    const QString vault = vaultDir() + QStringLiteral("/") + sha256 +
                          QStringLiteral(".qvault");

    const QByteArray key = deriveKey();
    if (!aesCbcEncryptFile(originalPath, vault, key)) {
        Logger::error(QStringLiteral("Vault encrypt failed: %1").arg(originalPath));
        return {};
    }

    QSqlDatabase db;
    QString connectionName = QStringLiteral("quarantine_thread_link_%1").arg(quintptr(QThread::currentThreadId()));

    if (QSqlDatabase::contains(connectionName)) {
        db = QSqlDatabase::database(connectionName);
    } else {
        db = QSqlDatabase::addDatabase(QStringLiteral("QSQLITE"), connectionName);
        db.setDatabaseName(Logger::userDataDir() + QStringLiteral("/db/verax.sqlite"));
    }

    if (!db.isOpen() && !db.open()) {
        Logger::error(QStringLiteral("DB not open during quarantine move: %1").arg(db.lastError().text()));
        QFile::remove(vault);
        return {};
    }

    QSqlQuery q(db);
    q.prepare("INSERT INTO quarantine "
              "(original_path, vault_path, sha256, detection_name, size, quarantined_at, restore_blocked) "
              "VALUES (?, ?, ?, ?, ?, ?, 0)");

    q.bindValue(0, originalPath);
    q.bindValue(1, vault);
    q.bindValue(2, sha256);
    q.bindValue(3, detectionName);
    q.bindValue(4, fi.size());
    q.bindValue(5, QDateTime::currentSecsSinceEpoch());

    if (!q.exec()) {
        Logger::error(QStringLiteral("Quarantine DB insert failed: %1").arg(q.lastError().text()));
        QFile::remove(vault);
        return {};
    }

    if (!secureDelete(originalPath)) {
        Logger::warn(QStringLiteral("Source not securely deleted: %1").arg(originalPath));
    }

    QuarantineEntry e;
    e.id            = q.lastInsertId().toInt();
    e.originalPath  = originalPath;
    e.vaultPath     = vault;
    e.sha256        = sha256;
    e.detectionName = detectionName;
    e.size          = fi.size();
    e.quarantinedAt = QDateTime::currentSecsSinceEpoch();
    e.restoreBlocked = false;

    emit itemAdded(e);
    emit changed();

    return vault;
}

bool Quarantine::restore(int entryId)
{
    QSqlDatabase db = QSqlDatabase::database("verax_main");
    if (!db.isOpen()) return false;

    QSqlQuery q(db);
    q.prepare("SELECT original_path, vault_path, restore_blocked FROM quarantine WHERE id = ?");
    q.bindValue(0, entryId);
    if (!q.exec() || !q.next()) return false;

    const QString original = q.value(0).toString();
    const QString vault    = q.value(1).toString();
    const bool blocked     = q.value(2).toInt() != 0;
    if (blocked) {
        Logger::warn(QStringLiteral("Restore blocked: id=%1").arg(entryId));
        return false;
    }

    QDir().mkpath(QFileInfo(original).absolutePath());
    if (!aesCbcDecryptFile(vault, original, deriveKey())) return false;

    QSqlQuery del(db);
    del.prepare("DELETE FROM quarantine WHERE id = ?");
    del.bindValue(0, entryId);
    del.exec();
    QFile::remove(vault);

    emit changed();
    return true;
}

bool Quarantine::permanentDelete(int entryId)
{
    QSqlDatabase db = QSqlDatabase::database("verax_main");
    if (!db.isOpen()) return false;

    QSqlQuery q(db);
    q.prepare("SELECT vault_path FROM quarantine WHERE id = ?");
    q.bindValue(0, entryId);
    if (q.exec() && q.next())
        secureDelete(q.value(0).toString());

    QSqlQuery del(db);
    del.prepare("DELETE FROM quarantine WHERE id = ?");
    del.bindValue(0, entryId);
    const bool ok = del.exec();
    if (ok) emit changed();
    return ok;
}

QVector<QuarantineEntry> Quarantine::list() const
{
    QVector<QuarantineEntry> out;
    QSqlDatabase db = QSqlDatabase::database("verax_main");
    if (!db.isOpen()) return out;

    QSqlQuery q(db);
    if (!q.exec("SELECT id, original_path, vault_path, sha256, "
                "       detection_name, size, quarantined_at, restore_blocked "
                "FROM quarantine ORDER BY quarantined_at DESC"))
        return out;

    while (q.next()) {
        QuarantineEntry e;
        e.id             = q.value(0).toInt();
        e.originalPath   = q.value(1).toString();
        e.vaultPath      = q.value(2).toString();
        e.sha256         = q.value(3).toString();
        e.detectionName  = q.value(4).toString();
        e.size           = q.value(5).toLongLong();
        e.quarantinedAt  = q.value(6).toLongLong();
        e.restoreBlocked = q.value(7).toInt() != 0;
        out.append(e);
    }
    return out;
}

int Quarantine::count() const
{
    QSqlDatabase db = QSqlDatabase::database("verax_main");
    if (!db.isOpen()) return 0;
    QSqlQuery q(db);
    if (q.exec("SELECT COUNT(*) FROM quarantine") && q.next())
        return q.value(0).toInt();
    return 0;
}

qint64 Quarantine::totalBytes() const
{
    QSqlDatabase db = QSqlDatabase::database("verax_main");
    if (!db.isOpen()) return 0;
    QSqlQuery q(db);
    if (q.exec("SELECT COALESCE(SUM(size),0) FROM quarantine") && q.next())
        return q.value(0).toLongLong();
    return 0;
}

} // namespace verax
