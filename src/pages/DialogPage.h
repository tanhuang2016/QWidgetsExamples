#pragma once

#include "ui/ControlPageBase.h"

class QGroupBox;
class QLabel;
class QPushButton;

class DialogPage : public ControlPageBase {
    Q_OBJECT

public:
    explicit DialogPage(QWidget *parent = nullptr);
    QString pageName() const override;

public slots:
    void retranslateUi() override;

private slots:
    void showMessage();
    void chooseColor();
    void chooseFile();

private:
    QGroupBox *m_group;
    QPushButton *m_messageButton;
    QPushButton *m_colorButton;
    QPushButton *m_fileButton;
    QLabel *m_resultLabel;
};
