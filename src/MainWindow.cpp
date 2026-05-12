#include "MainWindow.h"
#include "core/AppContext.h"
#include "core/ExceptionHandler.h"
#include "i18n/TranslationManager.h"
#include "ui/ContentStack.h"
#include "ui/NavigationPanel.h"

#include <QAction>
#include <QApplication>
#include <QEvent>
#include <QHBoxLayout>
#include <QMenu>
#include <QMenuBar>
#include <QSplitter>

/**
 * @brief MainWindow构造函数
 * 
 * 【初始化顺序】
 * 1. buildUi() - 构建UI结构（导航树 + 内容栈）
 * 2. buildMenus() - 构建菜单栏
 * 3. applyStyle() - 应用全局样式
 * 4. retranslateUi() - 设置初始文本（根据当前语言）
 * 
 * Java对比：类似于 @PostConstruct 或构造后的初始化方法
 */
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),  // 调用父类QMainWindow的构造函数
      m_navigation(nullptr), // 初始化指针为nullptr（避免悬空指针）
      m_content(nullptr),
      m_languageMenu(nullptr),
      m_englishAction(nullptr),
      m_chineseAction(nullptr)
{
    // 按顺序初始化UI组件
    buildUi();
    buildMenus();
    applyStyle();
    retranslateUi();

    // 【信号槽连接】
    // 当TranslationManager发出languageChanged信号时，自动调用retranslateUi()
    // Java对比：类似于 eventBus.subscribe("languageChanged", this::retranslateUi)
    connect(&AppContext::instance().translations(), &TranslationManager::languageChanged,
            this, &MainWindow::retranslateUi);
}

/**
 * @brief 处理Qt事件
 * 
 * 【重要】这是Qt的事件分发机制
 * 当语言切换时，Qt会发送QEvent::LanguageChange事件
 * 我们需要重写这个方法来响应语言变化
 * 
 * Java对比：类似于 Swing的 processEvent() 或 JavaFX的 EventDispatcher
 */
void MainWindow::changeEvent(QEvent *event)
{
    // 检查是否为语言变化事件
    if (event->type() == QEvent::LanguageChange) {
        retranslateUi();  // 重新翻译UI文本
    }
    // 调用父类的默认处理（保证其他事件正常处理）
    QMainWindow::changeEvent(event);
}

/**
 * @brief 切换到英文界面
 * 
 * 【异常处理】
 * 所有UI操作都包裹try-catch，防止异常逃逸导致程序崩溃
 */
void MainWindow::switchToEnglish()
{
    try {
        AppContext::instance().translations().switchLanguage("en_US");
    } catch (const std::exception &e) {
        ExceptionHandler::handle(e);
    }
}

/**
 * @brief 切换到中文界面
 */
void MainWindow::switchToChinese()
{
    try {
        AppContext::instance().translations().switchLanguage("zh_CN");
    } catch (const std::exception &e) {
        ExceptionHandler::handle(e);
    }
}

/**
 * @brief 重新翻译UI文本
 * 
 * 【工作原理】
 * tr() 是Qt的国际化函数，会根据当前加载的QTranslator返回对应语言的文本
 * 
 * Java对比：类似于 ResourceBundle.getString("key")
 * 
 * 注意：这里只翻译MainWindow自己的文本
 * 子widget的翻译由它们自己的retranslateUi()方法处理
 */
void MainWindow::retranslateUi()
{
    setWindowTitle(tr("Qt Widget Gallery"));  // 设置窗口标题
    
    // 翻译语言菜单
    if (m_languageMenu) {
        m_languageMenu->setTitle(tr("Language"));
        m_englishAction->setText(tr("English"));
        m_chineseAction->setText(tr("Chinese"));
    }
    
    // 递归调用子widget的翻译方法
    if (m_navigation) {
        m_navigation->retranslateUi();
    }
    if (m_content) {
        m_content->retranslatePages();
    }
}

/**
 * @brief 构建中央UI布局
 * 
 * 【布局结构】
 * QMainWindow (主窗口)
 *   └─ CentralWidget (中央容器)
 *       └─ QHBoxLayout (水平布局)
 *           └─ QSplitter (可拖拽分割器)
 *               ├─ NavigationPanel (左侧导航树，固定宽度)
 *               └─ ContentStack (右侧内容栈，自适应拉伸)
 * 
 * Java对比：类似于 BorderPane.setLeft() + BorderPane.setCenter()
 */
