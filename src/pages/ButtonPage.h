#pragma once

#include "ui/ControlPageBase.h"

class QCheckBox;
class QGroupBox;
class QLabel;
class QPushButton;
class QRadioButton;
class QToolButton;

class ButtonPage : public ControlPageBase {
    Q_OBJECT

public:
    explicit ButtonPage(QWidget *parent = nullptr);
    QString pageName() const override;

public slots:
    void retranslateUi() override;

private slots:
    void updateStatus();

private:
    QGroupBox *m_pushGroup;
    QGroupBox *m_optionGroup;
    QPushButton *m_primaryButton;
    QPushButton *m_toggleButton;
    QToolButton *m_toolButton;
    QCheckBox *m_checkBox;
    QRadioButton *m_radioA;
    QRadioButton *m_radioB;
    QLabel *m_statusLabel;
};
