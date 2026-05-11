#pragma once

#include <QString>
#include <QStringList>
#include <QHash>
#include <QList>
#include <functional>

class ControlPageBase;

class ControlRegistry {
public:
    typedef std::function<ControlPageBase *()> Factory;

    void registerPage(const QString &key, const QString &title, const Factory &factory);
    void registerPage(const QString &group, const QString &key, const QString &title, const Factory &factory);
    QStringList keys() const;
    QStringList groups() const;
    QStringList keysInGroup(const QString &group) const;
    QString groupTitle(const QString &group) const;
    QString groupOf(const QString &key) const;
    QString title(const QString &key) const;
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
