#pragma once

#include "ui/ControlPageBase.h"

class QGroupBox;
class QListWidget;
class QTableWidget;
class QTreeWidget;

class ViewPage : public ControlPageBase {
    Q_OBJECT

public:
    explicit ViewPage(QWidget *parent = nullptr);
    QString pageName() const override;

public slots:
    void retranslateUi() override;

private:
    void fillData();

    QGroupBox *m_listGroup;
    QGroupBox *m_tableGroup;
    QGroupBox *m_treeGroup;
    QListWidget *m_list;
    QTableWidget *m_table;
    QTreeWidget *m_tree;
};
