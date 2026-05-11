#pragma once

#include <QHash>
#include <QStackedWidget>

class ControlPageBase;

class ContentStack : public QStackedWidget {
    Q_OBJECT

public:
    explicit ContentStack(QWidget *parent = nullptr);
    void showPage(const QString &key);

public slots:
    void retranslatePages();

private:
    ControlPageBase *loadPage(const QString &key);
    QWidget *createErrorPage(const QString &message);

    QHash<QString, ControlPageBase *> m_pages;
};
