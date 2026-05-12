#include "NavigationPanel.h"
#include "core/AppContext.h"

#include <QHeaderView>

/**
 * @brief NavigationPanel构造函数
 * 
 * 【初始化设置】
 * 1. 设置固定宽度230px（左侧导航栏不需要太宽）
 * 2. 隐藏根节点装饰符（不显示展开/折叠图标）
 * 3. 统一行高（美观）
 * 4. 交替行颜色（便于区分）
 * 5. 隐藏表头（QTreeWidget默认有表头，但这里不需要）
 * 
 * Java对比：类似于 JTree的初始化配置
 * 
 * @param parent 父窗口指针
 */
NavigationPanel::NavigationPanel(QWidget *parent)
    : QTreeWidget(parent)
{
    setFixedWidth(230);              // 固定宽度230像素
    setRootIsDecorated(false);       // 不显示根节点的展开/折叠图标
    setUniformRowHeights(true);      // 所有行高度一致
    setAlternatingRowColors(true);   // 启用交替行颜色（斑马纹效果）
    header()->hide();                // 隐藏表头

    // 【信号槽连接】
    // 当用户点击不同的树节点时，触发onCurrentItemChanged槽函数
    // Java对比：类似于 tree.addTreeSelectionListener(this)
    connect(this, &QTreeWidget::currentItemChanged,
            this, &NavigationPanel::onCurrentItemChanged);
}

/**
 * @brief 设置可见页面列表并重建导航树
 * 
 * 【工作流程】
 * 1. 保存页面key列表到m_keys成员变量
 * 2. 调用retranslateUi()构建导航树结构
 * 3. 自动选中第一个可用的页面节点
 * 
 * @param keys 页面key列表，从ControlRegistry::keys()获取
 */
void NavigationPanel::setPages(const QStringList &keys)
{
    m_keys = keys;  // 保存页面列表
    retranslateUi(); // 构建导航树
    
    // 【自动选中第一项】
    // 遍历所有分组，找到第一个有子节点的分组，选中其第一个子节点
    for (int i = 0; i < topLevelItemCount(); ++i) {
        QTreeWidgetItem *group = topLevelItem(i);
        if (group->childCount() > 0) {
            setCurrentItem(group->child(0));  // 选中第一个子节点
            return;
        }
    }
}

/**
 * @brief 根据当前注册表状态重建导航树
 * 
 * 【用途】
 * 当注册表中的页面发生变化时调用此方法刷新导航树
 * 目前实现只是简单调用retranslateUi()
 */
void NavigationPanel::rebuild()
{
    retranslateUi();
}

/**
 * @brief 重新构建导航树并应用翻译文本
 * 
 * 【核心逻辑】
 * 1. 清空现有树结构
 * 2. 遍历所有分组（从ControlRegistry获取）
 * 3. 为每个分组创建顶层节点（不可选中）
 * 4. 为分组下的每个页面创建子节点（可选中）
 * 5. 将页面key存储到Qt::UserRole中（用于后续识别）
 * 6. 展开所有分组节点
 * 
 * 【Qt::UserRole说明】
 * Qt::UserRole是Qt预定义的用户数据角色，可以存储任意QVariant类型的数据
 * 这里用来存储页面key，即使显示文本改变（语言切换），key仍然保持不变
 * 
 * Java对比：类似于 TreeNode.setUserObject(key)
 */
void NavigationPanel::retranslateUi()
{
    clear();  // 清空所有节点
    
    // 获取全局页面注册表
    ControlRegistry &registry = AppContext::instance().registry();
    
    // 【外层循环】遍历所有分组
    for (const QString &groupKey : registry.groups()) {
        // 创建顶层节点（分组节点）
        QTreeWidgetItem *group = new QTreeWidgetItem(this);
        group->setText(0, registry.groupTitle(groupKey));  // 设置分组标题（已翻译）
        
        // 【重要】禁用分组的可选中标志，用户只能点击子节点
        group->setFlags(group->flags() & ~Qt::ItemIsSelectable);
        
        addTopLevelItem(group);  // 添加为顶层节点
        
        // 【内层循环】遍历该分组下的所有页面
        for (const QString &key : registry.keysInGroup(groupKey)) {
            // 检查该页面是否在可见列表中
            if (!m_keys.contains(key)) {
                continue;  // 跳过不可见的页面
            }
            
            // 创建子节点（页面节点）
            QTreeWidgetItem *item = new QTreeWidgetItem(group);
            item->setText(0, registry.title(key));  // 设置页面标题（已翻译）
            
            // 【关键】将页面key存储到Qt::UserRole中
            // 这样即使显示文本改变（语言切换），key仍然保持稳定
            // Java对比：类似于 node.putClientProperty("pageKey", key)
            item->setData(0, Qt::UserRole, key);
            
            group->addChild(item);  // 添加为分组节点的子节点
        }
        
        // 默认展开所有分组
        group->setExpanded(true);
    }
}

/**
 * @brief 处理当前选中项变化的事件
 * 
 * 【触发时机】
 * 用户在导航树中点击不同的节点时自动调用
 * 
 * 【处理逻辑】
 * 1. 检查current是否为空（可能用户取消了选择）
 * 2. 从Qt::UserRole中提取页面key
 * 3. 如果key非空，发出pageSelected信号
 * 
 * 【为什么需要检查key.isEmpty()？】
 * 因为分组节点没有设置Qt::UserRole，它们的key为空字符串
 * 这样可以确保只有点击页面节点时才发出信号
 * 
 * Java对比：类似于 TreeSelectionListener.valueChanged(TreeSelectionEvent e)
 * 
 * @param current 新选中的树节点
 * @param previous 之前选中的树节点（未使用）
 */
void NavigationPanel::onCurrentItemChanged(QTreeWidgetItem *current, QTreeWidgetItem *)
{
    // 安全检查：防止空指针
    if (!current) {
        return;
    }
    
    // 从Qt::UserRole中提取页面key
    const QString key = current->data(0, Qt::UserRole).toString();
    
    // 只有当key非空时才发出信号（排除分组节点）
    if (!key.isEmpty()) {
        emit pageSelected(key);  // 发出信号，通知ContentStack显示对应页面
    }
}
