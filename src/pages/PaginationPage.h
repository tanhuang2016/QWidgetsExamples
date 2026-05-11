#pragma once

#include "ui/ControlPageBase.h"

class QListWidget;
class QTextEdit;
class QSpinBox;
class PaginationWidget;

class PaginationPage : public ControlPageBase {
    Q_OBJECT

public:
    explicit PaginationPage(QWidget *parent = nullptr);
    QString pageName() const override;

public slots:
    void retranslateUi() override;

private slots:
    void refreshItems();
    void setTotalFromControl();
    void randomTotal();

private:
    QListWidget *m_list;
    QTextEdit *m_log;
    QSpinBox *m_totalSpin;
    PaginationWidget *m_pagination;
};
