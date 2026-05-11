# Qt 控件大全展示框架（工程级 Codex Prompt v2）

## 项目目标

实现基于 **Qt 5\.15 Widgets \+ C\+\+** 的桌面应用：

- 对标 Widget Gallery / Toolbox Demo

- 具备**工程级可扩展、可维护、可调试**架构

## 一、核心架构要求（严格分层）

### 推荐目录结构

```Plain Text
/src
  main.cpp
  MainWindow.h / .cpp

  /core
    AppContext.h/.cpp
    ExceptionHandler.h/.cpp
    Logger.h/.cpp
    ControlRegistry.h/.cpp

  /ui
    ControlPageBase.h
    NavigationPanel.h/.cpp
    ContentStack.h/.cpp

  /pages
    ButtonPage
    InputPage
    ViewPage
    DialogPage
    LayoutPage
    ModelViewPage
    GraphicsPage
    ...

  /i18n
    TranslationManager.h/.cpp

/resources
  translations/
    zh_CN.qm
    en_US.qm

/logs
  app.log
```

## 二、核心设计要求

### 1\. 控件展示模块化架构

基类定义：

```cpp
class ControlPageBase : public QWidget {
    Q_OBJECT
public:
    explicit ControlPageBase(QWidget *parent = nullptr);
    virtual QString pageName() const = 0;
};
```

### 2\. 页面注册机制

- 动态注册，禁止写死 `switch\-case`

- 所有页面支持可注册扩展

- 支持懒加载：点击才创建页面

注册示例：

```cpp
ControlRegistry::registerPage("Button", []() {
    return new ButtonPage();
});
```

### 3\. UI 结构

- 左侧：`QTreeWidget / QListWidget` 导航栏

- 右侧：`QStackedWidget` 页面容器

## 三、国际化（必做）

### 支持语言

- 中文：zh\_CN

- 英文：en\_US

### 要求

- 所有 UI 文本统一用 `tr\(\)`

- `QTranslator` 动态加载翻译

- 菜单切换语言，**无需重启程序**

- 菜单层级：Language → English / 中文

## 四、异常处理机制

### 1\. 全局异常捕获

`main\.cpp` 全局安装：

```cpp
qInstallMessageHandler(Logger::messageHandler);
```

### 2\. UI 事件统一保护

所有 UI slot 必须包裹：

```cpp
try {
    // UI logic
} catch (const std::exception &e) {
    ExceptionHandler::handle(e);
}
```

### 3\. ExceptionHandler 功能

- 捕获异常信息

- 自动写入日志文件

- 弹窗友好提示用户

- 避免程序直接崩溃

## 五、日志系统

### 日志等级枚举

```cpp
enum LogLevel {
    Debug,
    Info,
    Warning,
    Error,
    Fatal
};
```

### 日志格式

`\[2026\-05\-11 12:00:00\] \[INFO\] ButtonPage initialized`

### 功能要求

- 输出文件：`logs/app\.log`

- Debug 模式同步控制台打印

- 支持日志自动滚动（按天 / 按文件大小分割）

- 调用接口：

```cpp
Logger::info("Application started");
Logger::error("Failed to load page");
```

## 六、异常 \+ 日志联动

- ExceptionHandler 捕获异常 → 自动落日志

- UI 层只调用，不处理底层细节

示例模板：

```cpp
catch (...) {
    Logger::error("Unknown exception in ButtonPage");
    ExceptionHandler::show("Unexpected error occurred");
}
```

## 七、UI 可靠性要求

- 全局异常兜底，保证 UI 不崩溃

- 单页面加载失败，不影响主窗口运行

- 内置 fallback 错误兜底页面机制

## 八、工程级增强要求

### 1\. 懒加载 Lazy Loading

- 页面首次点击导航才实例化

- 已创建页面全局缓存，复用不重复创建

### 2\. AppContext 全局上下文

统一全局共享实例：

- Logger

- Translator

- ControlRegistry

- 配置 Config

### 3\. Config 配置（可选推荐）

- 记录最近打开页面

- 保存语言选择配置

- 持久化 UI 窗口布局状态

## 九、UI 规范要求

- 左侧导航固定宽度

- 右侧内容区域自适应拉伸

- 界面风格简洁，对标 IDE 工具窗口

