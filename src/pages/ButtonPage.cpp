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

ButtonPage::ButtonPage(QWidget *parent)
    : ControlPageBase(parent)
{
    QVBoxLayout *root = new QVBoxLayout(this);
    root->setContentsMargins(24, 24, 24, 24);

    m_pushGroup = new QGroupBox(this);
    QHBoxLayout *pushLayout = new QHBoxLayout(m_pushGroup);
    m_primaryButton = new QPushButton(this);
    m_toggleButton = new QPushButton(this);
    m_toggleButton->setCheckable(true);
    m_toolButton = new QToolButton(this);
    m_toolButton->setIcon(style()->standardIcon(QStyle::SP_FileDialogDetailedView));
    pushLayout->addWidget(m_primaryButton);
    pushLayout->addWidget(m_toggleButton);
    pushLayout->addWidget(m_toolButton);
    pushLayout->addStretch();

    m_optionGroup = new QGroupBox(this);
    QVBoxLayout *optionLayout = new QVBoxLayout(m_optionGroup);
    m_checkBox = new QCheckBox(this);
    m_radioA = new QRadioButton(this);
    m_radioB = new QRadioButton(this);
    m_radioA->setChecked(true);
    optionLayout->addWidget(m_checkBox);
    optionLayout->addWidget(m_radioA);
    optionLayout->addWidget(m_radioB);

    m_statusLabel = new QLabel(this);

    root->addWidget(m_pushGroup);
    root->addWidget(m_optionGroup);
    root->addWidget(m_statusLabel);
    root->addStretch();

    connect(m_primaryButton, &QPushButton::clicked, this, &ButtonPage::updateStatus);
    connect(m_toggleButton, &QPushButton::toggled, this, &ButtonPage::updateStatus);
    connect(m_toolButton, &QToolButton::clicked, this, &ButtonPage::updateStatus);
    connect(m_checkBox, &QCheckBox::toggled, this, &ButtonPage::updateStatus);
    connect(m_radioA, &QRadioButton::toggled, this, &ButtonPage::updateStatus);
    connect(m_radioB, &QRadioButton::toggled, this, &ButtonPage::updateStatus);

    retranslateUi();
    updateStatus();
    Logger::info("ButtonPage initialized");
}

QString ButtonPage::pageName() const
{
    return tr("Buttons");
}

void ButtonPage::retranslateUi()
{
    m_pushGroup->setTitle(tr("Push Buttons"));
    m_optionGroup->setTitle(tr("Options"));
    m_primaryButton->setText(tr("Primary Action"));
    m_toggleButton->setText(tr("Toggle"));
    m_toolButton->setText(tr("Tool"));
    m_toolButton->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    m_checkBox->setText(tr("Enable extra option"));
    m_radioA->setText(tr("Mode A"));
    m_radioB->setText(tr("Mode B"));
    updateStatus();
}

void ButtonPage::updateStatus()
{
    try {
        const QString mode = m_radioA->isChecked() ? tr("Mode A") : tr("Mode B");
        m_statusLabel->setText(tr("Current state: toggle=%1, option=%2, mode=%3")
                               .arg(m_toggleButton->isChecked() ? tr("on") : tr("off"))
                               .arg(m_checkBox->isChecked() ? tr("checked") : tr("unchecked"))
                               .arg(mode));
    } catch (const std::exception &e) {
        ExceptionHandler::handle(e);
    }
}
