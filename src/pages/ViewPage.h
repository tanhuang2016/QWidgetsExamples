#pragma once

#include "ui/ControlPageBase.h"

class QGroupBox;
class QListWidget;
class QTableWidget;
class QTreeWidget;

/**
 * @brief 基于 item 的视图控件示例页。
 *
 * QListWidget、QTableWidget、QTreeWidget 属于易上手的 item 控件，适合小数据量
 * 和学习阶段使用。熟悉它们后，再学习 Model/View 会更顺。
 */
class ViewPage : public ControlPageBase {
    Q_OBJECT

public:
    explicit ViewPage(QWidget *parent = nullptr);
    /** 返回翻译后的页面标题。 */
    QString pageName() const override;

public slots:
    /** 重新设置分组标题，并填充翻译后的示例数据。 */
    void retranslateUi() override;

private:
    /** 给列表、表格、树填充小规模示例数据。 */
    void fillData();

    QGroupBox *m_listGroup;
    QGroupBox *m_tableGroup;
    QGroupBox *m_treeGroup;
    QListWidget *m_list;
    QTableWidget *m_table;
    QTreeWidget *m_tree;
};