- **全程不使用 Qt Designer \.ui 文件**，纯代码手写 UI

## 十、输出交付要求

1. 输出完整 Qt C\+\+ CMake 工程

2. 可直接编译、一键运行

3. 严格匹配目录与架构分层

4. 每个控件页面附带最小可用示例

5. Logger、异常处理、国际化全部可运行

## 十一、控件与功能范围补充（V3 待确认清单）

> 本节用于确认最终开发范围。确认前只维护需求文档，不进入编码实现。

### 1. 基础控件页 Basic Widgets

- 文本显示：`QLabel`、富文本 QLabel、超链接 QLabel、图片 QLabel
- 按钮类：`QPushButton`、`QToolButton`、`QRadioButton`、`QCheckBox`、`QCommandLinkButton`
- 输入类：`QLineEdit`、`QTextEdit`、`QPlainTextEdit`、`QSpinBox`、`QDoubleSpinBox`
- 日期时间：`QDateEdit`、`QTimeEdit`、`QDateTimeEdit`、`QCalendarWidget`
- 选择类：`QComboBox`、`QFontComboBox`、颜色选择示例、文件路径选择示例
- 数值调节：`QSlider`、`QDial`、`QScrollBar`、`QProgressBar`
- 分组与容器：`QGroupBox`、`QFrame`、`QScrollArea`、`QSplitter`、`QTabWidget`、`QToolBox`、`QStackedWidget`

### 2. 导航与分页控件 Navigation / Pagination

- 左侧导航树：`QTreeWidget`
- 顶部标签导航：`QTabWidget`
- 面包屑导航：自定义 Breadcrumb 控件
- 分页控件：自定义 `PaginationWidget`
  - 首页、上一页、下一页、末页
  - 页码按钮
  - 当前页 / 总页数
  - 每页条数 `QComboBox`
  - 跳转页码 `QSpinBox`
  - 总记录数展示
- 分页应可绑定表格、列表、卡片流、图表数据源

### 3. Model/View 与数据视图页

- 简单视图：`QListWidget`、`QTableWidget`、`QTreeWidget`
- Model/View：`QListView`、`QTableView`、`QTreeView` + 自定义 `QAbstractTableModel`
- 代理编辑：`QStyledItemDelegate`
- 数据能力：
  - 增删改行
  - 排序
  - 过滤
  - 搜索
  - 分页
  - 单选 / 多选
  - 单元格编辑
  - 行状态标记
  - CSV 导入 / 导出（可选）

### 4. 对话框与反馈页 Dialogs / Feedback

- 标准对话框：`QMessageBox`、`QFileDialog`、`QColorDialog`、`QFontDialog`、`QInputDialog`
- 自定义对话框：表单弹窗、确认弹窗、进度弹窗、非模态工具弹窗
- 反馈控件：状态栏 `QStatusBar`、工具提示 `QToolTip`、进度提示、空状态页、错误页

#### 4.1 标准弹窗完整清单

- 消息弹窗：`QMessageBox`
  - Information
  - Warning
  - Critical
  - Question
  - About
  - About Qt
  - 自定义按钮组合
  - 默认按钮 / 取消按钮
  - 详细信息展开区域
- 文件弹窗：`QFileDialog`
  - 打开单个文件
  - 打开多个文件
  - 保存文件
  - 选择目录
  - 文件类型过滤
  - 原生 / 非原生对话框切换
- 颜色弹窗：`QColorDialog`
  - RGB / HSV 选择
  - Alpha 透明度选择
  - 当前颜色预览
  - 自定义颜色
- 字体弹窗：`QFontDialog`
  - 字体族
  - 字号
  - 粗体
  - 斜体
  - 下划线
  - 删除线
- 输入弹窗：`QInputDialog`
  - 文本输入
  - 多行文本输入
  - 整数输入
  - 浮点数输入
  - 下拉选项输入
- 进度弹窗：`QProgressDialog`
  - 可取消进度
  - 最小值 / 最大值
  - 当前进度
  - 自动关闭
  - 自动重置
- 错误信息弹窗：`QErrorMessage`
  - 错误去重
  - 再次显示控制
- 向导弹窗：`QWizard`
  - 多步骤页面
  - 上一步 / 下一步 / 完成
  - 字段校验
  - 分支流程
