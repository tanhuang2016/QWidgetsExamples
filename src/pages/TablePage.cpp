#include "TablePage.h"
#include "core/ExceptionHandler.h"
#include "core/Logger.h"
#include "ui/PaginationWidget.h"

#include <QAction>
#include <QApplication>
#include <QClipboard>
#include <QColorDialog>
#include <QComboBox>
#include <QFormLayout>
#include <QGroupBox>
#include <QHeaderView>
#include <QLineEdit>
#include <QMenu>
#include <QPushButton>
#include <QRandomGenerator>
#include <QSpinBox>
#include <QTableWidget>
#include <QTextEdit>
#include <QTime>
#include <QVBoxLayout>
#include <QStyle>
#include <QKeySequence>
#include <QAbstractItemView>
#include <functional>

TablePage::TablePage(QWidget *parent)
    : ControlPageBase(parent),
      m_table(new QTableWidget(this)),
      m_log(new QTextEdit(this)),
      m_searchEdit(new QLineEdit(this)),
      m_statusFilter(new QComboBox(this)),
      m_fontSizeSpin(new QSpinBox(this)),
      m_rowHeightSpin(new QSpinBox(this)),
      m_textColorButton(new QPushButton(this)),
      m_backgroundButton(new QPushButton(this)),
      m_pagination(new PaginationWidget(this)),
      m_textColor(Qt::black),
      m_backgroundColor(Qt::white),
      m_rendering(false)
{
    QVBoxLayout *root = new QVBoxLayout(this);
    root->setContentsMargins(24, 24, 24, 24);

    QGroupBox *filterGroup = new QGroupBox(this);
    QHBoxLayout *filterLayout = new QHBoxLayout(filterGroup);
    QPushButton *addButton = new QPushButton(this);
    QPushButton *deleteButton = new QPushButton(this);
    QPushButton *resetButton = new QPushButton(this);
    QPushButton *randomButton = new QPushButton(this);
    filterLayout->addWidget(m_searchEdit, 2);
    filterLayout->addWidget(m_statusFilter);
    filterLayout->addWidget(addButton);
    filterLayout->addWidget(deleteButton);
    filterLayout->addWidget(resetButton);
    filterLayout->addWidget(randomButton);

    m_table->setAlternatingRowColors(true);
    m_table->setSelectionBehavior(QAbstractItemView::SelectRows);
    m_table->setSelectionMode(QAbstractItemView::ExtendedSelection);
    m_table->setSortingEnabled(true);
    m_table->setContextMenuPolicy(Qt::CustomContextMenu);
    m_table->horizontalHeader()->setStretchLastSection(true);

    m_log->setReadOnly(true);
    m_log->setMaximumHeight(90);

    QGroupBox *controlGroup = new QGroupBox(this);
    QFormLayout *controls = new QFormLayout(controlGroup);
    m_fontSizeSpin->setRange(8, 30);
    m_fontSizeSpin->setValue(11);
    m_rowHeightSpin->setRange(22, 80);
    m_rowHeightSpin->setValue(30);
    controls->addRow(tr("Font size"), m_fontSizeSpin);
    controls->addRow(tr("Row height"), m_rowHeightSpin);
    controls->addRow(tr("Text color"), m_textColorButton);
    controls->addRow(tr("Background"), m_backgroundButton);

    root->addWidget(filterGroup);
    root->addWidget(m_table, 1);
    root->addWidget(m_pagination);
    root->addWidget(m_log);
    root->addWidget(controlGroup);

    addButton->setObjectName("addButton");
    deleteButton->setObjectName("deleteButton");
    resetButton->setObjectName("resetButton");
    randomButton->setObjectName("randomButton");

    connect(m_searchEdit, &QLineEdit::textChanged, this, &TablePage::applyFilter);
    connect(m_statusFilter, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &TablePage::applyFilter);
    connect(m_pagination, &PaginationWidget::pageChanged, this, &TablePage::refreshPage);
    connect(addButton, &QPushButton::clicked, this, &TablePage::addRow);
    connect(deleteButton, &QPushButton::clicked, this, &TablePage::deleteSelectedRows);
    connect(resetButton, &QPushButton::clicked, this, &TablePage::resetData);
    connect(randomButton, &QPushButton::clicked, this, &TablePage::randomData);
    connect(m_fontSizeSpin, QOverload<int>::of(&QSpinBox::valueChanged), this, &TablePage::applyStyleControls);
    connect(m_rowHeightSpin, QOverload<int>::of(&QSpinBox::valueChanged), this, &TablePage::applyStyleControls);
    connect(m_textColorButton, &QPushButton::clicked, this, [this]() {
        const QColor color = QColorDialog::getColor(m_textColor, this, tr("Text color"));
        if (color.isValid()) {
            m_textColor = color;
            applyStyleControls();
        }
    });
    connect(m_backgroundButton, &QPushButton::clicked, this, [this]() {
        const QColor color = QColorDialog::getColor(m_backgroundColor, this, tr("Background"));
        if (color.isValid()) {
            m_backgroundColor = color;
            applyStyleControls();
        }
    });
    connect(m_table, &QTableWidget::customContextMenuRequested, this, &TablePage::showContextMenu);
    connect(m_table, &QTableWidget::cellChanged, this, &TablePage::onCellChanged);

    buildData();
    retranslateUi();
    renderTable();
    Logger::info("TablePage initialized");
}

