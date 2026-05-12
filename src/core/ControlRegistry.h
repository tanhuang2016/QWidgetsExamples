#pragma once

#include <QString>
#include <QStringList>
#include <QHash>
#include <QList>
#include <functional>

class ControlPageBase;

/**
 * @brief 示例页面注册表。
 *
 * 这个类把“导航系统”和“具体页面类”解耦。MainWindow / ContentStack
 * 只需要知道页面 key，例如 "table"、"charts"，不需要写 switch-case，
 * 也不需要直接包含每个页面的具体类型。
 */
class ControlRegistry {
public:
    /**
     * @brief 页面工厂函数类型，用于懒加载创建页面。
     */
    typedef std::function<ControlPageBase *()> Factory;

    /**
     * @brief 把页面注册到默认 basic 分组。
     */
    void registerPage(const QString &key, const QString &title, const Factory &factory);

    /**
     * @brief 把页面注册到指定导航分组。
     *
     * @param group 逻辑分组 key，例如 "data" 或 "graphics"。
     * @param key 页面唯一 key。
     * @param title 兜底标题，实际显示时 title() 可能返回翻译后的标题。
     * @param factory 按需创建页面的函数。
     */
    void registerPage(const QString &group, const QString &key, const QString &title, const Factory &factory);

    /**
     * @brief 按注册顺序返回所有页面 key。
     */
    QStringList keys() const;

    /**
     * @brief 按注册顺序返回所有分组 key。
     */
    QStringList groups() const;

    /**
     * @brief 返回某个分组下的页面 key。
     */
    QStringList keysInGroup(const QString &group) const;

    /**
     * @brief 返回分组的翻译后显示标题。
     */
    QString groupTitle(const QString &group) const;

    /**
     * @brief 根据页面 key 返回所属分组 key。
     */
    QString groupOf(const QString &key) const;

    /**
     * @brief 返回页面的翻译后显示标题。
     */
    QString title(const QString &key) const;

    /**
     * @brief 根据页面 key 创建页面。
     *
     * 如果 key 不存在会抛出 std::runtime_error。返回的 QWidget 插入容器后，
     * 会进入 Qt 的父子对象内存管理体系。
     */
    ControlPageBase *create(const QString &key) const;

private:
    struct Entry {
        QString group;
        QString title;
        Factory factory;
    };

    QList<QString> m_order;
    QList<QString> m_groupOrder;
    QHash<QString, Entry> m_entries;
};
