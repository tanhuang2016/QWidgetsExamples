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

struct TableRecord {
    QString name;
    QString status;
    int value;
    bool checked;

    bool operator==(const TableRecord &other) const
    {
        return name == other.name && status == other.status && value == other.value && checked == other.checked;
    }
};

/**
 * @brief 功能较完整的 QTableWidget 示例页。
 *
 * 这个页面专门展示真实项目中常见的表格功能：过滤、分页、可编辑单元格、
 * 可勾选单元格、带图标单元格、单元格内嵌按钮、右键菜单、复制到剪贴板、
 * 字体颜色和行高控制等。
 */
class TablePage : public ControlPageBase {
    Q_OBJECT

public:
    explicit TablePage(QWidget *parent = nullptr);
    /** 返回翻译后的页面标题。 */
    QString pageName() const override;

public slots:
    /** 刷新标签文本，并重新渲染翻译后的表头。 */
    void retranslateUi() override;

private slots:
    /** 应用搜索文本和状态过滤条件。 */
    void applyFilter();
    /** 分页控件变化时重新渲染当前页。 */
    void refreshPage();
    /** 向底层数据列表添加一行。 */
    void addRow();
    /** 删除当前选中的源数据行。 */
    void deleteSelectedRows();
    /** 恢复固定的默认示例数据。 */
    void resetData();
    /** 生成随机示例数据。 */
    void randomData();
    /** 把字号、行高、颜色应用到当前可见单元格。 */
    void applyStyleControls();
    /** 打开表格右键菜单。 */
    void showContextMenu(const QPoint &pos);
    /** 把选中行复制为制表符分隔文本。 */
    void copySelection();
    /** 把用户编辑过的单元格值写回 m_records。 */
    void onCellChanged(int row, int column);

private:
    /** 创建默认的内存数据集。 */
    void buildData();
    /** 根据过滤和分页后的数据重建 QTableWidget。 */
    void renderTable();
    /** 返回符合当前搜索和状态过滤条件的行。 */
    QVector<TableRecord> filteredRows() const;
    /** 追加界面日志，并写入文件日志。 */
    void appendLog(const QString &message);

    QTableWidget *m_table;
    QTextEdit *m_log;
    QLineEdit *m_searchEdit;
    QComboBox *m_statusFilter;
    QSpinBox *m_fontSizeSpin;
    QSpinBox *m_rowHeightSpin;
    QPushButton *m_textColorButton;
    QPushButton *m_backgroundButton;
    PaginationWidget *m_pagination;
    QVector<TableRecord> m_records;
    QColor m_textColor;
    QColor m_backgroundColor;
    bool m_rendering;
};
