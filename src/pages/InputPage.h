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

/**
 * @brief 表单输入类控件示例页。
 *
 * 覆盖 QLineEdit、QComboBox、QSpinBox、QDateEdit、QTextEdit、QSlider。
 * 这个页面展示了 QFormLayout 的基本用法，以及如何响应输入值变化。
 */
class InputPage : public ControlPageBase {
    Q_OBJECT

public:
    explicit InputPage(QWidget *parent = nullptr);
    /** 返回翻译后的页面标题。 */
    QString pageName() const override;

public slots:
    /** 重建可见文本和下拉框选项文本。 */
    void retranslateUi() override;

private slots:
    /** 把输入值组合成实时预览文本。 */
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
