#pragma once

#include "ui/ControlPageBase.h"

class QAction;
class QMainWindow;
class QTextEdit;
class QToolBar;
class QMenu;

class MenuToolbarPage : public ControlPageBase {
    Q_OBJECT

public:
    explicit MenuToolbarPage(QWidget *parent = nullptr);
    QString pageName() const override;

public slots:
    void retranslateUi() override;

private slots:
    void addRecentFile();
    void toggleSaveEnabled(bool enabled);
    void showContextMenu(const QPoint &pos);
    void logAction();

private:
    QAction *createAction(const QIcon &icon, const QString &text, const QKeySequence &shortcut);
    void rebuildRecentMenu();
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
