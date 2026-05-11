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

class TablePage : public ControlPageBase {
    Q_OBJECT

public:
    explicit TablePage(QWidget *parent = nullptr);
    QString pageName() const override;

public slots:
    void retranslateUi() override;

private slots:
    void applyFilter();
    void refreshPage();
    void addRow();
    void deleteSelectedRows();
    void resetData();
    void randomData();
    void applyStyleControls();
    void showContextMenu(const QPoint &pos);
    void copySelection();
    void onCellChanged(int row, int column);

private:
    void buildData();
    void renderTable();
    QVector<TableRecord> filteredRows() const;
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
