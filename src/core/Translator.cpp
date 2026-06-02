// Translator.cpp - live language switch
// By Ali Sakkaf - https://alisakkaf.com
#include "Translator.h"
#include "Logger.h"
#include "Settings.h"

#include <QApplication>
#include <QTranslator>
#include <QLibraryInfo>
#include <QLocale>
#include <QFile>

namespace verax {

Translator& Translator::instance() {
    static Translator t;
    return t;
}

Translator::Translator(QObject *parent) : QObject(parent) {}

void Translator::install(const QString &codeIn)
{
    const QString code = codeIn.isEmpty() ? QStringLiteral("en") : codeIn.toLower();

    // Idempotent: a repeated call with the same code is a no-op. This is
    // critical because flag-click + combo currentIndexChanged + Settings
    // change can otherwise trigger install() up to four times per switch,
    // each one queuing populate cascades that race and crash the UI.
    if (code == m_current) {
        Logger::info(QStringLiteral("Translator::install skip (already %1)").arg(code));
        return;
    }

    // Persist BEFORE applying so any signal/event handler firing during
    // installTranslator() observes the committed value.
    Settings::instance().setLanguage(code);

    if (m_appTr) {
        qApp->removeTranslator(m_appTr);
        m_appTr->deleteLater();
        m_appTr = nullptr;
    }
    if (m_qtTr) {
        qApp->removeTranslator(m_qtTr);
        m_qtTr->deleteLater();
        m_qtTr = nullptr;
    }

    // Always install Qt's own translations (for standard dialogs etc.)
    m_qtTr = new QTranslator(qApp);
    if (m_qtTr->load(QStringLiteral("qt_") + code,
                     QLibraryInfo::location(QLibraryInfo::TranslationsPath)))
    {
        qApp->installTranslator(m_qtTr);
    }

    // App translations from qrc
    m_appTr = new QTranslator(qApp);
    if (m_appTr->load(QStringLiteral(":/i18n/verax_") + code + QStringLiteral(".qm"))) {
        qApp->installTranslator(m_appTr);
    } else {
        // Falls back to source strings (English)
        Logger::warn(QStringLiteral("Translation file not found for: %1").arg(code));
    }

    qApp->setLayoutDirection(code == QStringLiteral("ar")
                             ? Qt::RightToLeft : Qt::LeftToRight);

    m_current = code;
    Logger::info(QStringLiteral("Locale set: %1 (RTL=%2)")
                 .arg(code).arg(isRtl() ? "yes" : "no"));
    emit localeChanged(code);
}

} // namespace verax
