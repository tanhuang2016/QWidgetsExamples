#include "AppContext.h"
#include "i18n/TranslationManager.h"
// 包含所有页面类的头文件
// Java对比：类似于在配置类中 import 所有的 Controller
#include "pages/ButtonPage.h"
#include "pages/AdvancedPage.h"
#include "pages/ChartPage.h"
#include "pages/ContainerPage.h"
#include "pages/DialogPage.h"
#include "pages/FeedbackPage.h"
#include "pages/GraphicsPage.h"
#include "pages/InputPage.h"
#include "pages/LayoutPage.h"
#include "pages/MenuToolbarPage.h"
#include "pages/PaginationPage.h"
#include "pages/StylePage.h"
#include "pages/TablePage.h"
#include "pages/ViewPage.h"

#include <QApplication>

/**
 * @brief 获取全局唯一的AppContext实例（Meyers Singleton）
 * 
 * 【线程安全】
 * C++11保证静态局部变量的初始化是线程安全的
 * Java对比：类似于 static final AppContext INSTANCE = new AppContext()
 * 
 * @return AppContext& 单例对象的引用
 */
AppContext &AppContext::instance()
{
    // Meyers Singleton模式：首次调用时创建，后续调用复用同一实例
    static AppContext context;
    return context;
}

/**
 * @brief 私有构造函数
 * 
 * 【设计说明】
 * 构造函数设为private，防止外部直接创建实例
 * 只能通过 instance() 方法获取单例
 */
AppContext::AppContext()
    : m_translations(nullptr),  // 初始化为nullptr，稍后由initialize()创建
      m_initialized(false)       // 初始化标志，防止重复初始化
{
}

/**
 * @brief 初始化应用上下文
 * 
 * 【重要】必须在main()中创建QApplication之后调用
 * 
 * 【初始化步骤】
 * 1. 检查是否已经初始化（防止重复初始化）
 * 2. 创建TranslationManager并设置父对象为QApplication
 * 3. 注册所有示例页面到ControlRegistry
 * 4. 设置m_initialized标志为true
 * 
 * Java对比：类似于 @PostConstruct 或 ApplicationContext.refresh()
 * 
 * @param app QApplication指针，用于设置TranslationManager的父对象
 */
void AppContext::initialize(QApplication *app)
{
    // 幂等性检查：如果已经初始化过，直接返回
    if (m_initialized) {
        return;
    }

    // 【内存管理】
    // TranslationManager的父对象设置为QApplication
    // Qt的对象树机制会在QApplication销毁时自动delete它
    // Java对比：类似于Spring容器管理的Bean生命周期
    m_translations = new TranslationManager(app);

    // 【注册页面工厂】
    // 这里只注册工厂函数，不会立即创建页面对象
    // 实际页面会在用户点击导航项时由ContentStack懒加载创建
    registerPages();
    
    // 标记为已初始化
    m_initialized = true;
}

/**
 * @brief 获取页面注册表
 * 
 * @return ControlRegistry& 页面注册表的引用
 */
ControlRegistry &AppContext::registry()
{
    return m_registry;
}

/**
 * @brief 获取国际化管理器
 * 
 * @return TranslationManager& 国际化管理器的引用
 */
TranslationManager &AppContext::translations()
{
    return *m_translations;
}

/**
 * @brief 注册所有示例页面
 * 
 * 【注册格式】
 * registerPage(分组key, 页面key, 显示标题, 工厂函数)
 * 
 * 【分组说明】
 * - "basic": 基础控件（按钮、输入框等）
 * - "data": 数据视图（表格、列表、分页等）
 * - "actions": 菜单和工具栏
 * - "feedback": 对话框和反馈提示
 * - "graphics": 图形绘制和图表
 * - "style": 布局和样式
 * - "advanced": 高级功能（容器、复杂控件等）
 * 
 * 【工厂函数说明】
 * 使用C++ lambda表达式作为工厂函数：[]() { return new ButtonPage(); }
 * Java对比：类似于 () -> new ButtonPage() 或 Supplier<ButtonPage>
 * 
 * 【内存管理】
 * 工厂函数返回的QWidget指针会被插入QStackedWidget
 * Qt的父子对象机制会自动管理其生命周期
 */
void AppContext::registerPages()
{
    // === 基础控件组 ===
    m_registry.registerPage("basic", "buttons", QObject::tr("Buttons"), []() { return new ButtonPage(); });
    m_registry.registerPage("basic", "inputs", QObject::tr("Inputs"), []() { return new InputPage(); });
    
    // === 数据视图组 ===
    m_registry.registerPage("data", "views", QObject::tr("Views"), []() { return new ViewPage(); });
    m_registry.registerPage("data", "table", QObject::tr("Tables"), []() { return new TablePage(); });
    m_registry.registerPage("data", "pagination", QObject::tr("Pagination"), []() { return new PaginationPage(); });
    
    // === 菜单工具栏组 ===
    m_registry.registerPage("actions", "menuToolbar", QObject::tr("Menu && Toolbar"), []() { return new MenuToolbarPage(); });
    
    // === 对话框和反馈组 ===
    m_registry.registerPage("feedback", "dialogs", QObject::tr("Dialogs"), []() { return new DialogPage(); });
    m_registry.registerPage("feedback", "feedback", QObject::tr("Dialogs && Tips"), []() { return new FeedbackPage(); });
    
    // === 图形绘制组 ===
    m_registry.registerPage("graphics", "graphics", QObject::tr("Graphics View"), []() { return new GraphicsPage(); });
    m_registry.registerPage("graphics", "charts", QObject::tr("Charts"), []() { return new ChartPage(); });
    
    // === 布局和样式组 ===
    m_registry.registerPage("style", "layouts", QObject::tr("Layouts"), []() { return new LayoutPage(); });
    m_registry.registerPage("style", "style", QObject::tr("Style Controls"), []() { return new StylePage(); });
    
    // === 高级功能组 ===
    m_registry.registerPage("advanced", "containers", QObject::tr("Containers"), []() { return new ContainerPage(); });
    m_registry.registerPage("advanced", "advanced", QObject::tr("Advanced"), []() { return new AdvancedPage(); });
}
