#pragma once

#include "ui/ControlPageBase.h"

class QLCDNumber;
class QLineEdit;
class QTextEdit;
class QUndoStack;

class AdvancedPage : public ControlPageBase {
    Q_OBJECT

public:
    explicit AdvancedPage(QWidget *parent = nullptr);
    QString pageName() const override;

public slots:
    void retranslateUi() override;

private slots:
    void updateLcd(int value);
    void pushUndoCommand();

private:
    QLCDNumber *m_lcd;
    QLineEdit *m_completerEdit;
    QTextEdit *m_log;
    QUndoStack *m_undoStack;
};
