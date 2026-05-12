#include "ControlRegistry.h"
#include "Logger.h"
#include "ui/ControlPageBase.h"

#include <QObject>
#include <stdexcept>

void ControlRegistry::registerPage(const QString &key, const QString &title, const Factory &factory)
{
    registerPage("basic", key, title, factory);
}

void ControlRegistry::registerPage(const QString &group, const QString &key, const QString &title, const Factory &factory)
{
    if (key.trimmed().isEmpty() || !factory) {
        throw std::runtime_error("Invalid page registration.");
    }

    // Keep explicit order lists because QHash is intentionally unordered.
    if (!m_groupOrder.contains(group)) {
        m_groupOrder.append(group);
    }
    if (!m_entries.contains(key)) {
        m_order.append(key);
    }
    m_entries.insert(key, Entry{group, title, factory});
    Logger::info(QString("Registered page: %1").arg(key));
}

QStringList ControlRegistry::keys() const
{
    QStringList result;
    for (const QString &key : m_order) {
        result.append(key);
    }
    return result;
}

QStringList ControlRegistry::groups() const
{
    QStringList result;
    for (const QString &group : m_groupOrder) {
        result.append(group);
    }
    return result;
}

QStringList ControlRegistry::keysInGroup(const QString &group) const
{
    QStringList result;
    for (const QString &key : m_order) {
        if (m_entries.value(key).group == group) {
            result.append(key);
        }
    }
    return result;
}

QString ControlRegistry::groupTitle(const QString &group) const
{
    if (group == "basic") {
        return QObject::tr("Basic Widgets");
    }
    if (group == "data") {
        return QObject::tr("Data Display");
    }
    if (group == "actions") {
        return QObject::tr("Menus && Actions");
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
    return group;
}

QString ControlRegistry::groupOf(const QString &key) const
{
    return m_entries.value(key).group;
}

QString ControlRegistry::title(const QString &key) const
{
    // The registry stores fallback titles, but this switch-like mapping keeps
    // visible titles translated even after runtime language changes. We avoid
    // using it for page creation; creation still uses the registered factory.
    if (key == "buttons") {
        return QObject::tr("Buttons");
    }
    if (key == "inputs") {
        return QObject::tr("Inputs");
    }
    if (key == "views") {
        return QObject::tr("Views");
    }
    if (key == "dialogs") {
        return QObject::tr("Dialogs");
    }
    if (key == "layouts") {
        return QObject::tr("Layouts");
    }
    if (key == "table") {
        return QObject::tr("Tables");
    }
    if (key == "pagination") {
        return QObject::tr("Pagination");
    }
    if (key == "menuToolbar") {
        return QObject::tr("Menu && Toolbar");
    }
    if (key == "feedback") {
        return QObject::tr("Dialogs && Tips");
    }
    if (key == "graphics") {
        return QObject::tr("Graphics View");
    }
    if (key == "charts") {
        return QObject::tr("Charts");
    }
    if (key == "style") {
        return QObject::tr("Style Controls");
    }
    if (key == "containers") {
        return QObject::tr("Containers");
    }
    if (key == "advanced") {
        return QObject::tr("Advanced");
    }
    return m_entries.value(key).title;
}

ControlPageBase *ControlRegistry::create(const QString &key) const
{
    if (!m_entries.contains(key)) {
        throw std::runtime_error(QString("Page not found: %1").arg(key).toStdString());
    }
    // The factory hides the concrete C++ type from the caller.
    return m_entries.value(key).factory();
}
