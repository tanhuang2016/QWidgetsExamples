#pragma once

#include <QTreeWidget>

/**
 * @brief 左侧导航树。
 *
 * 顶层节点是不可选中的分组，子节点才是具体页面。子节点把页面 key 存在
 * Qt::UserRole 中，点击时发出 pageSelected(key) 信号。
 */
class NavigationPanel : public QTreeWidget {
    Q_OBJECT

public:
    explicit NavigationPanel(QWidget *parent = nullptr);

    /**
     * @brief 设置可见页面 key，并重建导航树。
     */
    void setPages(const QStringList &keys);

    /**
     * @brief 根据当前注册表状态重建导航树。
     */
    void rebuild();

    /**
     * @brief 重新应用分组和页面标题的翻译文本。
     */
    void retranslateUi();

signals:
    /**
     * @brief 用户选择具体页面节点时发出。
     */
    void pageSelected(const QString &key);

private slots:
    void onCurrentItemChanged(QTreeWidgetItem *current, QTreeWidgetItem *);

private:
    QStringList m_keys;
};
