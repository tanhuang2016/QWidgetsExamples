#pragma once

#include "ui/ControlPageBase.h"

class QLabel;
class QLineEdit;
class QTextEdit;

class FeedbackPage : public ControlPageBase {
    Q_OBJECT

public:
    explicit FeedbackPage(QWidget *parent = nullptr);
    QString pageName() const override;

public slots:
    void retranslateUi() override;

private slots:
    void showInfo();
    void showQuestion();
    void showInput();
    void showProgress();
    void showTooltipNow();
    void validateText();

private:
    void toast(const QString &message);
    void appendLog(const QString &message);

    QLabel *m_tipTarget;
    QLabel *m_validationLabel;
    QLineEdit *m_lineEdit;
    QTextEdit *m_log;
};
