#include "ControlRegistry.h"
#include "Logger.h"
#include "ui/ControlPageBase.h"

#include <QObject>
#include <stdexcept>

/**
 * @brief 注册页面到默认分组 "basic"
 * 
 * 【使用场景】
 * 用于快速注册不需要分组的简单页面,自动归类到 "basic" 分组
 * 
 * 【调用示例】
 * ControlRegistry::registerPage("buttons", tr("Buttons"), []() {
 *     return new ButtonPage();
 * });
 * 
 * @param key 页面唯一标识符(如 "buttons", "inputs")
 * @param title 页面显示标题(支持国际化翻译)
 * @param factory 工厂函数,返回新创建的页面对象指针
 */
void ControlRegistry::registerPage(const QString &key, const QString &title, const Factory &factory)
{
    // 委托给完整的注册方法,默认分组为 "basic"
    registerPage("basic", key, title, factory);
}

/**
 * @brief 注册页面到指定分组(核心注册方法)
 * 
 * 【工作原理】
 * 1. 验证参数合法性(key不能为空,factory必须有效)
 * 2. 维护分组顺序列表(m_groupOrder) - 保证导航栏分组显示顺序
 * 3. 维护页面顺序列表(m_order) - 保证同组内页面显示顺序
 * 4. 存储页面元信息到哈希表(m_entries) - 快速查找
 * 
 * 【数据结构设计】
 * - QHash m_entries: 提供 O(1) 查找速度,key -> Entry映射
 * - QList m_order: 保持插入顺序(QHash本身无序)
 * - QList m_groupOrder: 保持分组出现顺序
 * 
 * Java对比:
 * - 类似于 Spring 的 BeanDefinitionRegistry.registerBeanDefinition()
 * - 或者 Map<String, BeanDefinition> beanDefinitions
 * 
 * @param group 逻辑分组标识,如:
 *              - "basic": 基础控件(按钮、输入框等)
 *              - "data": 数据展示(表格、列表等)
 *              - "actions": 菜单和工具栏
 *              - "feedback": 对话框和反馈
 *              - "graphics": 图形和图表
 *              - "style": 样式和主题
 *              - "advanced": 高级功能
 * @param key 页面唯一标识,在整個应用中必须唯一
 * @param title 页面标题(注册时存储,但实际显示时会通过title()方法获取翻译后的版本)
 * @param factory 工厂函数,每次调用都会创建新的页面实例
 * 
 * @throws std::runtime_error 如果key为空或factory无效
 * 
 * 【调用示例】
 * ControlRegistry::registerPage("basic", "buttons", "Buttons", []() {
 *     return new ButtonPage();  // 懒加载:只有用户点击时才创建
 * });
 */
void ControlRegistry::registerPage(const QString &group, const QString &key, const QString &title, const Factory &factory)
{
    // 【参数校验】防止非法注册
    if (key.trimmed().isEmpty() || !factory) {
        throw std::runtime_error("Invalid page registration.");
    }

    // 【维护分组顺序】
    // 如果这个分组是第一次出现,添加到顺序列表
    // 例如: 先注册 "basic" 分组,再注册 "data" 分组
    // 导航栏就会按这个顺序显示分组
    if (!m_groupOrder.contains(group)) {
        m_groupOrder.append(group);
    }
    
    // 【维护页面顺序】
    // 如果这个页面key还没注册过,添加到顺序列表
    // 保证后续 keys() 返回的顺序与注册顺序一致
    if (!m_entries.contains(key)) {
        m_order.append(key);
    }
    
    // 【存储页面元信息】
    // 将分组、标题、工厂函数打包成 Entry 结构体存入哈希表
    // 后续可以通过 key 快速查找(时间复杂度 O(1))
    m_entries.insert(key, Entry{group, title, factory});
    
    // 【记录日志】方便调试,查看哪些页面被注册了
    Logger::info(QString("Registered page: %1").arg(key));
}

/**
 * @brief 获取所有页面的key列表(按注册顺序)
 * 
 * 【用途】
 * NavigationPanel 调用此方法构建左侧导航树的所有叶子节点
 * 
 * 【为什么不用 m_entries.keys()?】
 * - QHash::keys() 返回的顺序是不确定的(哈希表特性)
 * - 我们需要保持注册顺序,让用户看到的导航项顺序可预测
 * - 所以遍历 m_order(有序列表)而不是 m_entries(无序哈希)
 * 
 * Java对比: 类似于 router.getRoutes().stream().map(Route::getKey).collect(toList())
 * 
 * @return QStringList 所有页面的key,如 ["buttons", "inputs", "views", ...]
 */
