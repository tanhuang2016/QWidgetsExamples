#include "LayoutPage.h"
#include "core/Logger.h"

#include <QFormLayout>
#include <QGridLayout>
#include <QGroupBox>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QVBoxLayout>

LayoutPage::LayoutPage(QWidget *parent)
    : ControlPageBase(parent)
{
    QVBoxLayout *root = new QVBoxLayout(this);
    root->setContentsMargins(24, 24, 24, 24);

    m_gridGroup = new QGroupBox(this);
    QGridLayout *grid = new QGridLayout(m_gridGroup);
    for (int row = 0; row < 2; ++row) {
        for (int col = 0; col < 3; ++col) {
            QLabel *label = new QLabel(this);
            label->setAlignment(Qt::AlignCenter);
            label->setMinimumHeight(48);
            label->setStyleSheet("background: #eef2f7; border: 1px solid #c9ced6; border-radius: 3px;");
            m_gridLabels.append(label);
            grid->addWidget(label, row, col);
        }
    }

    m_boxGroup = new QGroupBox(this);
    QVBoxLayout *boxRoot = new QVBoxLayout(m_boxGroup);
    QHBoxLayout *toolbar = new QHBoxLayout();
    toolbar->addWidget(new QPushButton(tr("New"), this));
    toolbar->addWidget(new QPushButton(tr("Save"), this));
    toolbar->addStretch();
    m_boxLabel = new QLabel(this);
    m_boxLabel->setWordWrap(true);
    boxRoot->addLayout(toolbar);
    boxRoot->addWidget(m_boxLabel);

    QGroupBox *formGroup = new QGroupBox(tr("Form Layout"), this);
    QFormLayout *form = new QFormLayout(formGroup);
    form->addRow(tr("Host"), new QLineEdit("localhost", this));
    form->addRow(tr("Port"), new QLineEdit("8080", this));

    root->addWidget(m_gridGroup);
    root->addWidget(m_boxGroup);
    root->addWidget(formGroup);
    root->addStretch();

    retranslateUi();
    Logger::info("LayoutPage initialized");
}

QString LayoutPage::pageName() const
{
    return tr("Layouts");
}

void LayoutPage::retranslateUi()
{
    m_gridGroup->setTitle(tr("Grid Layout"));
    m_boxGroup->setTitle(tr("Box Layout"));
    for (int i = 0; i < m_gridLabels.size(); ++i) {
        m_gridLabels[i]->setText(tr("Cell %1").arg(i + 1));
    }
    m_boxLabel->setText(tr("HBoxLayout and VBoxLayout compose a compact tool area."));
}
