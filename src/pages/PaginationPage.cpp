#include "PaginationPage.h"
#include "core/Logger.h"
#include "ui/PaginationWidget.h"

#include <QGroupBox>
#include <QListWidget>
#include <QPushButton>
#include <QRandomGenerator>
#include <QSpinBox>
#include <QStyle>
#include <QTextEdit>
#include <QTime>
#include <QHBoxLayout>
#include <QVBoxLayout>

PaginationPage::PaginationPage(QWidget *parent)
    : ControlPageBase(parent),
      m_list(new QListWidget(this)),
      m_log(new QTextEdit(this)),
      m_totalSpin(new QSpinBox(this)),
      m_pagination(new PaginationWidget(this))
{
    QVBoxLayout *root = new QVBoxLayout(this);
    root->setContentsMargins(24, 24, 24, 24);
    m_totalSpin->setRange(0, 10000);
    m_totalSpin->setValue(128);
    m_log->setReadOnly(true);
    m_log->setMaximumHeight(90);

    QGroupBox *controls = new QGroupBox(this);
    QHBoxLayout *controlLayout = new QHBoxLayout(controls);
    QPushButton *applyButton = new QPushButton(this);
    QPushButton *randomButton = new QPushButton(this);
    applyButton->setObjectName("applyButton");
    randomButton->setObjectName("randomButton");
    controlLayout->addWidget(m_totalSpin);
    controlLayout->addWidget(applyButton);
    controlLayout->addWidget(randomButton);
    controlLayout->addStretch();

    root->addWidget(m_list, 1);
    root->addWidget(m_pagination);
    root->addWidget(m_log);
    root->addWidget(controls);
    connect(m_pagination, &PaginationWidget::pageChanged, this, &PaginationPage::refreshItems);
    connect(applyButton, &QPushButton::clicked, this, &PaginationPage::setTotalFromControl);
    connect(randomButton, &QPushButton::clicked, this, &PaginationPage::randomTotal);
    retranslateUi();
    setTotalFromControl();
}

QString PaginationPage::pageName() const { return tr("Pagination"); }

void PaginationPage::retranslateUi()
{
    findChild<QPushButton *>("applyButton")->setText(tr("Apply total"));
    findChild<QPushButton *>("randomButton")->setText(tr("Random total"));
    m_pagination->retranslateUi();
}

void PaginationPage::refreshItems()
{
    m_list->clear();
    const int start = (m_pagination->currentPage() - 1) * m_pagination->pageSize();
    const int end = qMin(m_pagination->totalCount(), start + m_pagination->pageSize());
    for (int i = start; i < end; ++i) {
        QListWidgetItem *item = new QListWidgetItem(style()->standardIcon(QStyle::SP_FileIcon), tr("Paged record %1").arg(i + 1));
        item->setToolTip(tr("This item is generated from current page data."));
        m_list->addItem(item);
    }
    const QString message = tr("Loaded records %1-%2").arg(start + 1).arg(end);
    m_log->append(QTime::currentTime().toString("hh:mm:ss") + "  " + message);
    Logger::info(QString("PaginationPage: %1").arg(message));
}

void PaginationPage::setTotalFromControl()
{
    m_pagination->setTotalCount(m_totalSpin->value());
    refreshItems();
}

void PaginationPage::randomTotal()
{
    m_totalSpin->setValue(QRandomGenerator::global()->bounded(10, 360));
    setTotalFromControl();
}
