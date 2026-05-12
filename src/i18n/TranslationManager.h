#pragma once

#include <QObject>
#include <QTranslator>

class QApplication;

/**
 * @brief 基于 Qt QTranslator 的运行时语言切换器。
 *
 * 源码里的界面文本使用 tr("...") 包裹。CMake 会从 .ts 文件生成 .qm
 * 二进制翻译文件，本类负责在程序运行时加载对应 .qm，无需重启程序。
 */
class TranslationManager : public QObject {
    Q_OBJECT

public:
    /**
     * @brief 需要传入 QApplication，因为翻译器要安装到 QApplication 上。
     */
    explicit TranslationManager(QApplication *app, QObject *parent = nullptr);

    /**
     * @brief 当前语言区域 ID，例如 "en_US" 或 "zh_CN"。
     */
    QString currentLocale() const;

public slots:
    /**
     * @brief 切换语言，并发出 languageChanged() 信号。
     *
     * 如果找不到 .qm 文件会返回 false。即使加载失败也会发出信号，
     * 这样界面可以按需刷新状态。
     */
    bool switchLanguage(const QString &locale);

signals:
    /**
     * @brief 翻译器变更后发出。
     */
    void languageChanged();

private:
    QApplication *m_app;
    QTranslator m_translator;
    QString m_locale;
};
