#include "ButtonPage.h"
#include "core/ExceptionHandler.h"
#include "core/Logger.h"

#include <QCheckBox>
#include <QGroupBox>
#include <QLabel>
#include <QPushButton>
#include <QRadioButton>
#include <QStyle>
#include <QToolButton>
#include <QVBoxLayout>

/**
 * @brief ButtonPage构造函数
 * 
 * 【页面结构】
 * QVBoxLayout (根布局)
 *   ├─ QGroupBox "Push Buttons" (按钮组)
 *   │   └─ QHBoxLayout
 *   │       ├─ QPushButton "Primary Action" (普通按钮)
 *   │       ├─ QPushButton "Toggle" (可切换按钮)
 *   │       └─ QToolButton "Tool" (工具按钮，带图标)
 *   ├─ QGroupBox "Options" (选项组)
 *   │   └─ QVBoxLayout
 *   │       ├─ QCheckBox "Enable extra option" (复选框)
 *   │       ├─ QRadioButton "Mode A" (单选按钮A，默认选中)
 *   │       └─ QRadioButton "Mode B" (单选按钮B)
 *   └─ QLabel (状态显示标签)
 * 
 * 【信号槽连接】
 * 所有控件的状态变化都连接到updateStatus()槽函数
 * 这样任何控件被操作时，状态标签都会实时更新
 * 
 * Java对比：类似于 JPanel + ActionListener的组合
 * 
 * @param parent 父窗口指针
 */
ButtonPage::ButtonPage(QWidget *parent)
    : ControlPageBase(parent)  // 调用基类构造函数
{
    // 【创建根布局】
    // QVBoxLayout: 垂直布局，控件从上到下排列
    QVBoxLayout *root = new QVBoxLayout(this);
    root->setContentsMargins(24, 24, 24, 24);  // 设置内边距：上下左右各24px

    // === 第一组：按钮组 ===
    m_pushGroup = new QGroupBox(this);  // 分组框容器
    QHBoxLayout *pushLayout = new QHBoxLayout(m_pushGroup);  // 水平布局
    
    // 创建三个按钮
    m_primaryButton = new QPushButton(this);  // 普通按钮
    
    m_toggleButton = new QPushButton(this);  // 可切换按钮
    m_toggleButton->setCheckable(true);      // 设置为可切换状态（类似复选框）
    
    m_toolButton = new QToolButton(this);  // 工具按钮
    // 设置标准图标：文件对话框详细视图图标
    m_toolButton->setIcon(style()->standardIcon(QStyle::SP_FileDialogDetailedView));
    
    // 将按钮添加到水平布局
    pushLayout->addWidget(m_primaryButton);
    pushLayout->addWidget(m_toggleButton);
    pushLayout->addWidget(m_toolButton);
    pushLayout->addStretch();  // 添加弹性空间，让按钮靠左对齐

    // === 第二组：选项组 ===
    m_optionGroup = new QGroupBox(this);  // 分组框容器
    QVBoxLayout *optionLayout = new QVBoxLayout(m_optionGroup);  // 垂直布局
    
    // 创建复选框和单选按钮
    m_checkBox = new QCheckBox(this);  // 复选框
    
    m_radioA = new QRadioButton(this);  // 单选按钮A
    m_radioB = new QRadioButton(this);  // 单选按钮B
    m_radioA->setChecked(true);         // 默认选中A
    
    // 将选项控件添加到垂直布局
    optionLayout->addWidget(m_checkBox);
    optionLayout->addWidget(m_radioA);
    optionLayout->addWidget(m_radioB);

    // === 状态显示标签 ===
    m_statusLabel = new QLabel(this);

    // 【组装根布局】
    root->addWidget(m_pushGroup);     // 添加按钮组
    root->addWidget(m_optionGroup);   // 添加选项组
    root->addWidget(m_statusLabel);   // 添加状态标签
    root->addStretch();               // 添加弹性空间，让内容靠上对齐

    // 【信号槽连接】
    // 所有控件的状态变化都连接到updateStatus()
    // Java对比：类似于 addActionListener(this) 或 addChangeListener(this)
    connect(m_primaryButton, &QPushButton::clicked, this, &ButtonPage::updateStatus);
    connect(m_toggleButton, &QPushButton::toggled, this, &ButtonPage::updateStatus);
    connect(m_toolButton, &QToolButton::clicked, this, &ButtonPage::updateStatus);
    connect(m_checkBox, &QCheckBox::toggled, this, &ButtonPage::updateStatus);
    connect(m_radioA, &QRadioButton::toggled, this, &ButtonPage::updateStatus);
    connect(m_radioB, &QRadioButton::toggled, this, &ButtonPage::updateStatus);

    // 【初始化UI文本和状态】
    retranslateUi();   // 设置初始文本（根据当前语言）
    updateStatus();    // 更新初始状态显示
    Logger::info("ButtonPage initialized");  // 记录日志
}