- 打印相关弹窗（可选，依赖模块）：`QPrintDialog`、`QPrintPreviewDialog`、`QPageSetupDialog`

#### 4.2 自定义弹窗完整清单

- 模态弹窗 `QDialog::exec`
- 非模态弹窗 `QDialog::show`
- 表单编辑弹窗
- 详情查看弹窗
- 确认删除弹窗
- 批量操作确认弹窗
- 登录弹窗
- 设置弹窗
- 关于弹窗
- 进度任务弹窗
- 可拖拽无边框弹窗
- 右下角通知弹窗 Toast
- 顶部横幅通知 Banner
- Popover 气泡弹窗
- Dropdown 下拉浮层
- 自定义 Tooltip 浮层
- Loading 遮罩弹窗
- 空状态弹窗
- 错误恢复弹窗

#### 4.3 Tooltip 与提示消息完整清单

- `QToolTip`
  - 鼠标悬停提示
  - 手动显示提示
  - 延迟隐藏
  - 富文本 Tooltip
  - 控件级 Tooltip
  - 表格单元格 Tooltip
  - 图表数据点 Tooltip
  - 图形项 Tooltip
- `QWhatsThis`
  - 进入 What's This 模式
  - 控件说明
  - 帮助文本弹出
- `QStatusTip`
  - 菜单项状态栏提示
  - 工具栏按钮状态栏提示
- Placeholder 提示
  - `QLineEdit::setPlaceholderText`
  - `QTextEdit` 空内容提示（自定义）
- 校验提示
  - 输入框错误边框
  - 输入框下方错误文本
  - 表单级错误摘要
  - 必填项标记
  - 范围错误提示
- Toast / Notification（自定义）
  - 成功
  - 信息
  - 警告
  - 错误
  - 加载中
  - 自动消失
  - 手动关闭
  - 多条堆叠
- Badge / Tag 提示
  - 数量徽标
  - 状态标签
  - 风险等级标签
- 空状态 / 错误状态提示
  - 无数据
  - 搜索无结果
  - 加载失败
  - 网络错误模拟
  - 权限不足模拟
  - 重试按钮

#### 4.4 弹窗与提示的可控数据

- 标题
- 正文
- 详细信息
- 图标类型
- 按钮组合
- 默认按钮
- 自动关闭时间
- 是否模态
- 是否置顶
- 是否显示关闭按钮
- 宽度 / 高度
- 背景色
- 文本颜色
- 边框颜色
- 圆角
- 透明度
- 阴影
- 动画开关
- 动画时长
- 出现位置：居中、鼠标附近、控件附近、右上角、右下角、顶部、底部
- Tooltip 延迟时间
- Toast 堆叠数量
- 错误消息列表

### 5. 菜单、工具栏与动作系统

- `QMenuBar`、`QMenu`、`QAction`
- `QToolBar`
- `QStatusBar`
- 快捷键 `QShortcut`
- 右键上下文菜单
- Action 统一注册、启用/禁用状态联动

### 6. 布局展示页 Layouts

- `QHBoxLayout`、`QVBoxLayout`
- `QGridLayout`
- `QFormLayout`
- `QStackedLayout`
- Splitter 可拖拽布局
- ScrollArea 自适应内容
- 响应式示例：窗口宽度变化时调整列数、间距、控件尺寸

### 7. 图形与绘制页 Graphics / Painting

- `QGraphicsView`、`QGraphicsScene`、`QGraphicsItem`
- 基础图元：
  - 矩形
  - 圆 / 椭圆
  - 线段
  - 多边形
  - 文本
  - 图片
- 图形交互：
  - 拖拽
  - 缩放
  - 旋转
  - 选择
  - 对齐
  - 层级调整
  - 删除
- 绘制参数：
  - 填充颜色
  - 边框颜色
  - 边框宽度
  - 透明度
  - 旋转角度
  - 圆角
  - 阴影
  - Z 轴层级

### 8. 图形报表与图表页 Charts / Reports

- 优先使用 QtCharts（Qt 5.15 `Qt5::Charts`），如果本地 Qt 未安装 Charts 模块，则实现轻量自绘图表作为 fallback。
- 图表类型：
  - 柱状图 Bar Chart
  - 条形图 Horizontal Bar Chart
  - 折线图 Line Chart
  - 面积图 Area Chart
  - 饼图 Pie Chart
  - 环形图 Donut Chart
  - 散点图 Scatter Chart
  - 仪表盘 Gauge（可自绘）
  - 进度环 / 百分比环（可自绘）
