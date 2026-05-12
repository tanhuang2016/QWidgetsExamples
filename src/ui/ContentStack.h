#pragma once

#include <QHash>
#include <QStackedWidget>

class ControlPageBase;

/**
 * @brief 右侧页面容器，支持懒加载。
 *
 * 它继承自 QStackedWidget，但不会在启动时创建所有页面。用户第一次点击
 * 某个页面 key 时，ContentStack 才会通过 ControlRegistry 创建页面，
 * 并缓存起来供后续复用。
 */
class ContentStack : public QStackedWidget {
    Q_OBJECT

public:
    explicit ContentStack(QWidget *parent = nullptr);

    /**
     * @brief 根据注册表 key 显示页面，必要时创建页面。
     */
    void showPage(const QString &key);

public slots:
    /**
     * @brief 对已经创建的页面调用 retranslateUi()。
     */
    void retranslatePages();

private:
    /**
     * @brief 返回已缓存页面，或通过 ControlRegistry 创建新页面。
     */
    ControlPageBase *loadPage(const QString &key);

    /**
     * @brief 页面加载失败时显示的兜底错误页。
     */
    QWidget *createErrorPage(const QString &message);

    QHash<QString, ControlPageBase *> m_pages;
};