QString TablePage::pageName() const { return tr("Tables"); }

void TablePage::retranslateUi()
{
    m_searchEdit->setPlaceholderText(tr("Search name..."));
    const QString current = m_statusFilter->currentData().toString();
    m_statusFilter->blockSignals(true);
    m_statusFilter->clear();
    m_statusFilter->addItem(tr("All"), "");
    m_statusFilter->addItem(tr("Ready"), "Ready");
    m_statusFilter->addItem(tr("Pending"), "Pending");
    m_statusFilter->addItem(tr("Error"), "Error");
    m_statusFilter->setCurrentIndex(qMax(0, m_statusFilter->findData(current)));
    m_statusFilter->blockSignals(false);
    findChild<QPushButton *>("addButton")->setText(tr("Add"));
    findChild<QPushButton *>("deleteButton")->setText(tr("Delete"));
    findChild<QPushButton *>("resetButton")->setText(tr("Reset"));
    findChild<QPushButton *>("randomButton")->setText(tr("Random"));
    m_textColorButton->setText(m_textColor.name());
    m_backgroundButton->setText(m_backgroundColor.name());
    m_pagination->retranslateUi();
    renderTable();
}

void TablePage::applyFilter()
{
    try {
        m_pagination->setCurrentPage(1);
        renderTable();
        appendLog(tr("Filter applied"));
    } catch (const std::exception &e) {
        ExceptionHandler::handle(e);
    }
}

void TablePage::refreshPage()
{
    renderTable();
}

void TablePage::addRow()
{
    m_records.append(TableRecord{tr("New Item"), "Ready", 50, true});
    renderTable();
    appendLog(tr("Row added"));
}

void TablePage::deleteSelectedRows()
{
    QList<int> rows;
    for (const QModelIndex &index : m_table->selectionModel()->selectedRows()) {
        rows.append(m_table->item(index.row(), 0)->data(Qt::UserRole).toInt());
    }
    std::sort(rows.begin(), rows.end(), std::greater<int>());
    for (int sourceRow : rows) {
        if (sourceRow >= 0 && sourceRow < m_records.size()) {
            m_records.remove(sourceRow);
        }
    }
    renderTable();
    appendLog(tr("Selected rows deleted"));
}

void TablePage::resetData()
{
    buildData();
    renderTable();
    appendLog(tr("Default data restored"));
}

void TablePage::randomData()
{
    static const QStringList statuses = QStringList() << "Ready" << "Pending" << "Error";
    m_records.clear();
    for (int i = 0; i < 36; ++i) {
        m_records.append(TableRecord{
            tr("Record %1").arg(i + 1),
            statuses.at(QRandomGenerator::global()->bounded(statuses.size())),
            QRandomGenerator::global()->bounded(10, 999),
            QRandomGenerator::global()->bounded(2) == 1
        });
    }
    renderTable();
    appendLog(tr("Random data generated"));
}

void TablePage::applyStyleControls()
{
    QFont font = m_table->font();
    font.setPointSize(m_fontSizeSpin->value());
    m_table->setFont(font);
    for (int row = 0; row < m_table->rowCount(); ++row) {
        m_table->setRowHeight(row, m_rowHeightSpin->value());
        for (int col = 0; col < m_table->columnCount(); ++col) {
            QTableWidgetItem *item = m_table->item(row, col);
            if (item) {
                item->setForeground(m_textColor);
                item->setBackground(m_backgroundColor);
                item->setFont(font);
            }
        }
    }
    m_textColorButton->setText(m_textColor.name());
    m_backgroundButton->setText(m_backgroundColor.name());
}

void TablePage::showContextMenu(const QPoint &pos)
{
    QMenu menu(this);
    QAction *copyAction = menu.addAction(style()->standardIcon(QStyle::SP_FileDialogDetailedView), tr("Copy selection"));
    copyAction->setShortcut(QKeySequence::Copy);
    QAction *deleteAction = menu.addAction(style()->standardIcon(QStyle::SP_TrashIcon), tr("Delete rows"));
    QAction *selected = menu.exec(m_table->viewport()->mapToGlobal(pos));
    if (selected == copyAction) {
        copySelection();
    } else if (selected == deleteAction) {
        deleteSelectedRows();
    }
}

