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

/**
 * @brief 样式和 QSS 实时预览示例页。
 *
 * 展示 QFontComboBox、QColorDialog、滑块、数字框和动态样式表的用法。
 * 目标是让你看到 QWidget 外观如何在运行时被控制。
 */
class StylePage : public ControlPageBase {
    Q_OBJECT

public:
    explicit StylePage(QWidget *parent = nullptr);
    /** 返回翻译后的页面标题。 */
    QString pageName() const override;

public slots:
    /** 语言切换后重新应用可见文本。 */
    void retranslateUi() override;

private slots:
    /** 打开 QColorDialog 选择文字颜色。 */
    void chooseTextColor();
    /** 打开 QColorDialog 选择背景颜色。 */
    void chooseBackgroundColor();
    /** 根据当前控制项重新构建字体和样式表。 */
    void applyStyle();
    /** 恢复默认样式控制值。 */
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