- 图表能力：
  - 图例显示 / 隐藏
  - 坐标轴标题
  - 坐标轴范围
  - 数据标签
  - 网格线
  - 动画开关
  - 主题切换
  - 数据刷新
  - PNG 导出（可选）

### 9. 多媒体与资源展示页（可选）

- 图片预览、缩放、适配窗口
- GIF 播放
- 图标浏览
- 资源文件列表
- 颜色板 / 调色板

### 10. 高级交互控件页 Advanced Widgets

- 可搜索下拉框
- 标签输入 Tag Editor
- 属性编辑器 Property Editor
- 可折叠面板
- 卡片列表
- 时间线 Timeline
- 步骤条 Stepper
- 评分控件 Rating
- Toast / Notification（自定义）
- Loading / Busy Indicator（自定义）

### 10.1 其他 Qt Widgets 易漏控件补充

- `QLCDNumber`
- `QKeySequenceEdit`
- `QCompleter`
- `QSizeGrip`
- `QRubberBand`
- `QSystemTrayIcon`
- `QUndoView`
- `QDockWidget`
- `QMdiArea`、`QMdiSubWindow`
- `QWizard`、`QWizardPage`
- `QButtonGroup`
- `QActionGroup`
- `QGraphicsWidget`、`QGraphicsProxyWidget`
- `QGraphicsRectItem`、`QGraphicsEllipseItem`、`QGraphicsLineItem`、`QGraphicsPolygonItem`、`QGraphicsTextItem`、`QGraphicsPixmapItem`
- `QHeaderView`
- `QAbstractItemView` 常见选择模式、编辑触发方式、拖放模式
- `QStyledItemDelegate`、`QItemEditorFactory`
- `QDataWidgetMapper`
- `QCompleter` 输入补全示例
- `QValidator`：`QIntValidator`、`QDoubleValidator`、`QRegularExpressionValidator`

### 11. 每个控件页必须附带“可控数据面板”

每个控件展示页底部必须提供一个统一的数据/属性编辑区域，用于实时改变上方控件效果。优先采用 `QGroupBox + QFormLayout`，标题统一为“可控数据 / Control Data”。

通用可控项：

- 文本：标题、占位符、按钮文字、说明文字
- 数值：最小值、最大值、当前值、步长
- 状态：启用/禁用、只读、选中、可见
- 尺寸：宽度、高度、最小尺寸、最大尺寸
- 颜色：前景色、背景色、边框色、填充色
- 字体：字体族、字号、粗体、斜体
- 外观：透明度、圆角、边框宽度、阴影开关
- 布局：间距、边距、对齐方式
- 动画：是否启用、持续时间、速度
- 交互：是否可拖拽、是否可编辑、是否多选

图形/图表专用可控项：

- 数据集：分类名、系列名、数值列表
- 图表类型：柱状图、条形图、折线图、饼图等
- 图形参数：X/Y 坐标、宽高、旋转角度、透明度、圆角
- 颜色参数：填充色、线条色、数据系列颜色
- 坐标轴：最小值、最大值、刻度数量、标题
- 标签：显示/隐藏、位置、格式
- 图例：显示/隐藏、位置
- 数据操作：新增数据、删除数据、随机数据、清空数据、重置默认数据

### 12. 示例数据与用户填充机制

- 每个页面必须内置默认示例数据，打开即可看到效果。
- 每个页面必须允许用户在底部数据面板修改数据并即时刷新控件。
- 表格、列表、图表、分页类页面必须提供“重置数据”“随机数据”“应用数据”按钮。
- 图表数据建议支持简化文本格式输入，例如：

```Plain Text
一月,120
二月,180
三月,90
四月,240
```

- 对非法输入必须给出友好提示，不能导致程序崩溃。

### 12.1 字体、颜色、尺寸与样式设置（必须覆盖）

所有可视控件页面必须提供样式控制能力。基础控件至少支持当前控件局部样式修改；复杂页面支持选中对象样式修改；必要时支持全局主题预览。

#### 字体设置

