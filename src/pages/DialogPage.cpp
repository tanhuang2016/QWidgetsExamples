#include "DialogPage.h"
#include "core/ExceptionHandler.h"
#include "core/Logger.h"

#include <QColorDialog>
#include <QFileDialog>
#include <QGroupBox>
#include <QLabel>
#include <QMessageBox>
#include <QPushButton>
#include <QVBoxLayout>

DialogPage::DialogPage(QWidget *parent)
    : ControlPageBase(parent)
{
    QVBoxLayout *root = new QVBoxLayout(this);
    root->setContentsMargins(24, 24, 24, 24);

    m_group = new QGroupBox(this);
    QVBoxLayout *layout = new QVBoxLayout(m_group);
    m_messageButton = new QPushButton(this);
    m_colorButton = new QPushButton(this);
    m_fileButton = new QPushButton(this);
    m_resultLabel = new QLabel(this);
    m_resultLabel->setWordWrap(true);

    layout->addWidget(m_messageButton);
    layout->addWidget(m_colorButton);
    layout->addWidget(m_fileButton);
    layout->addWidget(m_resultLabel);

    root->addWidget(m_group);
    root->addStretch();

    connect(m_messageButton, &QPushButton::clicked, this, &DialogPage::showMessage);
    connect(m_colorButton, &QPushButton::clicked, this, &DialogPage::chooseColor);
    connect(m_fileButton, &QPushButton::clicked, this, &DialogPage::chooseFile);

    retranslateUi();
    Logger::info("DialogPage initialized");
}

QString DialogPage::pageName() const
{
    return tr("Dialogs");
}

void DialogPage::retranslateUi()
{
    m_group->setTitle(tr("Dialog Examples"));
    m_messageButton->setText(tr("Show Message Box"));
    m_colorButton->setText(tr("Choose Color"));
    m_fileButton->setText(tr("Choose File"));
    if (m_resultLabel->text().isEmpty()) {
        m_resultLabel->setText(tr("Dialog result will appear here."));
    }
}

void DialogPage::showMessage()
{
    try {
        QMessageBox::information(this, tr("Message"), tr("This is a QMessageBox example."));
        m_resultLabel->setText(tr("Message box closed."));
    } catch (const std::exception &e) {
        ExceptionHandler::handle(e);
    }
}

void DialogPage::chooseColor()
{
    try {
        const QColor color = QColorDialog::getColor(Qt::blue, this, tr("Choose Color"));
        if (color.isValid()) {
            m_resultLabel->setText(tr("Selected color: %1").arg(color.name()));
        }
    } catch (const std::exception &e) {
        ExceptionHandler::handle(e);
    }
}

void DialogPage::chooseFile()
{
    try {
        const QString fileName = QFileDialog::getOpenFileName(this, tr("Choose File"));
        if (!fileName.isEmpty()) {
            m_resultLabel->setText(tr("Selected file: %1").arg(fileName));
        }
    } catch (const std::exception &e) {
        ExceptionHandler::handle(e);
    }
}
