#include "ContentStack.h"
#include "core/AppContext.h"
#include "core/ExceptionHandler.h"
#include "core/Logger.h"
#include "ui/ControlPageBase.h"

#include <QLabel>
#include <QVBoxLayout>
#include <stdexcept>

/**
 * @brief ContentStack构造函数
 * 
 * 【初始化】
 * 继承自QStackedWidget，不需要额外的初始化
 * QStackedWidget本身就是一个可以切换显示子widget的容器
 * 
 * Java对比：类似于 JTabbedPane() 或 CardLayout
 * 
 * @param parent 父窗口指针
 */
ContentStack::ContentStack(QWidget *parent)
    : QStackedWidget(parent)
{
}

/**
 * @brief 根据页面key显示对应的页面
 * 
 * 【核心功能】
 * 这是ContentStack的主要方法，实现懒加载和页面切换
 * 
 * 【工作流程】
 * 1. 调用loadPage(key)获取页面对象（如果不存在则创建）
 * 2. 调用setCurrentWidget(page)切换到该页面
 * 3. 如果发生异常，显示错误页面并记录日志
 * 
 * 【异常处理】
 * - 捕获std::exception并记录到日志
 * - 创建错误提示页面作为fallback
 * - 调用ExceptionHandler显示友好提示
 * 
 * Java对比：类似于 tabbedPane.setSelectedComponent(getOrCreatePanel(key))
 * 
 * @param key 页面唯一标识（如"button"、"table"）
 */
void ContentStack::showPage(const QString &key)
{
    try {
        // 【懒加载】loadPage()会检查缓存，不存在则创建新页面
        ControlPageBase *page = loadPage(key);
        
        // 【切换显示】QStackedWidget的方法，显示指定的widget
        // 其他widget会被隐藏，但不会被销毁
        setCurrentWidget(page);
        
    } catch (const std::exception &e) {
        // 【异常处理】记录错误并显示fallback页面
        Logger::error(QString("Failed to show page %1: %2").arg(key, e.what()));
        
        // 创建错误提示页面
        QWidget *errorPage = createErrorPage(tr("Failed to load this page."));
        addWidget(errorPage);           // 添加到QStackedWidget
        setCurrentWidget(errorPage);    // 显示错误页面
        
        // 弹出错误对话框
        ExceptionHandler::handle(e);
    }
}

/**
 * @brief 重新翻译所有已加载页面的文本
 * 
 * 【工作原理】
 * 遍历m_pages缓存中的所有页面，调用它们的retranslateUi()方法
 * 
 * 【注意】
 * - 只会翻译已经加载的页面
 * - 未加载的页面在首次显示时会自动调用retranslateUi()
 * 
 * Java对比：类似于 for (Panel p : loadedPanels) { p.retranslateUI(); }
 */
void ContentStack::retranslatePages()
{
    // 范围for循环：C++11特性，类似Java的增强for循环
    for (ControlPageBase *page : m_pages) {
        page->retranslateUi();  // 调用每个页面的翻译方法
    }
}

/**
 * @brief 加载页面（懒加载核心方法）
 * 
 * 【工作流程】
 * 1. 检查m_pages缓存中是否已有该页面
 *    - 如果有：直接返回缓存的页面对象
 *    - 如果没有：继续执行步骤2-4
 * 
 * 2. 从ControlRegistry获取工厂函数并创建页面
 *    - 调用 AppContext::registry().create(key)
 *    - 工厂函数返回新的页面对象
 * 
 * 3. 验证页面指针是否有效
 *    - 如果为nullptr，抛出std::runtime_error
 * 
 * 4. 缓存页面并添加到QStackedWidget
 *    - 插入m_pages哈希表
 *    - 调用addWidget()添加到容器
 *    - 记录日志
 * 
 * 【内存管理】
 * - 页面通过new创建，堆分配
 * - addWidget(page)会将page的父对象设置为ContentStack
 * - Qt的对象树机制会在ContentStack销毁时自动delete所有子widget
 * 
 * Java对比：类似于 getOrCreateBean(key, PageClass)
 * 
 * @param key 页面唯一标识
 * @return ControlPageBase* 页面对象指针
 * @throws std::runtime_error 如果工厂函数返回nullptr
 */
ControlPageBase *ContentStack::loadPage(const QString &key)
{
    // 【缓存检查】如果页面已经存在，直接返回
    if (m_pages.contains(key)) {
        return m_pages.value(key);  // QHash的value()方法，类似Java HashMap的get()
    }

    // 【首次访问】通过注册表的工厂函数创建页面
    // Java对比：类似于 applicationContext.getBean(key)
    ControlPageBase *page = AppContext::instance().registry().create(key);
    
    // 【空指针检查】确保工厂函数返回了有效对象
    if (!page) {
        throw std::runtime_error("Page factory returned null.");
    }

    // 【缓存页面】
    m_pages.insert(key, page);  // 存入哈希表，类似HashMap.put()
    addWidget(page);            // 添加到QStackedWidget容器
    
    // 【记录日志】
    Logger::info(QString("Lazy loaded page: %1").arg(key));
    
    return page;
}

/**
 * @brief 创建错误提示页面（Fallback机制）
 * 
 * 【用途】
 * 当页面加载失败时，显示一个友好的错误提示，而不是让程序崩溃
 * 
 * 【页面结构】
 * QWidget
 *   └─ QVBoxLayout
 *       └─ QLabel (居中显示错误消息)
 * 
 * 【设计原则】
 * - 简单可靠：只使用最基本的控件，避免再次出错
 * - 用户友好：清晰的错误提示
 * - 容错性：即使某个页面失败，也不影响整个应用
 * 
 * Java对比：类似于创建 errorPanel with JLabel
 * 
 * @param message 错误消息文本
 * @return QWidget* 错误提示页面
 */
QWidget *ContentStack::createErrorPage(const QString &message)
{
    // 创建容器widget
    QWidget *page = new QWidget(this);
    
    // 创建垂直布局
    QVBoxLayout *layout = new QVBoxLayout(page);
    
    // 创建标签显示错误消息
    QLabel *label = new QLabel(message, page);
    label->setAlignment(Qt::AlignCenter);  // 居中对齐
    
    layout->addWidget(label);
    
    return page;
}
