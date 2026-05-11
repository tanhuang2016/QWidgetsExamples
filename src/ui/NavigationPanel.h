#pragma once

#include <QTreeWidget>

class NavigationPanel : public QTreeWidget {
    Q_OBJECT

public:
    explicit NavigationPanel(QWidget *parent = nullptr);
    void setPages(const QStringList &keys);
    void rebuild();
    void retranslateUi();

signals:
    void pageSelected(const QString &key);

private slots:
    void onCurrentItemChanged(QTreeWidgetItem *current, QTreeWidgetItem *);

private:
    QStringList m_keys;
};