QStringList ControlRegistry::keys() const
{
    QStringList result;
    // 遍历有序列表,保证返回顺序与注册顺序一致
    for (const QString &key : m_order) {
        result.append(key);
    }
    return result;
}

/**
 * @brief 获取所有分组的key列表(按首次出现的顺序)
 * 
 * 【用途】
 * NavigationPanel 调用此方法创建导航树的顶层分组节点
 * 
 * 【示例返回值】
 * ["basic", "data", "actions", "feedback", "graphics", "style", "advanced"]
 * 
 * @return QStringList 分组key列表
 */
QStringList ControlRegistry::groups() const
{
    QStringList result;
    for (const QString &group : m_groupOrder) {
        result.append(group);
    }
    return result;
}

/**
 * @brief 获取指定分组下的所有页面key(按注册顺序)
 * 
 * 【用途】
 * NavigationPanel 为每个分组创建子节点时使用
 * 例如: "basic" 分组下有 ["buttons", "inputs", "views"]
 * 
 * 【算法说明】
 * 遍历 m_order(保持顺序),筛选出属于指定分组的页面
 * 时间复杂度: O(n),n为总页面数
 * 
 * @param group 分组标识,如 "basic", "data"
 * @return QStringList 该分组下的页面key列表
 * 
 * 【示例】
 * keysInGroup("basic") → ["buttons", "inputs", "views"]
 * keysInGroup("graphics") → ["graphics", "charts"]
 */
QStringList ControlRegistry::keysInGroup(const QString &group) const
{
    QStringList result;
    // 遍历有序列表,过滤出目标分组的页面
    for (const QString &key : m_order) {
        if (m_entries.value(key).group == group) {
            result.append(key);
        }
    }
    return result;
}

/**
 * @brief 获取分组的显示标题(已翻译)
 * 
 * 【重要】这个方法硬编码了分组标题的翻译
 * 为什么不直接从 Entry.title 读取?因为:
 * 1. Entry.title 是注册时传入的原始字符串(可能未翻译)
 * 2. 这里使用 QObject::tr() 确保标题随语言切换动态更新
 * 3. 集中管理翻译,避免分散在各个注册调用处
 * 
 * 【Java对比】
 * 类似于 ResourceBundle.getString("group.basic.title")
 * 
 * @param group 分组key,如 "basic", "data"
 * @return QString 翻译后的分组标题
 * 
 * 【示例】
 * groupTitle("basic") → "Basic Widgets" (英文) / "基础控件" (中文)
 * groupTitle("unknown") → "unknown" (未知分组返回原值)
 */
QString ControlRegistry::groupTitle(const QString &group) const
{
    // 【分组标题映射表】
    // 注意: && 在 tr() 中会被转义为单个 & (Qt菜单加速键语法)
    if (group == "basic") {
        return QObject::tr("Basic Widgets");
    }
    if (group == "data") {
        return QObject::tr("Data Display");
    }
    if (group == "actions") {
        return QObject::tr("Menus && Actions");  // 显示为 "Menus & Actions"
    }
    if (group == "feedback") {
        return QObject::tr("Dialogs && Feedback");
    }
    if (group == "graphics") {
        return QObject::tr("Graphics && Reports");
    }
    if (group == "style") {
        return QObject::tr("Style && Theme");
    }
    if (group == "advanced") {
        return QObject::tr("Advanced");
    }
    // 未知分组:直接返回group作为标题(降级处理)
    return group;
}

/**
 * @brief 根据页面key查询所属分组
 * 
 * 【用途】
 * ContentStack 或 NavigationPanel 需要知道某个页面属于哪个分组时使用
 * 
 * 【时间复杂度】O(1) - 哈希表查找
 * 
 * @param key 页面key,如 "buttons", "table"
 * @return QString 分组key,如 "basic", "data"
 * 
 * 【示例】
 * groupOf("buttons") → "basic"
 * groupOf("table") → "data"
 */
QString ControlRegistry::groupOf(const QString &key) const
{
    // QHash::value() 在key不存在时返回默认构造的Entry(group为空字符串)
    return m_entries.value(key).group;
}

/**
 * @brief 获取页面的显示标题(已翻译)
 * 
 * 【设计说明】
 * 这个方法类似 groupTitle(),也是硬编码翻译映射
 * 
 * 【为什么不直接用 Entry.title?】
 * 1. Entry.title 是注册时传入的原始值,可能没有用 tr() 包裹
 * 2. 这里统一用 tr() 确保国际化支持
 * 3. 即使注册时传错标题,这里也能保证正确显示
 * 
 * 【两级查找策略】
 * 1. 优先使用硬编码的翻译映射(保证UI一致性)
 * 2. 如果找不到,降级使用注册时传入的 title(兜底)
 * 
 * @param key 页面key,如 "buttons", "inputs"
 * @return QString 翻译后的页面标题
 * 
 * 【示例】
 * title("buttons") → "Buttons" (英文) / "按钮" (中文)
 * title("unknown") → 返回注册时的原始title
 */
