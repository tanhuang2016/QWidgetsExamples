#include "ViewPage.h"
#include "core/Logger.h"

#include <QGroupBox>
#include <QHBoxLayout>
#include <QHeaderView>
#include <QListWidget>
#include <QTableWidget>
#include <QTreeWidget>
#include <QVBoxLayout>

ViewPage::ViewPage(QWidget *parent)
    : ControlPageBase(parent)
{
    QVBoxLayout *root = new QVBoxLayout(this);
    root->setContentsMargins(24, 24, 24, 24);
    QHBoxLayout *views = new QHBoxLayout();

    m_listGroup = new QGroupBox(this);
    m_tableGroup = new QGroupBox(this);
    m_treeGroup = new QGroupBox(this);
    m_list = new QListWidget(this);
    m_table = new QTableWidget(this);
    m_tree = new QTreeWidget(this);

    QVBoxLayout *listLayout = new QVBoxLayout(m_listGroup);
    QVBoxLayout *tableLayout = new QVBoxLayout(m_tableGroup);
    QVBoxLayout *treeLayout = new QVBoxLayout(m_treeGroup);
    listLayout->addWidget(m_list);
    tableLayout->addWidget(m_table);
    treeLayout->addWidget(m_tree);

    views->addWidget(m_listGroup);
    views->addWidget(m_tableGroup, 2);
    views->addWidget(m_treeGroup);
    root->addLayout(views);

    m_table->horizontalHeader()->setStretchLastSection(true);
    m_tree->header()->setStretchLastSection(true);

    retranslateUi();
    Logger::info("ViewPage initialized");
}

QString ViewPage::pageName() const
{
    return tr("Views");
}

void ViewPage::retranslateUi()
{
    m_listGroup->setTitle(tr("List Widget"));
    m_tableGroup->setTitle(tr("Table Widget"));
    m_treeGroup->setTitle(tr("Tree Widget"));
    fillData();
}

void ViewPage::fillData()
{
    m_list->clear();
    m_list->addItems(QStringList() << tr("Alpha") << tr("Beta") << tr("Gamma"));

    m_table->clear();
    m_table->setRowCount(3);
    m_table->setColumnCount(3);
    m_table->setHorizontalHeaderLabels(QStringList() << tr("Name") << tr("Status") << tr("Value"));
    for (int row = 0; row < 3; ++row) {
        m_table->setItem(row, 0, new QTableWidgetItem(tr("Item %1").arg(row + 1)));
        m_table->setItem(row, 1, new QTableWidgetItem(row % 2 == 0 ? tr("Ready") : tr("Pending")));
        m_table->setItem(row, 2, new QTableWidgetItem(QString::number((row + 1) * 10)));
    }

    m_tree->clear();
    m_tree->setHeaderLabels(QStringList() << tr("Control") << tr("Description"));
    QTreeWidgetItem *inputs = new QTreeWidgetItem(m_tree, QStringList() << tr("Inputs") << tr("Editable controls"));
    new QTreeWidgetItem(inputs, QStringList() << tr("Line Edit") << tr("Single line text"));
    new QTreeWidgetItem(inputs, QStringList() << tr("Spin Box") << tr("Numeric input"));
    QTreeWidgetItem *views = new QTreeWidgetItem(m_tree, QStringList() << tr("Views") << tr("Data display"));
    new QTreeWidgetItem(views, QStringList() << tr("Table") << tr("Rows and columns"));
    m_tree->expandAll();
}