- 字体族：`QFontComboBox`
- 字号：`QSpinBox`
- 粗体：`QCheckBox`
- 斜体：`QCheckBox`
- 下划线：`QCheckBox`
- 删除线：`QCheckBox`
- 字重：`QComboBox` 或 `QSpinBox`
- 字间距：`QDoubleSpinBox`
- 行高 / 行距：`QDoubleSpinBox`
- 大小写转换：正常、大写、小写、首字母大写（适用于文本示例）
- 对齐方式：左、中、右、两端、上中下
- 文本省略：无、省略左侧、省略中间、省略右侧
- 富文本开关：纯文本 / 富文本
- 字体弹窗：`QFontDialog`
- 字体预览：修改后必须即时作用到示例控件

#### 颜色设置

- 前景色 / 文本色
- 背景色
- 边框色
- 填充色
- 悬停色
- 按下色
- 选中色
- 禁用态颜色
- 高亮色
- 阴影颜色
- 图表系列颜色
- 图形项线条色
- 图形项填充色
- 透明色支持
- Alpha 透明度：0-255 或 0%-100%
- 颜色输入方式：
  - `QColorDialog`
  - 十六进制输入，如 `#2F6FED`
  - RGB 输入
  - HSL/HSV 输入（可选）
  - 预设色板
  - 最近使用颜色
- 颜色预览块：必须展示当前颜色
- 非法颜色输入必须提示并保留上一次有效值

#### 尺寸与间距设置

- 宽度
- 高度
- 最小宽度
- 最小高度
- 最大宽度
- 最大高度
- 固定尺寸开关
- 内容边距：left / top / right / bottom
- 控件间距 spacing
- 图标尺寸
- 表格行高
- 表格列宽
- 列宽自适应开关
- 文本换行开关
- 图片缩放模式：原始、适配、填充、等比裁剪
- DPI / 高分屏适配说明

#### 边框、圆角、阴影与透明度

- 边框宽度
- 边框样式：实线、虚线、点线、无边框
- 圆角半径：统一圆角
- 四角独立圆角：左上、右上、右下、左下
- 透明度
- 阴影开关
- 阴影偏移 X/Y
- 阴影模糊半径
- 阴影扩散半径（如使用自定义绘制）
- 阴影颜色
- 背景图片（可选）

#### 状态样式

- Normal 普通态
- Hover 悬停态
- Pressed 按下态
- Checked 选中态
- Disabled 禁用态
- Focused 焦点态
- Error 错误态
- Warning 警告态
- Success 成功态
- 每种状态至少能演示文本色、背景色、边框色变化

#### 样式应用范围

- 当前选中控件
- 当前页面全部同类控件
- 当前页面全部控件
- 全局应用主题（可选）
- 恢复默认样式
- 导出当前样式为 QSS（可选）
- 从 QSS 文本应用样式（可选，必须做异常保护）

#### 样式控制页建议

- 新增 `StylePage`
  - 字体设置示例
  - 颜色设置示例
  - 尺寸设置示例
  - 圆角/边框/阴影示例
  - 状态样式示例
  - QSS 实时预览
- `StylePage` 必须作为独立页面注册到导航栏。

### 13. 页面建议清单

- ButtonPage：按钮、单选、多选、工具按钮
- InputPage：文本、数字、日期、滑块、下拉框
- SelectorPage：颜色、字体、文件、目录、枚举选择
- StylePage：字体、字号、颜色、尺寸、边框、圆角、阴影、QSS 预览
- ContainerPage：Tab、ToolBox、GroupBox、ScrollArea、Splitter
- LayoutPage：常见布局与响应式布局
- ViewPage：List、Table、Tree
- ModelViewPage：Model/View、排序、过滤、分页、代理编辑
- PaginationPage：独立分页控件与数据绑定示例
- DialogPage：标准对话框与自定义弹窗
- MenuToolbarPage：菜单、工具栏、快捷键、右键菜单
- GraphicsPage：GraphicsView 图元编辑
- ChartPage：柱状图、条形图、折线图、饼图、散点图
- ReportPage：图表 + 表格 + 筛选条件组合报表
- FeedbackPage：状态栏、提示、进度、空状态、错误状态
- AdvancedPage：属性编辑器、可折叠面板、标签输入、Toast

### 14. 开发顺序建议

