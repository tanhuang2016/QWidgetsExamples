#pragma once

#include "ControlRegistry.h"

class QApplication;
class TranslationManager;

/**
 * @brief 应用级上下文对象。
 *
 * Qt 已经有 QApplication 作为全局应用对象，但不建议把业务服务都塞进
 * QApplication。AppContext 用来集中管理本示例程序的共享服务：
 *
 * - ControlRegistry：记录有哪些示例页面，以及如何创建这些页面。
 * - TranslationManager：负责运行时切换语言。
 *
 * 这里使用 Meyers Singleton 写法，适合这个学习型桌面程序。更大的项目
 * 可以考虑依赖注入，减少全局单例。
 */
class AppContext {
public:
    /**
     * @brief 返回唯一的全局 AppContext 实例。
     */
    static AppContext &instance();

    /**
     * @brief 初始化依赖 QApplication 的服务。
     *
     * 必须在 main() 中创建 QApplication 之后调用一次。
     */
    void initialize(QApplication *app);

    /**
     * @brief 获取页面注册表。
     */
    ControlRegistry &registry();

    /**
     * @brief 获取运行时国际化管理器。
     */
    TranslationManager &translations();

private:
    AppContext();

    /**
     * @brief 注册所有可用的示例页面。
     *
     * 每个页面注册时包含：分组、唯一 key、显示标题、创建页面的工厂函数。
     * 注意这里不会立即创建页面对象，而是等用户点击导航项时再懒加载。
     */
    void registerPages();

    ControlRegistry m_registry;
    TranslationManager *m_translations;
    bool m_initialized;
};
