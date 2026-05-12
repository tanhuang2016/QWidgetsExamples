#pragma once

#include <QMainWindow>

class QAction;
class ContentStack;
class NavigationPanel;

/**
 * @brief 应用主窗口（外壳UI）
 * 
 * 【职责说明】
 * MainWindow负责整个应用的外壳UI，包括：
 * 1. 菜单栏（MenuBar）：语言切换等全局操作
 * 2. 左侧导航树（NavigationPanel）：显示所有可用页面
 * 3. 右侧内容栈（ContentStack）：显示当前选中的页面
 * 
 * 【不包含的内容】
 * 真正的控件示例都在 src/pages 目录下的页面类中
 * MainWindow只负责框架结构，不关心具体页面的实现细节
 * 
 * Java对比：
 * - 类似于JavaFX的 PrimaryStage + BorderPane布局
 * - 或者Spring Boot应用的 Shell UI（侧边栏 + 主内容区）
 */
class MainWindow : public QMainWindow {
    Q_OBJECT  // Qt宏，启用信号槽机制（类似Java的事件监听器基础设施）

public:
    /**
     * @brief 构造函数
     * 
     * @param parent 父窗口指针，通常为nullptr（主窗口没有父窗口）
     */
    explicit MainWindow(QWidget *parent = nullptr);

protected:
    /**
     * @brief 处理Qt的LanguageChange事件
     * 
     * 【重要】当调用 QTranslator::load() 加载新语言时
     * Qt会自动向所有widget发送 LanguageChange 事件
     * 
     * Java对比：类似于 ResourceBundle 重新加载后刷新UI
     * 
     * @param event Qt事件对象
     */
    void changeEvent(QEvent *event) override;

private slots:
    /** 切换到英文界面 */
    void switchToEnglish();
    
    /** 切换到中文界面 */
    void switchToChinese();
    
    /**
     * @brief 重新翻译UI文本
     * 
     * 【调用时机】
     * 1. 用户通过菜单切换语言时
     * 2. Qt发送LanguageChange事件时
     * 
     * 这个方法会递归调用所有子widget的retranslateUi()
     */
    void retranslateUi();

private:
    /** 
     * @brief 构建中央分割区域：左侧导航，右侧内容
     * 
     * 使用QSplitter实现可拖拽调整宽度的左右分栏
     */
    void buildUi();
    
    /** 
     * @brief 构建应用菜单栏
     * 
     * 创建 "Language" 菜单，包含 English / Chinese 两个选项
     */
    void buildMenus();
    
    /** 
     * @brief 应用接近IDE工具窗口风格的紧凑样式
     * 
     * 使用QSS（Qt Style Sheets）设置全局样式
     * Java对比：类似于CSS样式表或JavaFX的CSS
     */
    void applyStyle();

    NavigationPanel *m_navigation;  // 左侧导航树
    ContentStack *m_content;        // 右侧内容栈
    QMenu *m_languageMenu;          // 语言菜单
    QAction *m_englishAction;       // 英文选项
    QAction *m_chineseAction;       // 中文选项
};
