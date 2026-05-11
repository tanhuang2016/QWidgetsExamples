#pragma once

#include "ui/ControlPageBase.h"

class QComboBox;
class QDateEdit;
class QGroupBox;
class QLineEdit;
class QLabel;
class QSlider;
class QSpinBox;
class QTextEdit;

class InputPage : public ControlPageBase {
    Q_OBJECT

public:
    explicit InputPage(QWidget *parent = nullptr);
    QString pageName() const override;

public slots:
    void retranslateUi() override;

private slots:
    void updatePreview();

private:
    QGroupBox *m_formGroup;
    QLabel *m_nameLabel;
    QLabel *m_typeLabel;
    QLabel *m_countLabel;
    QLabel *m_dateLabel;
    QLabel *m_noteLabel;
    QLineEdit *m_nameEdit;
    QComboBox *m_typeCombo;
    QSpinBox *m_countSpin;
    QDateEdit *m_dateEdit;
    QTextEdit *m_noteEdit;
    QSlider *m_slider;
    QLabel *m_previewLabel;
};
