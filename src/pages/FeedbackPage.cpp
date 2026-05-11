#include "FeedbackPage.h"
#include "core/Logger.h"

#include <QErrorMessage>
#include <QApplication>
#include <QFormLayout>
#include <QGroupBox>
#include <QHBoxLayout>
#include <QInputDialog>
#include <QLabel>
#include <QLineEdit>
#include <QMessageBox>
#include <QProgressDialog>
#include <QPushButton>
#include <QRegularExpressionValidator>
#include <QTextEdit>
#include <QThread>
#include <QTimer>
#include <QTime>
#include <QToolTip>
#include <QVBoxLayout>
#include <QWhatsThis>

FeedbackPage::FeedbackPage(QWidget *parent)
    : ControlPageBase(parent),
      m_tipTarget(new QLabel(this)),
      m_validationLabel(new QLabel(this)),
      m_lineEdit(new QLineEdit(this)),
      m_log(new QTextEdit(this))
{
    QVBoxLayout *root = new QVBoxLayout(this);
    root->setContentsMargins(24, 24, 24, 24);

    QGroupBox *dialogs = new QGroupBox(this);
    QHBoxLayout *dialogLayout = new QHBoxLayout(dialogs);
    QPushButton *infoButton = new QPushButton(this);
    QPushButton *questionButton = new QPushButton(this);
    QPushButton *inputButton = new QPushButton(this);
    QPushButton *progressButton = new QPushButton(this);
    QPushButton *tooltipButton = new QPushButton(this);
    dialogLayout->addWidget(infoButton);
    dialogLayout->addWidget(questionButton);
    dialogLayout->addWidget(inputButton);
    dialogLayout->addWidget(progressButton);
    dialogLayout->addWidget(tooltipButton);
    dialogLayout->addStretch();

    QGroupBox *tips = new QGroupBox(this);
    QFormLayout *tipLayout = new QFormLayout(tips);
    m_tipTarget->setFrameShape(QFrame::StyledPanel);
    m_tipTarget->setMinimumHeight(42);
    m_tipTarget->setAlignment(Qt::AlignCenter);
    m_tipTarget->setToolTip(tr("<b>Rich Tooltip</b><br/>This tooltip supports HTML."));
    m_tipTarget->setWhatsThis(tr("What's This help explains a widget in more detail."));
    m_lineEdit->setPlaceholderText(tr("Type 3-12 letters"));
    m_lineEdit->setValidator(new QRegularExpressionValidator(QRegularExpression("[A-Za-z]{0,12}"), this));
    m_validationLabel->setStyleSheet("color: #b00020;");
    tipLayout->addRow(tr("Tooltip target"), m_tipTarget);
    tipLayout->addRow(tr("Validated input"), m_lineEdit);
    tipLayout->addRow(QString(), m_validationLabel);

    m_log->setReadOnly(true);
    m_log->setMaximumHeight(110);
    root->addWidget(dialogs);
    root->addWidget(tips);
    root->addWidget(m_log);
    root->addStretch();

    infoButton->setObjectName("infoButton");
    questionButton->setObjectName("questionButton");
    inputButton->setObjectName("inputButton");
    progressButton->setObjectName("progressButton");
    tooltipButton->setObjectName("tooltipButton");
    connect(infoButton, &QPushButton::clicked, this, &FeedbackPage::showInfo);
    connect(questionButton, &QPushButton::clicked, this, &FeedbackPage::showQuestion);
    connect(inputButton, &QPushButton::clicked, this, &FeedbackPage::showInput);
    connect(progressButton, &QPushButton::clicked, this, &FeedbackPage::showProgress);
    connect(tooltipButton, &QPushButton::clicked, this, &FeedbackPage::showTooltipNow);
    connect(m_lineEdit, &QLineEdit::textChanged, this, &FeedbackPage::validateText);
    retranslateUi();
}

QString FeedbackPage::pageName() const { return tr("Dialogs && Tips"); }

void FeedbackPage::retranslateUi()
{
    findChild<QPushButton *>("infoButton")->setText(tr("Information"));
    findChild<QPushButton *>("questionButton")->setText(tr("Question"));
    findChild<QPushButton *>("inputButton")->setText(tr("Input"));
    findChild<QPushButton *>("progressButton")->setText(tr("Progress"));
    findChild<QPushButton *>("tooltipButton")->setText(tr("Show Tooltip"));
    m_tipTarget->setText(tr("Hover or click the button to see Tooltip / What's This."));
    validateText();
}

void FeedbackPage::showInfo()
{
    QMessageBox box(QMessageBox::Information, tr("Information"), tr("This message box has details and custom buttons."), QMessageBox::Ok, this);
    box.setDetailedText(tr("Details area: this demonstrates expandable diagnostic information."));
    box.exec();
    appendLog(tr("Information dialog closed"));
}

void FeedbackPage::showQuestion()
{
    const QMessageBox::StandardButton result = QMessageBox::question(this, tr("Confirm"), tr("Do you want to show a toast message?"));
    appendLog(tr("Question result: %1").arg(result == QMessageBox::Yes ? tr("Yes") : tr("No")));
    if (result == QMessageBox::Yes) {
        toast(tr("Toast notification example"));
    }
}

void FeedbackPage::showInput()
{
    bool ok = false;
    const QString text = QInputDialog::getText(this, tr("Input"), tr("Name"), QLineEdit::Normal, m_lineEdit->text(), &ok);
    if (ok) {
        m_lineEdit->setText(text);
        appendLog(tr("Input accepted"));
    }
}

void FeedbackPage::showProgress()
{
    QProgressDialog dialog(tr("Processing items..."), tr("Cancel"), 0, 100, this);
    dialog.setWindowModality(Qt::WindowModal);
    for (int i = 0; i <= 100; i += 10) {
        dialog.setValue(i);
        qApp->processEvents();
        if (dialog.wasCanceled()) {
            appendLog(tr("Progress canceled"));
            return;
        }
        QThread::msleep(20);
    }
    appendLog(tr("Progress finished"));
}

void FeedbackPage::showTooltipNow()
{
    QToolTip::showText(m_tipTarget->mapToGlobal(QPoint(10, m_tipTarget->height())), tr("Manual tooltip near the target widget."), m_tipTarget, QRect(), 3000);
    appendLog(tr("Tooltip shown"));
}

void FeedbackPage::validateText()
{
    const QString text = m_lineEdit->text();
    if (text.size() < 3) {
        m_validationLabel->setText(tr("At least 3 letters are required."));
        m_lineEdit->setStyleSheet("border: 1px solid #b00020;");
    } else {
        m_validationLabel->setText(tr("Input is valid."));
        m_lineEdit->setStyleSheet("border: 1px solid #2e7d32;");
    }
}

void FeedbackPage::toast(const QString &message)
{
    QLabel *label = new QLabel(message, this);
    label->setWindowFlags(Qt::ToolTip);
    label->setStyleSheet("background: #24292f; color: white; padding: 10px 14px; border-radius: 4px;");
    label->move(mapToGlobal(QPoint(width() - 260, 40)));
    label->show();
    QTimer::singleShot(2200, label, &QLabel::deleteLater);
}

void FeedbackPage::appendLog(const QString &message)
{
    m_log->append(QTime::currentTime().toString("hh:mm:ss") + "  " + message);
    Logger::info(QString("FeedbackPage: %1").arg(message));
}
