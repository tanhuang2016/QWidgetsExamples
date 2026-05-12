#include "TranslationManager.h"
#include "core/Logger.h"

#include <QApplication>
#include <QCoreApplication>
#include <QDir>
#include <QStringList>

namespace {
QStringList translationSearchPaths()
{
    const QDir appDir(QCoreApplication::applicationDirPath());
    QStringList paths;
    paths << appDir.filePath("resources/translations");
    paths << appDir.filePath("../resources/translations");
    paths << appDir.filePath("../../resources/translations");
    paths << appDir.filePath("../../../resources/translations");
    paths << QDir(QCoreApplication::applicationDirPath() + "/../Resources").filePath("translations");
    paths.removeDuplicates();
    return paths;
}
}

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
        loaded = false;
        for (const QString &basePath : translationSearchPaths()) {
            if (m_translator.load(locale, basePath)) {
                loaded = true;
                Logger::info(QString("Loaded translation %1 from %2").arg(locale, basePath));
                break;
            }
        }
        if (!loaded) {
            Logger::warning(QString("Translation file not found for %1. Search paths: %2")
                            .arg(locale, translationSearchPaths().join("; ")));
        } else {
            m_app->installTranslator(&m_translator);
        }
    }

    emit languageChanged();
    return loaded;
}