void TablePage::copySelection()
{
    QStringList lines;
    for (const QModelIndex &index : m_table->selectionModel()->selectedRows()) {
        QStringList cells;
        for (int col = 0; col < m_table->columnCount(); ++col) {
            QTableWidgetItem *item = m_table->item(index.row(), col);
            cells << (item ? item->text() : QString());
        }
        lines << cells.join('\t');
    }
    QApplication::clipboard()->setText(lines.join('\n'));
    appendLog(tr("Selection copied"));
}

void TablePage::onCellChanged(int row, int column)
{
    if (m_rendering || column < 0 || row < 0) {
        return;
    }
    QTableWidgetItem *source = m_table->item(row, 0);
    if (!source) {
        return;
    }
    const int sourceRow = source->data(Qt::UserRole).toInt();
    if (sourceRow < 0 || sourceRow >= m_records.size()) {
        return;
    }
    if (column == 0) {
        m_records[sourceRow].name = m_table->item(row, column)->text();
    } else if (column == 2) {
        m_records[sourceRow].value = m_table->item(row, column)->text().toInt();
    }
    appendLog(tr("Cell changed: row %1 column %2").arg(row + 1).arg(column + 1));
}

void TablePage::buildData()
{
    m_records.clear();
    for (int i = 0; i < 42; ++i) {
        const QString status = (i % 5 == 0) ? "Error" : ((i % 2 == 0) ? "Ready" : "Pending");
        m_records.append(TableRecord{tr("Item %1").arg(i + 1), status, (i + 1) * 7, i % 3 == 0});
    }
}

void TablePage::renderTable()
{
    m_rendering = true;
    const QVector<TableRecord> rows = filteredRows();
    m_pagination->setTotalCount(rows.size());
    const int start = (m_pagination->currentPage() - 1) * m_pagination->pageSize();
    const int count = qMin(m_pagination->pageSize(), rows.size() - start);

    m_table->clear();
    m_table->setColumnCount(5);
    m_table->setHorizontalHeaderLabels(QStringList() << tr("Name") << tr("Status") << tr("Value") << tr("Checked") << tr("Action"));
    m_table->setRowCount(qMax(0, count));
    for (int i = 0; i < count; ++i) {
        const TableRecord record = rows.at(start + i);
        const int sourceRow = m_records.indexOf(record);
        QTableWidgetItem *name = new QTableWidgetItem(style()->standardIcon(QStyle::SP_FileIcon), record.name);
        name->setData(Qt::UserRole, sourceRow);
        name->setToolTip(tr("Editable name with icon"));
        QTableWidgetItem *status = new QTableWidgetItem(record.status == "Ready" ? tr("Ready") : record.status == "Pending" ? tr("Pending") : tr("Error"));
        status->setFlags(status->flags() & ~Qt::ItemIsEditable);
        status->setToolTip(tr("Read-only status"));
        QTableWidgetItem *value = new QTableWidgetItem(QString::number(record.value));
        QTableWidgetItem *checked = new QTableWidgetItem();
        checked->setFlags((checked->flags() | Qt::ItemIsUserCheckable) & ~Qt::ItemIsEditable);
        checked->setCheckState(record.checked ? Qt::Checked : Qt::Unchecked);
        QPushButton *button = new QPushButton(tr("Open"), m_table);
        connect(button, &QPushButton::clicked, this, [this, record]() { appendLog(tr("Action clicked: %1").arg(record.name)); });
        m_table->setItem(i, 0, name);
        m_table->setItem(i, 1, status);
        m_table->setItem(i, 2, value);
        m_table->setItem(i, 3, checked);
        m_table->setCellWidget(i, 4, button);
    }
    m_table->resizeColumnsToContents();
    applyStyleControls();
    m_rendering = false;
}

QVector<TableRecord> TablePage::filteredRows() const
{
    QVector<TableRecord> rows;
    const QString text = m_searchEdit->text().trimmed();
    const QString status = m_statusFilter->currentData().toString();
    for (const TableRecord &record : m_records) {
        if (!text.isEmpty() && !record.name.contains(text, Qt::CaseInsensitive)) {
            continue;
        }
        if (!status.isEmpty() && record.status != status) {
            continue;
        }
        rows.append(record);
    }
    return rows;
}

void TablePage::appendLog(const QString &message)
{
    m_log->append(QTime::currentTime().toString("hh:mm:ss") + "  " + message);
    Logger::info(QString("TablePage: %1").arg(message));
}
