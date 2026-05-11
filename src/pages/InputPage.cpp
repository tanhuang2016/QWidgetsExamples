#include "InputPage.h"
#include "core/ExceptionHandler.h"
#include "core/Logger.h"

#include <QComboBox>
#include <QDateEdit>
#include <QFormLayout>
#include <QGroupBox>
#include <QLabel>
#include <QLineEdit>
#include <QSlider>
#include <QSpinBox>
#include <QTextEdit>
#include <QVBoxLayout>

InputPage::InputPage(QWidget *parent)
    : ControlPageBase(parent)
{
    QVBoxLayout *root = new QVBoxLayout(this);
    root->setContentsMargins(24, 24, 24, 24);

    m_formGroup = new QGroupBox(this);
    QFormLayout *form = new QFormLayout(m_formGroup);
    m_nameLabel = new QLabel(this);
    m_typeLabel = new QLabel(this);
    m_countLabel = new QLabel(this);
    m_dateLabel = new QLabel(this);
    m_noteLabel = new QLabel(this);
    m_nameEdit = new QLineEdit(this);
    m_typeCombo = new QComboBox(this);
    m_countSpin = new QSpinBox(this);
    m_dateEdit = new QDateEdit(QDate::currentDate(), this);
    m_dateEdit->setCalendarPopup(true);
    m_noteEdit = new QTextEdit(this);
    m_slider = new QSlider(Qt::Horizontal, this);
    m_slider->setRange(0, 100);
    m_slider->setValue(40);
    m_previewLabel = new QLabel(this);
    m_previewLabel->setWordWrap(true);

    m_countSpin->setRange(0, 999);
    m_noteEdit->setMaximumHeight(90);

    form->addRow(m_nameLabel, m_nameEdit);
    form->addRow(m_typeLabel, m_typeCombo);
    form->addRow(m_countLabel, m_countSpin);
    form->addRow(m_dateLabel, m_dateEdit);
    form->addRow(m_noteLabel, m_noteEdit);
    form->addRow(tr("Slider"), m_slider);

    root->addWidget(m_formGroup);
    root->addWidget(m_previewLabel);
    root->addStretch();

    connect(m_nameEdit, &QLineEdit::textChanged, this, &InputPage::updatePreview);
    connect(m_typeCombo, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &InputPage::updatePreview);
    connect(m_countSpin, QOverload<int>::of(&QSpinBox::valueChanged), this, &InputPage::updatePreview);
    connect(m_dateEdit, &QDateEdit::dateChanged, this, &InputPage::updatePreview);
    connect(m_noteEdit, &QTextEdit::textChanged, this, &InputPage::updatePreview);
    connect(m_slider, &QSlider::valueChanged, this, &InputPage::updatePreview);

    retranslateUi();
    Logger::info("InputPage initialized");
}

QString InputPage::pageName() const
{
    return tr("Inputs");
}

void InputPage::retranslateUi()
{
    const QString currentType = m_typeCombo->currentData().toString();
    m_formGroup->setTitle(tr("Form Inputs"));
    m_nameLabel->setText(tr("Name"));
    m_typeLabel->setText(tr("Type"));
    m_countLabel->setText(tr("Count"));
    m_dateLabel->setText(tr("Date"));
    m_noteLabel->setText(tr("Notes"));
    m_nameEdit->setPlaceholderText(tr("Enter text"));

    m_typeCombo->blockSignals(true);
    m_typeCombo->clear();
    m_typeCombo->addItem(tr("Basic"), "basic");
    m_typeCombo->addItem(tr("Advanced"), "advanced");
    m_typeCombo->addItem(tr("Custom"), "custom");
    const int index = m_typeCombo->findData(currentType.isEmpty() ? "basic" : currentType);
    m_typeCombo->setCurrentIndex(index < 0 ? 0 : index);
    m_typeCombo->blockSignals(false);

    updatePreview();
}

void InputPage::updatePreview()
{
    try {
        m_previewLabel->setText(tr("Preview: %1, %2 items, %3, slider %4")
                                .arg(m_nameEdit->text().isEmpty() ? tr("unnamed") : m_nameEdit->text())
                                .arg(m_countSpin->value())
                                .arg(m_typeCombo->currentText())
                                .arg(m_slider->value()));
    } catch (const std::exception &e) {
        ExceptionHandler::handle(e);
    }
}