void MainWindow::buildUi()
{
    // 创建中央容器widget
    QWidget *central = new QWidget(this);
    
    // 创建水平布局（左右排列）
    QHBoxLayout *layout = new QHBoxLayout(central);
    layout->setContentsMargins(0, 0, 0, 0);  // 去除边距，让内容填满窗口

    // 创建分割器（支持用户拖拽调整左右宽度）
    QSplitter *splitter = new QSplitter(Qt::Horizontal, central);
    
    // 创建左侧导航面板和右侧内容栈
    m_navigation = new NavigationPanel(splitter);
    m_content = new ContentStack(splitter);

    // 将两个widget添加到分割器
    splitter->addWidget(m_navigation);
    splitter->addWidget(m_content);
    
    // 【关键】设置伸缩因子
    // 0: 左侧导航不拉伸（保持固定宽度）
    // 1: 右侧内容区自动拉伸填充剩余空间
    splitter->setStretchFactor(0, 0);
    splitter->setStretchFactor(1, 1);
    
    layout->addWidget(splitter);
    setCentralWidget(central);  // 设置为QMainWindow的中央widget

    // 【信号槽连接】
    // 当用户点击导航树的某个页面时，ContentStack显示对应的页面
    // Java对比：类似于 navigation.onSelect(key -> content.showPage(key))
    connect(m_navigation, &NavigationPanel::pageSelected,
            m_content, &ContentStack::showPage);
    
    // 从注册表获取所有页面key，并设置到导航树
    m_navigation->setPages(AppContext::instance().registry().keys());
}

/**
 * @brief 构建菜单栏
 * 
 * 【菜单结构】
 * MenuBar
 *   └─ Language (菜单)
 *       ├─ English (动作)
 *       └─ Chinese (动作)
 * 
 * Java对比：类似于 JMenuBar + JMenu + JMenuItem
 */
void MainWindow::buildMenus()
{
    // 创建"Language"菜单（初始文本为空，稍后由retranslateUi()设置）
    m_languageMenu = menuBar()->addMenu(QString());
    
    // 添加"English"和"Chinese"两个动作
    // QAction是Qt的命令抽象，可以关联到菜单项、工具栏按钮、快捷键等
    m_englishAction = m_languageMenu->addAction(QString(), this, &MainWindow::switchToEnglish);
    m_chineseAction = m_languageMenu->addAction(QString(), this, &MainWindow::switchToChinese);
}

/**
 * @brief 应用全局样式（QSS）
 * 
 * 【QSS说明】
 * Qt Style Sheets类似于CSS，用于美化UI控件
 * 语法与CSS非常相似，但选择器略有不同
 * 
 * Java对比：类似于JavaFX的CSS样式表
 * 
 * 样式说明：
 * - QMainWindow: 背景色 #f5f6f8（浅灰色）
 * - QTreeWidget: 白色背景，右侧边框，14px字体
 * - QTreeWidget::item: 每个选项高度36px，左内边距12px
 * - QTreeWidget::item:selected: 选中时蓝色背景+白色文字
 * - QGroupBox: 加粗标题，圆角边框
 * - QPushButton: 最小高度28px
 * - 输入控件: 统一边框和内边距
 */
void MainWindow::applyStyle()
{
    qApp->setStyleSheet(
        "QMainWindow { background: #f5f6f8; }"
        "QTreeWidget { background: #ffffff; border: 0; border-right: 1px solid #d6d9df; font-size: 14px; }"
        "QTreeWidget::item { height: 36px; padding-left: 12px; }"
        "QTreeWidget::item:selected { background: #2f6fed; color: white; }"
        "QGroupBox { font-weight: 600; border: 1px solid #d6d9df; border-radius: 4px; margin-top: 12px; padding: 10px; }"
        "QGroupBox::title { subcontrol-origin: margin; left: 8px; padding: 0 4px; }"
        "QPushButton { min-height: 28px; padding: 4px 12px; }"
        "QLineEdit, QComboBox, QSpinBox, QDoubleSpinBox, QTextEdit, QTableWidget, QListWidget { border: 1px solid #c9ced6; border-radius: 3px; padding: 4px; background: white; }"
    );
}
