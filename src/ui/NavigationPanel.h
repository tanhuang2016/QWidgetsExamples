#pragma once

#include <QTreeWidget>

/**
 * @brief 左侧导航树面板
 * 
 * 【功能说明】
 * 显示所有可用的示例页面，按分组组织成树形结构：
 * - 顶层节点：分组名称（如"基础控件"、"数据视图"），不可选中
 * - 子节点：具体页面（如"按钮"、"输入框"），可点击切换
 * 
 * 【交互方式】
 * 用户点击某个页面节点时，发出 pageSelected(key) 信号
 * ContentStack 接收该信号并显示对应的页面
 * 
 * Java对比：类似于 JTree 或 JavaFX的 TreeView
 */
class NavigationPanel : public QTreeWidget {
    Q_OBJECT

public:
    /**
     * @brief 构造函数
     * 
     * @param parent 父窗口指针
     */
    explicit NavigationPanel(QWidget *parent = nullptr);

    /**
     * @brief 设置可见页面列表并重建导航树
     * 
     * @param keys 页面key列表，从 ControlRegistry::keys() 获取
     */
    void setPages(const QStringList &keys);

    /**
     * @brief 根据当前注册表状态重建导航树
     * 
     * 当注册表中的页面发生变化时调用此方法刷新导航树
     */
    void rebuild();

    /**
     * @brief 重新应用分组和页面标题的翻译文本
     * 
     * 【重要】语言切换时调用，更新所有节点的显示文本
     */
    void retranslateUi();

signals:
    /**
     * @brief 用户选择具体页面节点时发出的信号
     * 
     * Java对比：类似于 ActionListener.actionPerformed(ActionEvent e)
     * 
     * @param key 被选中的页面唯一标识
     */
    void pageSelected(const QString &key);

private slots:
    /**
     * @brief 当前选中项变化时的处理槽函数
     * 
     * 【触发时机】
     * 用户在导航树中点击不同的节点时自动调用
     * 
     * @param current 新选中的树节点
     * @param previous 之前选中的树节点（未使用）
     */
    void onCurrentItemChanged(QTreeWidgetItem *current, QTreeWidgetItem *);

private:
    QStringList m_keys;  // 当前显示的页面key列表
};
