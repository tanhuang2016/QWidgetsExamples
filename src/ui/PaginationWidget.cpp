#include "PaginationWidget.h"

#include <QComboBox>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QSpinBox>

PaginationWidget::PaginationWidget(QWidget *parent)
    : QWidget(parent),
      m_firstButton(new QPushButton(this)),
      m_prevButton(new QPushButton(this)),
      m_nextButton(new QPushButton(this)),
      m_lastButton(new QPushButton(this)),
      m_jumpButton(new QPushButton(this)),
      m_infoLabel(new QLabel(this)),
      m_pageSizeLabel(new QLabel(this)),
      m_pageSpin(new QSpinBox(this)),
      m_pageSizeCombo(new QComboBox(this)),
      m_totalCount(0),
      m_currentPage(1)
{
    QHBoxLayout *layout = new QHBoxLayout(this);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->addWidget(m_firstButton);
    layout->addWidget(m_prevButton);
    layout->addWidget(m_infoLabel);
    layout->addWidget(m_nextButton);
    layout->addWidget(m_lastButton);
    layout->addSpacing(12);
    layout->addWidget(m_pageSizeLabel);
    layout->addWidget(m_pageSizeCombo);
    layout->addWidget(m_pageSpin);
    layout->addWidget(m_jumpButton);
    layout->addStretch();

    m_pageSizeCombo->addItems(QStringList() << "5" << "10" << "20" << "50");
    m_pageSizeCombo->setCurrentText("10");
    m_pageSpin->setMinimum(1);

    connect(m_firstButton, &QPushButton::clicked, this, &PaginationWidget::first);
    connect(m_prevButton, &QPushButton::clicked, this, &PaginationWidget::previous);
    connect(m_nextButton, &QPushButton::clicked, this, &PaginationWidget::next);
    connect(m_lastButton, &QPushButton::clicked, this, &PaginationWidget::last);
    connect(m_jumpButton, &QPushButton::clicked, this, &PaginationWidget::jump);
    connect(m_pageSizeCombo, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &PaginationWidget::pageSizeChanged);
    retranslateUi();
    updateState(false);
}

int PaginationWidget::currentPage() const { return m_currentPage; }
int PaginationWidget::pageSize() const { return m_pageSizeCombo->currentText().toInt(); }
int PaginationWidget::totalCount() const { return m_totalCount; }
int PaginationWidget::pageCount() const { return qMax(1, (m_totalCount + pageSize() - 1) / pageSize()); }

void PaginationWidget::setTotalCount(int totalCount)
{
    m_totalCount = qMax(0, totalCount);
    if (m_currentPage > pageCount()) {
        m_currentPage = pageCount();
    }
    updateState(false);
}

void PaginationWidget::setCurrentPage(int page)
{
    m_currentPage = qBound(1, page, pageCount());
    updateState(true);
}

void PaginationWidget::retranslateUi()
{
    m_firstButton->setText(tr("First"));
    m_prevButton->setText(tr("Prev"));
    m_nextButton->setText(tr("Next"));
    m_lastButton->setText(tr("Last"));
    m_jumpButton->setText(tr("Jump"));
    m_pageSizeLabel->setText(tr("Page size"));
    updateState(false);
}

void PaginationWidget::first() { setCurrentPage(1); }
void PaginationWidget::previous() { setCurrentPage(m_currentPage - 1); }
void PaginationWidget::next() { setCurrentPage(m_currentPage + 1); }
void PaginationWidget::last() { setCurrentPage(pageCount()); }
void PaginationWidget::jump() { setCurrentPage(m_pageSpin->value()); }

void PaginationWidget::pageSizeChanged()
{
    m_currentPage = 1;
    updateState(true);
}

void PaginationWidget::updateState(bool emitChange)
{
    const int pages = pageCount();
    m_currentPage = qBound(1, m_currentPage, pages);
    m_pageSpin->setMaximum(pages);
    m_pageSpin->setValue(m_currentPage);
    m_infoLabel->setText(tr("Page %1 / %2, total %3").arg(m_currentPage).arg(pages).arg(m_totalCount));
    m_firstButton->setEnabled(m_currentPage > 1);
    m_prevButton->setEnabled(m_currentPage > 1);
    m_nextButton->setEnabled(m_currentPage < pages);
    m_lastButton->setEnabled(m_currentPage < pages);
    if (emitChange) {
        emit pageChanged(m_currentPage, pageSize());
    }
}
