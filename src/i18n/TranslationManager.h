#pragma once

#include <QObject>
#include <QTranslator>

class QApplication;

class TranslationManager : public QObject {
    Q_OBJECT

public:
    explicit TranslationManager(QApplication *app, QObject *parent = nullptr);
    QString currentLocale() const;

public slots:
    bool switchLanguage(const QString &locale);

signals:
    void languageChanged();

private:
    QApplication *m_app;
    QTranslator m_translator;
    QString m_locale;
};
