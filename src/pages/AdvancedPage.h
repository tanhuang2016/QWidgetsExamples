#pragma once

#include "ui/ControlPageBase.h"

class QLCDNumber;
class QLineEdit;
class QTextEdit;
class QUndoStack;

/**
 * @brief 其他高级控件示例页。
 *
 * 覆盖 QLCDNumber、QKeySequenceEdit、QCompleter、QUndoView/QUndoStack。
 * 这些控件不如按钮和表单常见，但在真实工具软件中很有用。
 */
class AdvancedPage : public ControlPageBase {
    Q_OBJECT

public:
    explicit AdvancedPage(QWidget *parent = nullptr);
    /** 返回翻译后的页面标题。 */
    QString pageName() const override;

public slots:
    /** 语言刷新占位函数。 */
    void retranslateUi() override;

private slots:
    /** 把滑块值显示到 QLCDNumber。 */
    void updateLcd(int value);
    /** 向 QUndoStack 推入命令，让 QUndoView 自动显示。 */
    void pushUndoCommand();

private:
    QLCDNumber *m_lcd;
    QLineEdit *m_completerEdit;
    QTextEdit *m_log;
    QUndoStack *m_undoStack;
};