QString ControlRegistry::title(const QString &key) const
{
    // 【页面标题映射表】
    // 这些字符串都会被 Qt Linguist 工具提取到 .ts 翻译文件中
    // 注意: && 会被转义为单个 & (菜单加速键)
    
    // --- Basic Widgets 分组 ---
    if (key == "buttons") {
        return QObject::tr("Buttons");
    }
    if (key == "inputs") {
        return QObject::tr("Inputs");
    }
    if (key == "views") {
        return QObject::tr("Views");
    }
    
    // --- Data Display 分组 ---
    if (key == "table") {
        return QObject::tr("Tables");
    }
    if (key == "pagination") {
        return QObject::tr("Pagination");
    }
    
    // --- Menus & Actions 分组 ---
    if (key == "menuToolbar") {
        return QObject::tr("Menu && Toolbar");
    }
    
    // --- Dialogs & Feedback 分组 ---
    if (key == "dialogs") {
        return QObject::tr("Dialogs");
    }
    if (key == "feedback") {
        return QObject::tr("Dialogs && Tips");
    }
    
    // --- Graphics & Reports 分组 ---
    if (key == "graphics") {
        return QObject::tr("Graphics View");
    }
    if (key == "charts") {
        return QObject::tr("Charts");
    }
    
    // --- Style & Theme 分组 ---
    if (key == "style") {
        return QObject::tr("Style Controls");
    }
    
    // --- Advanced 分组 ---
    if (key == "containers") {
        return QObject::tr("Containers");
    }
    if (key == "advanced") {
        return QObject::tr("Advanced");
    }
    
    // 【兜底策略】如果上面都没匹配,返回注册时传入的原始title
    // 这种情况不应该发生,但作为防御性编程保留
    return m_entries.value(key).title;
}

/**
 * @brief 根据页面key创建页面实例(懒加载核心)
 * 
 * 【工作流程】
 * 1. 查找注册表中是否存在该key
 * 2. 如果存在,调用对应的工厂函数 factory()
 * 3. 工厂函数执行 new XxxPage(),返回新创建的页面对象
 * 
 * 【重要特性】
 * - 每次调用都会创建 NEW 实例(不是单例!)
 * - 所以 ContentStack 需要缓存已创建的页面,避免重复创建
 * - 这实现了真正的懒加载:只有用户点击导航项时才创建页面
 * 
 * 【内存管理】
 * C++ vs Java 对比:
 * - Java: new ButtonPage() → JVM垃圾回收器自动管理
 * - C++: new ButtonPage() → 需要手动delete或使用智能指针
 * - Qt方案: 采用父子对象机制
 *   * 页面被添加到QStackedWidget后,成为其子对象
 *   * 父对象销毁时自动delete所有子对象
 *   * 类似Java GC,但是确定性的(deterministic)
 * 
 * 【Java对比】
 * 类似于 Spring 的: applicationContext.getBean("buttonPage")
 * 或者工厂模式: PageFactory.createPage("buttons")
 * 
 * @param key 页面唯一标识
 * @return ControlPageBase* 新创建的页面对象指针
 * @throws std::runtime_error 如果key未在注册表中找到
 * 
 * 【调用示例】
 * // 用户点击"按钮"导航项
 * auto page = registry.create("buttons");  // 内部执行: new ButtonPage()
 * contentStack->addWidget(page);           // 添加到容器,建立父子关系
 * 
 * 【注意事项】
 * - 调用方不负责delete返回的指针(Qt父子机制接管)
 * - 如果key不存在会抛出异常,调用方需要捕获或确保key有效
 */
ControlPageBase *ControlRegistry::create(const QString &key) const
{
    // 【参数校验】防止访问未注册的页面
    if (!m_entries.contains(key)) {
        throw std::runtime_error(QString("Page not found: %1").arg(key).toStdString());
    }
    
    // 【工厂模式核心】
    // 调用存储的工厂函数,创建并返回新页面对象
    // factory 是一个 std::function,内部可能是:
    // []() { return new ButtonPage(); }
    // []() { return new TablePage(); }
    // 等等...
    // 
    // 这就是"控制反转(IoC)"的体现:
    // - ControlRegistry 不知道具体页面类型
    // - 它只知道调用 factory() 就能得到 ControlPageBase*
    // - 具体创建什么类型,由注册时的 lambda 决定
    return m_entries.value(key).factory();
}
