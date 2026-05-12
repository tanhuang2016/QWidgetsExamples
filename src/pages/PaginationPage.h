#pragma once

#include "ui/ControlPageBase.h"

class QListWidget;
class QTextEdit;
class QSpinBox;
class PaginationWidget;

/**
 * @brief 独立分页控件示例页。
 *
 * 展示 PaginationWidget 不只可以驱动表格，也可以驱动任意数据视图。
 * 这里的列表内容由当前页、每页条数、总记录数动态生成。
 */
class PaginationPage : public ControlPageBase {
    Q_OBJECT

public:
    explicit PaginationPage(QWidget *parent = nullptr);
    /** 返回翻译后的页面标题。 */
    QString pageName() const override;

public slots:
    /** 语言切换后重新应用按钮文本。 */
    void retranslateUi() override;

private slots:
    /** 根据当前页重建列表项。 */
    void refreshItems();
    /** 应用用户输入的总记录数。 */
    void setTotalFromControl();
    /** 生成随机总记录数并刷新列表。 */
    void randomTotal();

private:
    QListWidget *m_list;
    QTextEdit *m_log;
    QSpinBox *m_totalSpin;
    PaginationWidget *m_pagination;
};
