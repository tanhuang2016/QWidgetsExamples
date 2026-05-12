#pragma once

#include "ui/ControlPageBase.h"

class QAction;
class QMainWindow;
class QTextEdit;
class QToolBar;
class QMenu;

/**
 * @brief QAction、QMenuBar、QToolBar、QStatusBar 示例页。
 *
 * 这个页面的核心是 QAction：同一个 QAction 可以同时出现在菜单、工具栏、
 * 右键菜单中，并共享图标、快捷键、启用状态和 triggered 信号。
 */
class MenuToolbarPage : public ControlPageBase {
    Q_OBJECT

public:
    explicit MenuToolbarPage(QWidget *parent = nullptr);
    /** 返回翻译后的页面标题。 */
    QString pageName() const override;

public slots:
    /** 刷新菜单和 Action 的显示文本。 */
    void retranslateUi() override;

private slots:
    /** 向“最近文件”子菜单动态添加一项。 */
    void addRecentFile();
    /** 演示通过复选框改变 QAction 的启用状态。 */
    void toggleSaveEnabled(bool enabled);
    /** 打开一个复用现有 QAction 对象的右键菜单。 */
    void showContextMenu(const QPoint &pos);
    /** 记录哪个 Action 被触发。 */
    void logAction();

private:
    /** 创建统一配置的 QAction。 */
    QAction *createAction(const QIcon &icon, const QString &text, const QKeySequence &shortcut);
    /** 根据 m_recentFiles 重建“最近文件”子菜单。 */
    void rebuildRecentMenu();
    /** 追加界面日志，并写入文件日志。 */
    void appendLog(const QString &message);

    QMainWindow *m_demoWindow;
    QTextEdit *m_log;
    QMenu *m_fileMenu;
    QMenu *m_recentMenu;
    QMenu *m_viewMenu;
    QToolBar *m_toolbar;
    QAction *m_newAction;
    QAction *m_openAction;
    QAction *m_saveAction;
    QAction *m_boldAction;
    QAction *m_lightAction;
    QAction *m_darkAction;
    QStringList m_recentFiles;
};
