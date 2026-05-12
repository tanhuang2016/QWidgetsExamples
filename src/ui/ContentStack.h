#pragma once

#include <QHash>
#include <QStackedWidget>

class ControlPageBase;

/**
 * @brief 右侧页面容器（支持懒加载）
 * 
 * 【核心功能】
 * 继承自 QStackedWidget，实现页面的懒加载和缓存：
 * 1. 懒加载：页面只在首次访问时创建，而不是启动时全部创建
 * 2. 缓存机制：创建后的页面会被缓存，后续访问直接复用
 * 3. 切换显示：通过 showPage(key) 切换到指定页面
 * 
 * 【为什么需要懒加载？】
 * - 节省内存：不需要一次性创建所有页面
 * - 提升启动速度：应用启动更快
 * - 按需加载：用户只访问部分页面，避免浪费
 * 
 * Java对比：
 * - 类似于 JTabbedPane + Lazy Loading Pattern
 * - 或者 Spring的 @Lazy Bean
 */
class ContentStack : public QStackedWidget {
    Q_OBJECT

public:
    /**
     * @brief 构造函数
     * 
     * @param parent 父窗口指针
     */
    explicit ContentStack(QWidget *parent = nullptr);

    /**
     * @brief 根据注册表key显示页面，必要时创建页面
     * 
     * 【工作流程】
     * 1. 检查 m_pages 缓存中是否已有该页面
     * 2. 如果有，直接调用 setCurrentWidget() 显示
     * 3. 如果没有，调用 loadPage(key) 创建并缓存
     * 
     * Java对比：类似于 tabbedPane.setSelectedTab("key")
     * 
     * @param key 页面唯一标识（如 "button"、"table"）
     */
    void showPage(const QString &key);

public slots:
    /**
     * @brief 对已经创建的页面调用 retranslateUi()
     * 
     * 【重要】只会翻译已加载的页面，未加载的页面在首次显示时会自动翻译
     */
    void retranslatePages();

private:
    /**
     * @brief 返回已缓存页面，或通过ControlRegistry创建新页面
     * 
     * 【异常处理】
     * 如果页面创建失败，会捕获异常并返回错误提示页面
     * 
     * @param key 页面唯一标识
     * @return ControlPageBase* 页面对象指针，失败返回错误页面
     */
    ControlPageBase *loadPage(const QString &key);

    /**
     * @brief 页面加载失败时显示的兜底错误页
     * 
     * 【容错机制】
     * 即使某个页面加载失败，也不会影响整个应用的运行
     * 用户会看到一个友好的错误提示，而不是程序崩溃
     * 
     * @param message 错误消息
     * @return QWidget* 错误提示页面
     */
    QWidget *createErrorPage(const QString &message);

    QHash<QString, ControlPageBase *> m_pages;  // 页面缓存池：key -> 页面实例
};
