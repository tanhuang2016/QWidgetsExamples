#pragma once

#include "ui/ControlPageBase.h"

class QLabel;
class QLineEdit;
class QTextEdit;

/**
 * @brief 弹窗、Tooltip、Toast 和输入校验示例页。
 *
 * 覆盖 QMessageBox、QInputDialog、QProgressDialog 等标准弹窗，也展示
 * QToolTip、What's This 帮助和表单校验。
 */
class FeedbackPage : public ControlPageBase {
    Q_OBJECT

public:
    explicit FeedbackPage(QWidget *parent = nullptr);
    /** 返回翻译后的页面标题。 */
    QString pageName() const override;

public slots:
    /** 重新应用按钮文本和校验提示文本。 */
    void retranslateUi() override;

private slots:
    /** 显示带详细信息区域的 QMessageBox。 */
    void showInfo();
    /** 显示询问型 QMessageBox，并按结果显示 Toast。 */
    void showQuestion();
    /** 显示 QInputDialog，并把确认后的文本写入输入框。 */
    void showInput();
    /** 显示一个短时间的 QProgressDialog 示例。 */
    void showProgress();
    /** 在目标标签附近手动显示 QToolTip。 */
    void showTooltipNow();
    /** 校验输入框，并更新错误/成功样式。 */
    void validateText();

private:
    /** 创建一个临时的 Tooltip 风格通知。 */
    void toast(const QString &message);
    /** 追加界面日志，并写入文件日志。 */
    void appendLog(const QString &message);

    QLabel *m_tipTarget;
    QLabel *m_validationLabel;
    QLineEdit *m_lineEdit;
    QTextEdit *m_log;
};
