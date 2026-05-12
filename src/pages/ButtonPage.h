#pragma once

#include "ui/ControlPageBase.h"

class QCheckBox;
class QGroupBox;
class QLabel;
class QPushButton;
class QRadioButton;
class QToolButton;

/**
 * @brief 常见按钮控件示例页。
 *
 * 覆盖 QPushButton、QToolButton、QCheckBox、QRadioButton。这个页面也演示
 * 多个控件的信号如何连接到同一个 slot，然后统一刷新状态文本。
 */
class ButtonPage : public ControlPageBase {
    Q_OBJECT

public:
    explicit ButtonPage(QWidget *parent = nullptr);
    /** 返回翻译后的页面标题。 */
    QString pageName() const override;

public slots:
    /** 语言切换后刷新页面上的所有文本。 */
    void retranslateUi() override;

private slots:
    /** 读取当前控件状态，并更新状态标签。 */
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
