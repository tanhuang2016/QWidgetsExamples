#include "StylePage.h"

#include <QCheckBox>
#include <QColorDialog>
#include <QComboBox>
#include <QFontComboBox>
#include <QFormLayout>
#include <QGroupBox>
#include <QLabel>
#include <QPushButton>
#include <QSlider>
#include <QSpinBox>
#include <QTextEdit>
#include <QVBoxLayout>

StylePage::StylePage(QWidget *parent)
    : ControlPageBase(parent),
      m_previewLabel(new QLabel(this)),
      m_previewButton(new QPushButton(this)),
      m_qssPreview(new QTextEdit(this)),
      m_fontCombo(new QFontComboBox(this)),
      m_fontSizeSpin(new QSpinBox(this)),
      m_boldCheck(new QCheckBox(this)),
      m_italicCheck(new QCheckBox(this)),
      m_radiusSpin(new QSpinBox(this)),
      m_borderSpin(new QSpinBox(this)),
      m_opacitySlider(new QSlider(Qt::Horizontal, this)),
      m_alignCombo(new QComboBox(this)),
      m_textColorButton(new QPushButton(this)),
      m_backgroundButton(new QPushButton(this)),
      m_textColor(Qt::white),
      m_backgroundColor("#2f6fed")
{
    QVBoxLayout *root = new QVBoxLayout(this);
    root->setContentsMargins(24, 24, 24, 24);

    QGroupBox *preview = new QGroupBox(this);
    QVBoxLayout *previewLayout = new QVBoxLayout(preview);
    m_previewLabel->setMinimumHeight(90);
    m_previewLabel->setAlignment(Qt::AlignCenter);
    m_previewButton->setMinimumHeight(36);
    previewLayout->addWidget(m_previewLabel);
    previewLayout->addWidget(m_previewButton);

    QGroupBox *controls = new QGroupBox(this);
    QFormLayout *form = new QFormLayout(controls);
    m_fontSizeSpin->setRange(8, 48);
    m_fontSizeSpin->setValue(18);
    m_radiusSpin->setRange(0, 40);
    m_radiusSpin->setValue(8);
    m_borderSpin->setRange(0, 8);
    m_borderSpin->setValue(1);
    m_opacitySlider->setRange(20, 100);
    m_opacitySlider->setValue(100);
    m_alignCombo->addItem(tr("Left"), int(Qt::AlignLeft | Qt::AlignVCenter));
    m_alignCombo->addItem(tr("Center"), int(Qt::AlignCenter));
    m_alignCombo->addItem(tr("Right"), int(Qt::AlignRight | Qt::AlignVCenter));
    form->addRow(tr("Font"), m_fontCombo);
    form->addRow(tr("Font size"), m_fontSizeSpin);
    form->addRow(tr("Bold"), m_boldCheck);
    form->addRow(tr("Italic"), m_italicCheck);
    form->addRow(tr("Text color"), m_textColorButton);
    form->addRow(tr("Background"), m_backgroundButton);
    form->addRow(tr("Radius"), m_radiusSpin);
    form->addRow(tr("Border"), m_borderSpin);
    form->addRow(tr("Opacity"), m_opacitySlider);
    form->addRow(tr("Align"), m_alignCombo);
    QPushButton *reset = new QPushButton(this);
    reset->setObjectName("resetButton");
    form->addRow(QString(), reset);

    m_qssPreview->setReadOnly(false);
    m_qssPreview->setMaximumHeight(130);

    root->addWidget(preview);
    root->addWidget(m_qssPreview);
    root->addWidget(controls);

    connect(m_fontCombo, &QFontComboBox::currentFontChanged, this, &StylePage::applyStyle);
    connect(m_fontSizeSpin, QOverload<int>::of(&QSpinBox::valueChanged), this, &StylePage::applyStyle);
    connect(m_boldCheck, &QCheckBox::toggled, this, &StylePage::applyStyle);
    connect(m_italicCheck, &QCheckBox::toggled, this, &StylePage::applyStyle);
    connect(m_radiusSpin, QOverload<int>::of(&QSpinBox::valueChanged), this, &StylePage::applyStyle);
    connect(m_borderSpin, QOverload<int>::of(&QSpinBox::valueChanged), this, &StylePage::applyStyle);
    connect(m_opacitySlider, &QSlider::valueChanged, this, &StylePage::applyStyle);
    connect(m_alignCombo, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &StylePage::applyStyle);
    connect(m_textColorButton, &QPushButton::clicked, this, &StylePage::chooseTextColor);
    connect(m_backgroundButton, &QPushButton::clicked, this, &StylePage::chooseBackgroundColor);
    connect(reset, &QPushButton::clicked, this, &StylePage::resetStyle);
    retranslateUi();
    applyStyle();
}

QString StylePage::pageName() const { return tr("Style Controls"); }

void StylePage::retranslateUi()
{
    m_previewLabel->setText(tr("Font, size, color, radius and opacity preview"));
    m_previewButton->setText(tr("Styled Button"));
    m_boldCheck->setText(tr("Enabled"));
    m_italicCheck->setText(tr("Enabled"));
    findChild<QPushButton *>("resetButton")->setText(tr("Reset style"));
    m_textColorButton->setText(m_textColor.name());
    m_backgroundButton->setText(m_backgroundColor.name());
}

void StylePage::chooseTextColor()
{
    const QColor color = QColorDialog::getColor(m_textColor, this, tr("Text color"), QColorDialog::ShowAlphaChannel);
    if (color.isValid()) {
        m_textColor = color;
        applyStyle();
    }
}

void StylePage::chooseBackgroundColor()
{
    const QColor color = QColorDialog::getColor(m_backgroundColor, this, tr("Background"), QColorDialog::ShowAlphaChannel);
    if (color.isValid()) {
        m_backgroundColor = color;
        applyStyle();
    }
}

void StylePage::applyStyle()
{
    QFont font = m_fontCombo->currentFont();
    font.setPointSize(m_fontSizeSpin->value());
    font.setBold(m_boldCheck->isChecked());
    font.setItalic(m_italicCheck->isChecked());
    m_previewLabel->setFont(font);
    m_previewButton->setFont(font);
    m_previewLabel->setAlignment(Qt::Alignment(m_alignCombo->currentData().toInt()));

    QColor background = m_backgroundColor;
    background.setAlphaF(m_opacitySlider->value() / 100.0);
    const QString qss = QString("color: %1; background-color: rgba(%2,%3,%4,%5); border: %6px solid #26364d; border-radius: %7px; padding: 10px;")
            .arg(m_textColor.name())
            .arg(background.red()).arg(background.green()).arg(background.blue()).arg(background.alpha())
            .arg(m_borderSpin->value()).arg(m_radiusSpin->value());
    m_previewLabel->setStyleSheet(qss);
    m_previewButton->setStyleSheet(qss);
    m_qssPreview->setPlainText(qss);
    m_textColorButton->setText(m_textColor.name(QColor::HexArgb));
    m_backgroundButton->setText(m_backgroundColor.name(QColor::HexArgb));
}

void StylePage::resetStyle()
{
    m_textColor = Qt::white;
    m_backgroundColor = QColor("#2f6fed");
    m_fontSizeSpin->setValue(18);
    m_boldCheck->setChecked(false);
    m_italicCheck->setChecked(false);
    m_radiusSpin->setValue(8);
    m_borderSpin->setValue(1);
    m_opacitySlider->setValue(100);
    applyStyle();
}
