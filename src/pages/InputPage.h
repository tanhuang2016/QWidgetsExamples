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
 * @brief 表单输入类控件示例页
 * 
 * 【功能说明】
 * 展示各种表单输入控件的使用方法，包括：
 * - QLineEdit: 单行文本输入
 * - QComboBox: 下拉选择框
 * - QSpinBox: 数值输入框
 * - QDateEdit: 日期选择器
 * - QTextEdit: 多行文本输入
 * - QSlider: 滑动条
 * 
 * 【布局方式】
 * 使用QFormLayout实现标签-输入框的对齐布局，类似HTML的<form>表单
 * 
 * Java对比：
 * - 类似于 JPanel + SpringLayout 或 GroupLayout
 * - 或者 JavaFX的 FormPane
 */
class InputPage : public ControlPageBase {
    Q_OBJECT

public:
    /**
     * @brief 构造函数
     * 
     * @param parent 父窗口指针
     */
    explicit InputPage(QWidget *parent = nullptr);
    
    /**
     * @brief 返回翻译后的页面标题
     * 
     * @return QString "Inputs" -> "输入控件"（根据当前语言）
     */
    QString pageName() const override;

public slots:
    /**
     * @brief 重新翻译页面上的所有文本
     * 
     * 【调用时机】
     * 当用户切换语言时，会调用此方法更新所有标签和控件的显示文本
     * 
     * Java对比：类似于 Component.revalidate() + setText(tr(...))
     */
    void retranslateUi() override;

private slots:
    /**
     * @brief 更新预览文本
     * 
     * 【功能】
     * 实时将各个输入控件的值组合成一段描述性文本
     * 用户在任意输入框输入时，预览文本会立即更新
     * 
     * Java对比：类似于 DocumentListener.changedUpdate() 触发的回调
     */
    void updatePreview();

private:
    // 表单容器组
    QGroupBox *m_formGroup;
    
    // 各种输入控件的标签
    QLabel *m_nameLabel;    // 名称标签
    QLabel *m_typeLabel;    // 类型标签
    QLabel *m_countLabel;   // 数量标签
    QLabel *m_dateLabel;    // 日期标签
    QLabel *m_noteLabel;    // 备注标签
    
    // 各种输入控件
    QLineEdit *m_nameEdit;   // 名称输入框
    QComboBox *m_typeCombo;   // 类型下拉框
    QSpinBox *m_countSpin;    // 数量数值框
    QDateEdit *m_dateEdit;    // 日期选择器
    QTextEdit *m_noteEdit;    // 备注多行输入
    QSlider *m_slider;        // 滑动条
    
    // 实时预览标签
    QLabel *m_previewLabel;   // 显示各输入控件值的组合文本
};
