#pragma once

#include <QMainWindow>

class QAction;
class ContentStack;
class NavigationPanel;

/**
 * @brief 应用主窗口。
 *
 * 主窗口负责外壳 UI：菜单栏、左侧导航树、右侧内容栈。真正的控件示例
 * 都在 src/pages 目录下的页面类中。
 */
class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);

protected:
    /**
     * @brief 处理 Qt 的 LanguageChange 事件。
     */
    void changeEvent(QEvent *event) override;

private slots:
    void switchToEnglish();
    void switchToChinese();
    void retranslateUi();

private:
    /** 构建中央分割区域：左侧导航，右侧内容。 */
    void buildUi();
    /** 构建应用菜单栏。 */
    void buildMenus();
    /** 应用接近 IDE 工具窗口风格的紧凑样式。 */
    void applyStyle();

    NavigationPanel *m_navigation;
    ContentStack *m_content;
    QMenu *m_languageMenu;
    QAction *m_englishAction;
    QAction *m_chineseAction;
};
