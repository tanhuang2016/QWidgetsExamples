#include "TranslationManager.h"
#include "core/Logger.h"

#include <QApplication>
#include <QCoreApplication>
#include <QDir>

TranslationManager::TranslationManager(QApplication *app, QObject *parent)
    : QObject(parent),
      m_app(app),
      m_locale("en_US")
{
}

QString TranslationManager::currentLocale() const
{
    return m_locale;
}

bool TranslationManager::switchLanguage(const QString &locale)
{
    if (locale == m_locale) {
        emit languageChanged();
        return true;
    }

    m_app->removeTranslator(&m_translator);
    m_locale = locale;

    bool loaded = true;
    if (locale != "en_US") {
        const QString basePath = QDir(QCoreApplication::applicationDirPath()).filePath("resources/translations");
        loaded = m_translator.load(locale, basePath);
        if (loaded) {
            m_app->installTranslator(&m_translator);
        } else {
            Logger::warning(QString("Translation file not found for %1").arg(locale));
        }
    }

    emit languageChanged();
    return loaded;
}