1. 先补齐页面注册、导航分类、统一可控数据面板基类。
2. 再补基础控件、布局、视图、分页。
3. 然后补 GraphicsView 图形编辑。
4. 最后补 Chart/Report 页面，并根据本机 Qt 模块决定使用 QtCharts 还是自绘 fallback。
5. 每完成一组页面必须编译验证，不能一次性堆完后再修。

## 十二、全方位控件使用示例验收矩阵

本项目目标不是简单罗列控件，而是做一个“Qt Widgets 控件使用示例程序”。每个页面必须尽量覆盖控件在真实项目中的常见功能点，包括图标、快捷键、状态、事件、数据、样式、上下文菜单、国际化和异常保护。

### 1. 每个控件示例的最低展示维度

每个控件或控件组至少考虑以下维度，能实现的必须实现，不能实现的要在页面内通过注释/说明区域标记原因：

- 基础显示
- 常用属性设置
- 启用 / 禁用
- 只读 / 可编辑
- 可见 / 隐藏
- Tooltip
- StatusTip
- What's This 帮助
- 图标设置（适用时）
- 快捷键设置（适用时）
- 右键菜单（适用时）
- 信号槽事件演示
- 动态修改文本
- 动态修改颜色
- 动态修改字体
- 动态修改尺寸
- 动态修改样式 QSS
- 状态变化展示
- 数据填充 / 数据清空 / 数据重置
- 错误输入校验
- 国际化文本
- 日志输出
- 异常捕获

### 2. 表格与数据控件必须覆盖的功能点

表格类页面必须作为重点页面实现，至少覆盖 `QTableWidget` 与 `QTableView + Model` 两种路线。

必须覆盖：

- 表头设置
- 行表头 / 列表头
- 添加行
- 删除行
- 插入行
- 清空数据
- 重置默认数据
- 单元格编辑
- 单元格只读
- 单元格图标
- 单元格 Tooltip
- 单元格背景色
- 单元格前景色
- 单元格字体
- 单元格对齐方式
- 单元格合并
- CheckBox 单元格
- ComboBox 代理编辑
- SpinBox 代理编辑
- Button 代理或操作列
- 行选择
- 单元格选择
- 多选
- 排序
- 过滤
- 搜索
- 分页
- 冻结列（可选，自定义实现）
- 自动列宽
- 手动列宽
- 行高设置
- 交替行颜色
- 表格右键菜单
- 复制选中内容
- 粘贴数据（可选）
- CSV 导入 / 导出（可选）
- 数据变更日志
- 大数据量性能演示（可选）

### 3. 菜单、工具栏、快捷键与 Action 系统必须覆盖

必须提供 `MenuToolbarPage`，完整展示 Qt Action 体系，而不是只放一个菜单。

必须覆盖：

- `QMenuBar`
- 顶级菜单
- 子菜单
- 分隔线
- `QAction`
- 可勾选 Action
- Action 图标
- Action 快捷键
- Action Tooltip
- Action StatusTip
- Action What's This
- Action 启用 / 禁用
- ActionGroup 单选互斥
- 最近文件菜单
- 动态添加菜单项
- 动态删除菜单项
- 右键上下文菜单
- `QToolBar`
- 工具栏图标按钮
- 工具栏文字 + 图标模式
- 工具栏可移动 / 固定
- 工具栏分隔线
- 工具栏嵌入控件：搜索框、下拉框、SpinBox
- `QStatusBar`
- 临时消息
- 永久控件
- 快捷键冲突提示
- 所有 Action 触发后必须在日志区或状态区展示结果

### 4. 图标资源必须覆盖

示例程序必须展示 Qt 中常用图标使用方式：

- `QStyle::standardIcon`
- 本地资源图标 `.qrc`
- 按钮图标
- 菜单图标
- 工具栏图标
- 表格单元格图标
- 树节点图标
- Tab 图标
- 系统托盘图标
- 图标尺寸设置
- 图标启用 / 禁用态
- 图标 + 文本显示模式
- 资源缺失 fallback 图标

### 5. 快捷键必须覆盖

必须提供独立快捷键示例或集成到菜单工具栏页：

- `QAction::setShortcut`
- `QShortcut`
- 单键快捷键
- 组合快捷键：Ctrl/Alt/Shift
- 多快捷键绑定（可选）
- 快捷键上下文：Window、Widget、Application
- 快捷键启用 / 禁用
- 快捷键冲突检测提示
- 快捷键触发日志
- 快捷键列表展示

