#include "NavigationPanel.h"
#include "core/AppContext.h"

#include <QHeaderView>

NavigationPanel::NavigationPanel(QWidget *parent)
    : QTreeWidget(parent)
{
    setFixedWidth(230);
    setRootIsDecorated(false);
    setUniformRowHeights(true);
    setAlternatingRowColors(true);
    header()->hide();

    connect(this, &QTreeWidget::currentItemChanged,
            this, &NavigationPanel::onCurrentItemChanged);
}

void NavigationPanel::setPages(const QStringList &keys)
{
    m_keys = keys;
    retranslateUi();
    for (int i = 0; i < topLevelItemCount(); ++i) {
        QTreeWidgetItem *group = topLevelItem(i);
        if (group->childCount() > 0) {
            setCurrentItem(group->child(0));
            return;
        }
    }
}

void NavigationPanel::rebuild()
{
    retranslateUi();
}

void NavigationPanel::retranslateUi()
{
    clear();
    ControlRegistry &registry = AppContext::instance().registry();
    for (const QString &groupKey : registry.groups()) {
        // Top-level item: category/group. It is not selectable; users select
        // concrete child pages.
        QTreeWidgetItem *group = new QTreeWidgetItem(this);
        group->setText(0, registry.groupTitle(groupKey));
        group->setFlags(group->flags() & ~Qt::ItemIsSelectable);
        addTopLevelItem(group);
        for (const QString &key : registry.keysInGroup(groupKey)) {
            if (!m_keys.contains(key)) {
                continue;
            }
            QTreeWidgetItem *item = new QTreeWidgetItem(group);
            item->setText(0, registry.title(key));
            // Store the page key separately from display text. Display text can
            // change with language; the key remains stable.
            item->setData(0, Qt::UserRole, key);
            group->addChild(item);
        }
        group->setExpanded(true);
    }
}

void NavigationPanel::onCurrentItemChanged(QTreeWidgetItem *current, QTreeWidgetItem *)
{
    if (!current) {
        return;
    }
    const QString key = current->data(0, Qt::UserRole).toString();
    if (!key.isEmpty()) {
        emit pageSelected(key);
    }
}
