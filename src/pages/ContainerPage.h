#pragma once

#include "ui/ControlPageBase.h"

class QMdiArea;
class QTabWidget;
class QTextEdit;

class ContainerPage : public ControlPageBase {
    Q_OBJECT

public:
    explicit ContainerPage(QWidget *parent = nullptr);
    QString pageName() const override;

public slots:
    void retranslateUi() override;

private slots:
    void addTab();
    void addMdiWindow();
    void tileMdi();
    void cascadeMdi();

private:
    void appendLog(const QString &message);

    QTabWidget *m_tabs;
    QMdiArea *m_mdi;
    QTextEdit *m_log;
};