/**
 * @brief 返回页面名称
 * 
 * 【重要】使用tr()包裹字符串以支持国际化
 * 
 * @return QString "Buttons" -> "按钮"（根据当前语言）
 */
QString ButtonPage::pageName() const
{
    return tr("Buttons");
}

/**
 * @brief 重新翻译页面上的所有文本
 * 
 * 【调用时机】
 * 当用户切换语言时，MainWindow会调用此方法
 * 
 * 【工作原理】
 * tr()函数会根据当前加载的QTranslator返回对应语言的文本
 * - 英文环境：tr("Buttons") -> "Buttons"
 * - 中文环境：tr("Buttons") -> "按钮"
 * 
 * Java对比：类似于 ResourceBundle.getString("buttons")
 */
void ButtonPage::retranslateUi()
{
    // 设置分组框标题
    m_pushGroup->setTitle(tr("Push Buttons"));
    m_optionGroup->setTitle(tr("Options"));
    
    // 设置按钮文本
    m_primaryButton->setText(tr("Primary Action"));
    m_toggleButton->setText(tr("Toggle"));
    m_toolButton->setText(tr("Tool"));
    m_toolButton->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);  // 图标在左，文字在右
    
    // 设置选项文本
    m_checkBox->setText(tr("Enable extra option"));
    m_radioA->setText(tr("Mode A"));
    m_radioB->setText(tr("Mode B"));
    
    // 刷新状态显示（因为模式文本可能改变了）
    updateStatus();
}

/**
 * @brief 更新状态标签文本
 * 
 * 【功能说明】
 * 读取所有控件的当前状态，组合成一段描述性文本
 * 例如："Current state: toggle=on, option=checked, mode=Mode A"
 * 
 * 【异常处理】
 * 包裹try-catch，防止异常逃逸导致程序崩溃
 * 
 * 【字符串格式化】
 * 使用QString::arg()进行占位符替换，类似Java的String.format()
 * %1, %2, %3 是占位符，按顺序被后面的参数替换
 * 
 * Java对比：类似于 String.format("toggle=%s, option=%s, mode=%s", ...)
 */
void ButtonPage::updateStatus()
{
    try {
        // 获取当前选中的模式
        const QString mode = m_radioA->isChecked() ? tr("Mode A") : tr("Mode B");
        
        // 【字符串格式化】
        // tr("Current state: toggle=%1, option=%2, mode=%3") 是模板字符串
        // .arg() 方法依次替换 %1, %2, %3
        m_statusLabel->setText(tr("Current state: toggle=%1, option=%2, mode=%3")
                               .arg(m_toggleButton->isChecked() ? tr("on") : tr("off"))
                               .arg(m_checkBox->isChecked() ? tr("checked") : tr("unchecked"))
                               .arg(mode));
    } catch (const std::exception &e) {
        // 【异常处理】统一交给ExceptionHandler处理
        ExceptionHandler::handle(e);
    }
}
