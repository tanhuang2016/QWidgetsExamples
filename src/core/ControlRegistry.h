#pragma once

#include <QString>
#include <QStringList>
#include <QHash>
#include <QList>
#include <functional>

class ControlPageBase;

/**
 * @brief 示例页面注册表（路由中心）
 * 
 * 【设计模式】
 * - 工厂模式：通过工厂函数创建页面实例
 * - 注册表模式：集中管理所有可访问的页面
 * 
 * Java对比：
 * - 类似于Spring MVC的 @RequestMapping 注册中心
 * - 或者类似 RouterRegistry，记录URL到Controller的映射
 * 
 * 核心优势：
 * 1. 解耦：MainWindow/ContentStack不需要知道具体页面类
 * 2. 可扩展：新增页面只需注册，无需修改现有代码（开闭原则）
 * 3. 懒加载：页面只在首次访问时创建，节省内存
 * 
 * 使用流程：
 * 1. AppContext::registerPages() 中注册所有页面
 * 2. NavigationPanel 从 registry.keys() 获取页面列表并显示
 * 3. 用户点击导航项时，ContentStack 调用 registry.create(key) 创建页面
 */
class ControlRegistry {
public:
    /**
     * @brief 页面工厂函数类型
     * 
     * Java对比：类似于 Supplier<ControlPageBase> 或 FactoryBean
     * 
     * std::function 是C++的函数对象包装器，可以存储：
     * - 普通函数指针
     * - Lambda表达式
     * - std::bind绑定结果
     * - 仿函数（重载operator()的对象）
     */
    typedef std::function<ControlPageBase *()> Factory;

    /**
     * @brief 把页面注册到默认 "basic" 分组
     * 
     * @param key 页面唯一标识（如 "button"、"input"）
     * @param title 页面显示标题（支持国际化）
     * @param factory 创建页面的工厂函数
     */
    void registerPage(const QString &key, const QString &title, const Factory &factory);

    /**
     * @brief 把页面注册到指定导航分组
     *
     * @param group 逻辑分组key，例如：
     *              - "basic": 基础控件
     *              - "data": 数据视图
     *              - "graphics": 图形绘制
     *              - "dialogs": 对话框
     * @param key 页面唯一标识
     * @param title 兜底标题（实际显示时可能返回翻译后的标题）
     * @param factory 按需创建页面的函数
     */
    void registerPage(const QString &group, const QString &key, const QString &title, const Factory &factory);

    /**
     * @brief 按注册顺序返回所有页面key
     * 
     * Java对比：类似于 router.getRouteKeys()
     * 
     * @return QStringList 所有页面的key列表，用于构建导航树
     */
    QStringList keys() const;

    /**
     * @brief 按注册顺序返回所有分组key
     * 
     * @return QStringList 分组列表，如 ["basic", "data", "graphics"]
     */
    QStringList groups() const;

    /**
     * @brief 返回某个分组下的页面key
     * 
     * @param group 分组名称
     * @return QStringList 该分组下的所有页面key
     */
    QStringList keysInGroup(const QString &group) const;

    /**
     * @brief 返回分组的翻译后显示标题
     * 
     * @param group 分组key
     * @return QString 翻译后的分组标题，如 "Basic Widgets" -> "基础控件"
     */
    QString groupTitle(const QString &group) const;

    /**
     * @brief 根据页面key返回所属分组key
     * 
     * @param key 页面key
     * @return QString 分组key
     */
    QString groupOf(const QString &key) const;

    /**
     * @brief 返回页面的翻译后显示标题
     * 
     * @param key 页面key
     * @return QString 翻译后的页面标题
     */
    QString title(const QString &key) const;

    /**
     * @brief 根据页面key创建页面实例
     *
     * 【重要】这是懒加载的核心方法
     * - 第一次调用：执行factory()创建新页面
     * - 后续调用：会再次创建新页面（所以ContentStack需要缓存）
     * 
     * Java对比：类似于 applicationContext.getBean("beanName")
     * 
     * @param key 页面唯一标识
     * @return ControlPageBase* 新创建的页面对象指针
     * @throws std::runtime_error 如果key不存在
     * 
     * 内存管理：
     * - 返回的QWidget插入容器后，会进入Qt的父子对象内存管理体系
     * - 父对象销毁时会自动delete子对象（类似Java的GC，但更 deterministic）
     */
    ControlPageBase *create(const QString &key) const;

private:
    // 内部结构体：存储每个页面的元信息
    struct Entry {
        QString group;   // 所属分组
        QString title;   // 显示标题
        Factory factory; // 工厂函数
    };

    QList<QString> m_order;           // 页面注册顺序（保持插入顺序）
    QList<QString> m_groupOrder;      // 分组注册顺序
    QHash<QString, Entry> m_entries;  // key -> Entry 的映射表
};
