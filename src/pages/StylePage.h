#pragma once

#include "ui/ControlPageBase.h"

class QCheckBox;
class QComboBox;
class QFontComboBox;
class QLabel;
class QPushButton;
class QSlider;
class QSpinBox;
class QTextEdit;

class StylePage : public ControlPageBase {
    Q_OBJECT

public:
    explicit StylePage(QWidget *parent = nullptr);
    QString pageName() const override;

public slots:
    void retranslateUi() override;

private slots:
    void chooseTextColor();
    void chooseBackgroundColor();
    void applyStyle();
    void resetStyle();

private:
    QLabel *m_previewLabel;
    QPushButton *m_previewButton;
    QTextEdit *m_qssPreview;
    QFontComboBox *m_fontCombo;
    QSpinBox *m_fontSizeSpin;
    QCheckBox *m_boldCheck;
    QCheckBox *m_italicCheck;
    QSpinBox *m_radiusSpin;
    QSpinBox *m_borderSpin;
    QSlider *m_opacitySlider;
    QComboBox *m_alignCombo;
    QPushButton *m_textColorButton;
    QPushButton *m_backgroundButton;
    QColor m_textColor;
    QColor m_backgroundColor;
};
