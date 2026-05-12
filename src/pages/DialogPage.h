#pragma once

#include "ui/ControlPageBase.h"

class QGroupBox;
class QLabel;
class QPushButton;

/**
 * @brief 基础标准对话框示例页。
 *
 * 这个较小的页面补充 FeedbackPage，重点展示 QMessageBox、QColorDialog、
 * QFileDialog。
 */
class DialogPage : public ControlPageBase {
    Q_OBJECT

public:
    explicit DialogPage(QWidget *parent = nullptr);
    /** 返回翻译后的页面标题。 */
    QString pageName() const override;

public slots:
    /** 语言切换后重新应用按钮文本。 */
    void retranslateUi() override;

private slots:
    /** 显示 QMessageBox::information。 */
    void showMessage();
    /** 打开 QColorDialog，并显示用户选择的颜色。 */
    void chooseColor();
    /** 打开 QFileDialog，并显示用户选择的路径。 */
    void chooseFile();

private:
    QGroupBox *m_group;
    QPushButton *m_messageButton;
    QPushButton *m_colorButton;
    QPushButton *m_fileButton;
    QLabel *m_resultLabel;
};
