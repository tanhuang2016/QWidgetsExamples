#pragma once

#include "ui/ControlPageBase.h"

#include <QVector>

class QLabel;
class QLineEdit;
class QTableWidget;
class QTextEdit;
class QSpinBox;
class QPushButton;
class QComboBox;
class PaginationWidget;

/**
 * @brief 表格数据记录结构体
 * 
 * 【用途】
 * 表示表格中的一行数据，包含4个字段：
 * - name: 名称（字符串）
 * - status: 状态（字符串，如"Active"、"Inactive"）
 * - value: 数值（整数）
 * - checked: 是否勾选（布尔值）
 * 
 * Java对比：类似于 Java的 POJO / DTO 类
 */
struct TableRecord {
    QString name;      // 名称
    QString status;    // 状态
    int value;         // 数值
    bool checked;      // 是否勾选

    /**
     * @brief 重载==运算符，用于比较两条记录是否相等
     * 
     * C++特性：运算符重载
     * Java对比：类似于 equals() 方法
     */
    bool operator==(const TableRecord &other) const
    {
        return name == other.name && status == other.status && value == other.value && checked == other.checked;
    }
};

/**
 * @brief 功能完整的QTableWidget示例页
 *
 * 【功能清单】
 * 这个页面专门展示真实项目中常见的表格功能：
 * 1. 数据过滤：按名称搜索、按状态筛选
 * 2. 分页显示：自定义PaginationWidget控件
 * 3. 单元格编辑：可编辑文本、可勾选复选框
 * 4. 样式控制：字体大小、行高、文字颜色、背景色
 * 5. 右键菜单：复制、删除等操作
 * 6. 剪贴板操作：复制选中行为TSV格式
 * 7. 日志记录：记录用户操作历史
 * 8. 数据管理：添加行、删除行、重置数据、随机生成数据
 * 
 * 【架构设计】
 * - 数据层：m_records 存储所有原始数据
 * - 视图层：m_table (QTableWidget) 显示当前页的数据
 * - 控制层：各种slot响应用户操作，更新数据和视图
 * 
 * Java对比：
 * - 类似于 JTable + TableModel + TableRowSorter
 * - 或者 JavaFX的 TableView + ObservableList
 */
class TablePage : public ControlPageBase {
    Q_OBJECT

public:
    /**
     * @brief 构造函数
     * 
     * @param parent 父窗口指针
     */
    explicit TablePage(QWidget *parent = nullptr);
    
    /**
     * @brief 返回翻译后的页面标题
     * 
     * @return QString "Tables" -> "表格控件"
     */
    QString pageName() const override;

public slots:
    /**
     * @brief 刷新标签文本，并重新渲染翻译后的表头
     * 
     * 【调用时机】
     * 当用户切换语言时调用
     */
    void retranslateUi() override;

private slots:
    /**
     * @brief 应用搜索文本和状态过滤条件
     * 
     * 【触发方式】
     * 用户在搜索框输入或选择状态过滤器时自动调用
     * 
     * Java对比：类似于 JTextField.addActionListener()
     */
    void applyFilter();
    
    /**
     * @brief 分页控件变化时重新渲染当前页
     * 
     * 【触发方式】
     * 用户点击上一页、下一页、跳转页码时调用
     */
    void refreshPage();
    
    /**
     * @brief 向底层数据列表添加一行
     * 
     * Java对比：类似于 tableModel.addRow(newRow)
     */
    void addRow();
    
    /**
     * @brief 删除当前选中的源数据行
     * 
     * 【注意】
     * 删除的是 m_records 中的数据，不是表格中的可见行
     * 删除后需要调用 renderTable() 刷新显示
     */
    void deleteSelectedRows();
    
    /**
     * @brief 恢复固定的默认示例数据
     * 
     * 将 m_records 重置为硬编码的示例数据
     */
    void resetData();
    
    /**
     * @brief 生成随机示例数据
     * 
     * 使用 qrand() 生成随机的名称、状态、数值
     */
    void randomData();
    
    /**
     * @brief 把字号、行高、颜色应用到当前可见单元格
     * 
     * 【功能】
     * 实时调整表格的视觉样式，方便用户预览效果
     */
    void applyStyleControls();
    
    /**
     * @brief 打开表格右键菜单
     * 
     * 【触发方式】
     * 用户在表格上右键点击时自动调用
     * 
     * Java对比：类似于 JPopupMenu.show()
     * 
     * @param pos 鼠标点击位置（相对于表格坐标系）
     */
    void showContextMenu(const QPoint &pos);
    
    /**
     * @brief 把选中行复制为制表符分隔文本（TSV）
     * 
     * 【用途】
     * 用户可以粘贴到Excel或其他表格软件中
     * 
     * Java对比：类似于 Clipboard.setContents()
     */
    void copySelection();
    
    /**
     * @brief 把用户编辑过的单元格值写回 m_records
     * 
     * 【重要】
     * QTableWidget的编辑不会自动同步到 m_records
     * 必须手动监听 cellChanged 信号并更新数据模型
     * 
     * @param row 行号
     * @param column 列号
     */
    void onCellChanged(int row, int column);

private:
    /**
     * @brief 创建默认的内存数据集
     * 
     * 初始化 m_records 为硬编码的示例数据
     */
    void buildData();
    
    /**
     * @brief 根据过滤和分页后的数据重建QTableWidget
     * 
     * 【工作流程】
     * 1. 调用 filteredRows() 获取符合条件的数据
     * 2. 根据当前页码和每页条数截取数据
     * 3. 清空 m_table 并重新填充
     * 4. 设置表头、样式、信号槽连接
     * 
     * 【性能优化】
     * 使用 m_rendering 标志防止递归调用
     */
    void renderTable();
    
    /**
     * @brief 返回符合当前搜索和状态过滤条件的行
     * 
     * 【过滤逻辑】
     * - 搜索框：模糊匹配 name 字段（不区分大小写）
     * - 状态过滤器：精确匹配 status 字段
     * 
     * @return QVector<TableRecord> 过滤后的数据子集
     */
    QVector<TableRecord> filteredRows() const;
    
    /**
     * @brief 追加界面日志，并写入文件日志
     * 
     * 【双重记录】
     * 1. 界面日志：显示在右侧的 QTextEdit 中，用户可见
     * 2. 文件日志：通过 Logger::info() 写入 logs/app.log
     * 
     * @param message 日志消息
     */
    void appendLog(const QString &message);

    // UI控件成员变量
    QTableWidget *m_table;           // 主表格控件
    QTextEdit *m_log;                // 日志显示区域
    QLineEdit *m_searchEdit;         // 搜索输入框
    QComboBox *m_statusFilter;       // 状态下拉过滤器
    QSpinBox *m_fontSizeSpin;        // 字体大小调节
    QSpinBox *m_rowHeightSpin;       // 行高调节
    QPushButton *m_textColorButton;   // 文字颜色选择按钮
    QPushButton *m_backgroundButton;  // 背景颜色选择按钮
    PaginationWidget *m_pagination;  // 分页控件
    
    // 数据成员
    QVector<TableRecord> m_records;  // 所有数据记录的集合
    QColor m_textColor;              // 当前文字颜色
    QColor m_backgroundColor;        // 当前背景颜色
    bool m_rendering;                // 渲染标志，防止递归调用
};
