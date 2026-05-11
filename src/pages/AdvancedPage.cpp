#include "AdvancedPage.h"
#include "core/Logger.h"

#include <QCompleter>
#include <QFormLayout>
#include <QGroupBox>
#include <QKeySequenceEdit>
#include <QLCDNumber>
#include <QLineEdit>
#include <QPushButton>
#include <QSlider>
#include <QTextEdit>
#include <QTime>
#include <QUndoCommand>
#include <QUndoStack>
#include <QUndoView>
#include <QVBoxLayout>

class LogUndoCommand : public QUndoCommand {
public:
    explicit LogUndoCommand(QTextEdit *log, const QString &text)
        : m_log(log), m_text(text)
    {
        setText(text);
    }

    void undo() override { m_log->append(QTime::currentTime().toString("hh:mm:ss") + "  undo: " + m_text); }
    void redo() override { m_log->append(QTime::currentTime().toString("hh:mm:ss") + "  redo: " + m_text); }

private:
    QTextEdit *m_log;
    QString m_text;
};

AdvancedPage::AdvancedPage(QWidget *parent)
    : ControlPageBase(parent),
      m_lcd(new QLCDNumber(this)),
      m_completerEdit(new QLineEdit(this)),
      m_log(new QTextEdit(this)),
      m_undoStack(new QUndoStack(this))
{
    QVBoxLayout *root = new QVBoxLayout(this);
    root->setContentsMargins(24, 24, 24, 24);

    QGroupBox *controls = new QGroupBox(tr("Advanced Widgets"), this);
    QFormLayout *form = new QFormLayout(controls);
    QSlider *slider = new QSlider(Qt::Horizontal, controls);
    slider->setRange(0, 9999);
    slider->setValue(2026);
    m_lcd->setDigitCount(4);
    m_lcd->display(slider->value());
    QKeySequenceEdit *shortcutEdit = new QKeySequenceEdit(QKeySequence(Qt::CTRL | Qt::ALT | Qt::Key_K), controls);
    m_completerEdit->setCompleter(new QCompleter(QStringList() << "QTableWidget" << "QMenuBar" << "QToolBar" << "QGraphicsView" << "QMessageBox", this));
    QPushButton *pushCommand = new QPushButton(tr("Push undo command"), controls);
    QUndoView *undoView = new QUndoView(m_undoStack, controls);
    undoView->setMaximumHeight(120);

    form->addRow(tr("LCD Number"), m_lcd);
    form->addRow(tr("LCD Value"), slider);
    form->addRow(tr("Key Sequence"), shortcutEdit);
    form->addRow(tr("Completer"), m_completerEdit);
    form->addRow(pushCommand);
    form->addRow(tr("Undo View"), undoView);

    m_log->setReadOnly(true);
    m_log->setMaximumHeight(100);
    root->addWidget(controls);
    root->addWidget(m_log);
    root->addStretch();

    connect(slider, &QSlider::valueChanged, this, &AdvancedPage::updateLcd);
    connect(pushCommand, &QPushButton::clicked, this, &AdvancedPage::pushUndoCommand);
}

QString AdvancedPage::pageName() const { return tr("Advanced"); }

void AdvancedPage::retranslateUi()
{
}

void AdvancedPage::updateLcd(int value)
{
    m_lcd->display(value);
    Logger::info(QString("AdvancedPage LCD value: %1").arg(value));
}

void AdvancedPage::pushUndoCommand()
{
    m_undoStack->push(new LogUndoCommand(m_log, tr("Command %1").arg(m_undoStack->count() + 1)));
}
