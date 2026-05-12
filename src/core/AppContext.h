#pragma once

#include "ControlRegistry.h"

class QApplication;
class TranslationManager;

/**
 * @brief 应用级上下文对象（全局单例）
 * 
 * 【架构设计说明】
 * Java对比：
 * - 类似于Spring的 ApplicationContext（IoC容器）
 * - 或者单例模式的 Configuration Manager
 * 
 * 为什么需要AppContext？
 * - QApplication是Qt框架的核心类，不应该被业务代码污染
 * - AppContext集中管理本应用的共享服务，避免全局变量散乱
 * 
 * 管理的服务：
 * 1. ControlRegistry：页面注册表（类似路由表）
 *    - 记录有哪些示例页面
 *    - 提供页面创建的工厂方法
 * 
 * 2. TranslationManager：国际化管理器
 *    - 运行时切换语言（中文/英文）
 *    - 无需重启程序即可生效
 * 
 * 【单例模式实现】
 * 使用Meyers Singleton（C++11线程安全的局部静态变量）
 * Java对比：类似于 enum singleton 或 static inner class singleton
 */
class AppContext {
public:
    /**
     * @brief 返回唯一的全局AppContext实例
     * 
     * Java对比：类似于 AppConfig.getInstance()
     * 
     * 调用方式：AppContext::instance().registry()
     * 注意：C++使用 :: 而不是 . 来访问静态方法
     */
    static AppContext &instance();

    /**
     * @brief 初始化依赖QApplication的服务
     *
     * 【重要】必须在main()中创建QApplication之后调用一次
     * 
     * Java对比：类似于 @PostConstruct 或 InitializingBean.afterPropertiesSet()
     * 
     * @param app QApplication指针，用于安装翻译器等
     */
    void initialize(QApplication *app);

    /**
     * @brief 获取页面注册表
     * 
     * Java对比：类似于 router.getRoutes()
     * 
     * @return ControlRegistry引用，用于注册和查询页面
     */
    ControlRegistry &registry();

    /**
     * @brief 获取运行时国际化管理器
     * 
     * Java对比：类似于 i18nManager 或 messageSource
     * 
     * @return TranslationManager引用，用于切换语言
     */
    TranslationManager &translations();

private:
    // 私有构造函数，防止外部直接创建实例（单例模式标准写法）
    AppContext();

    /**
     * @brief 注册所有可用的示例页面
     *
     * 【懒加载机制】
     * 这里只注册工厂函数，不会立即创建页面对象
     * 用户点击导航项时才会真正创建页面实例
     * 
     * Java对比：类似于在配置类中注册 @Bean，但实际创建是lazy的
     * 
     * 每个页面注册包含：
     * - group: 分组key（如"basic"、"data"、"graphics"）
     * - key: 页面唯一标识（如"button"、"table"）
     * - title: 显示标题（支持国际化）
     * - factory: 创建页面的lambda函数（工厂模式）
     */
    void registerPages();

    ControlRegistry m_registry;        // 页面注册表
    TranslationManager *m_translations; // 国际化管理器指针
    bool m_initialized;                 // 初始化标志，防止重复初始化
};