### 6. 树、列表、Tab、Dock、MDI 必须覆盖

导航和容器类控件必须覆盖真实项目常见能力：

- `QListWidget`
  - 图标列表
  - 多选
  - 拖拽排序
  - 右键菜单
  - 搜索过滤
- `QTreeWidget`
  - 多级节点
  - 节点图标
  - 节点勾选
  - 展开 / 折叠
  - 懒加载子节点
  - 右键菜单
- `QTabWidget`
  - 图标 Tab
  - 可关闭 Tab
  - 可移动 Tab
  - 动态新增 / 关闭 Tab
  - Tab 位置切换
- `QDockWidget`
  - 停靠 / 浮动
  - 关闭 / 显示
  - 多 Dock 区域
  - 保存 / 恢复布局
- `QMdiArea`
  - 新建子窗口
  - 平铺
  - 层叠
  - 激活窗口切换
  - 子窗口关闭

### 7. 表单与输入校验必须覆盖

输入类控件必须展示真实表单开发能力：

- 必填校验
- 长度校验
- 数字范围校验
- 正则校验
- `QIntValidator`
- `QDoubleValidator`
- `QRegularExpressionValidator`
- 输入掩码 `inputMask`
- 自动补全 `QCompleter`
- 密码输入
- 清空按钮
- 只读模式
- 禁用模式
- 错误提示 Tooltip
- 错误边框样式
- 表单提交
- 表单重置
- 表单数据预览

### 8. 图形图表必须覆盖交互功能

图形和图表不能只画静态图，必须展示可交互能力：

- 鼠标悬停 Tooltip
- 点击选中
- 拖拽移动
- 缩放
- 旋转
- 颜色修改
- 透明度修改
- 数据修改后刷新
- 图例开关
- 标签开关
- 坐标轴范围修改
- 导出图片（可选）
- 右键菜单
- 重置视图

### 9. 统一示例页面结构

每个页面建议统一为三段结构：

- 上方：控件展示区
- 中间：事件日志 / 当前状态区
- 下方：可控数据 / 属性编辑区

页面必须避免只展示静态控件。用户修改下方数据后，上方控件必须即时变化，事件日志必须记录关键交互。

### 10. 建议最终导航分组

最终左侧导航建议按分组展示，而不是平铺页面：

- 基础控件
  - 文本与按钮
  - 输入与表单
  - 选择器
  - 数值控件
- 数据展示
  - 列表
  - 表格
  - 树
  - Model/View
  - 分页
- 容器与布局
  - Layout
  - Tab/Stack/ToolBox
  - Splitter/ScrollArea
  - Dock/MDI
- 菜单与动作
  - Menu/Toolbar/StatusBar
  - Shortcut
  - Context Menu
- 弹窗与提示
  - Dialogs
  - Tooltip/WhatsThis
  - Toast/Notification
  - Progress/Error/Empty State
- 图形与报表
  - GraphicsView
  - Charts
  - Reports
- 样式与主题
  - Font
  - Color
  - Size
  - QSS
  - Theme
- 高级示例
  - System Tray
  - Undo/Redo
  - Drag/Drop
  - Clipboard
  - Settings Persistence

### 11. 全方位覆盖原则

- 优先覆盖 Qt Widgets 常用控件和真实项目高频功能。
- 对低频但 Qt Widgets 内置的控件，至少提供最小可运行示例。
- 对抽象基类不单独做页面，但必须通过具体子类体现其能力。
- 对可选模块，如 QtCharts、PrintSupport，先检测本机 Qt 是否可用；不可用时提供自绘或禁用提示。
- 每个功能点都要有可操作入口，不能只写在代码里。
- 每个页面都要有事件反馈，用户操作后必须能看到状态变化。

## 核心指令一句话

实现一个工程级 Qt Widgets 控件展示框架，支持动态注册页面 \+ 国际化 \+ 异常处理 \+ 日志系统 \+ 模块化架构 \+ 可扩展设计。

## 后续可升级方向

- V3 工业级：Qt Plugin 插件架构，动态 DLL 页面加载

- V4 工具平台：控件搜索、文档自动生成、示例代码导出

> （注：文档部分内容可能由 AI 生成）
