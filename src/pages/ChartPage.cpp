#include "ChartPage.h"

#include <QCheckBox>
#include <QColorDialog>
#include <QComboBox>
#include <QFormLayout>
#include <QGroupBox>
#include <QMouseEvent>
#include <QPainter>
#include <QPushButton>
#include <QRandomGenerator>
#include <QSlider>
#include <QTextEdit>
#include <QToolTip>
#include <QVBoxLayout>

ChartPage::ChartPage(QWidget *parent)
    : ControlPageBase(parent),
      m_canvas(new ChartCanvas(this)),
      m_dataEdit(new QTextEdit(this)),
      m_typeCombo(new QComboBox(this)),
      m_labelsCheck(new QCheckBox(this)),
      m_gridCheck(new QCheckBox(this)),
      m_opacitySlider(new QSlider(Qt::Horizontal, this)),
      m_seriesColor("#2f6fed")
{
    QVBoxLayout *root = new QVBoxLayout(this);
    root->setContentsMargins(24, 24, 24, 24);
    m_canvas->setMinimumHeight(320);
    m_dataEdit->setMaximumHeight(110);
    m_dataEdit->setPlainText("Jan,120\nFeb,180\nMar,90\nApr,240\nMay,160");
    m_typeCombo->addItem(tr("Bar"), "bar");
    m_typeCombo->addItem(tr("Horizontal Bar"), "hbar");
    m_typeCombo->addItem(tr("Line"), "line");
    m_typeCombo->addItem(tr("Pie"), "pie");
    m_labelsCheck->setChecked(true);
    m_gridCheck->setChecked(true);
    m_opacitySlider->setRange(20, 100);
    m_opacitySlider->setValue(90);

    QGroupBox *controls = new QGroupBox(this);
    QFormLayout *form = new QFormLayout(controls);
    QPushButton *applyButton = new QPushButton(this);
    QPushButton *randomButton = new QPushButton(this);
    QPushButton *colorButton = new QPushButton(this);
    applyButton->setObjectName("applyButton");
    randomButton->setObjectName("randomButton");
    colorButton->setObjectName("colorButton");
    form->addRow(tr("Chart type"), m_typeCombo);
    form->addRow(tr("Labels"), m_labelsCheck);
    form->addRow(tr("Grid"), m_gridCheck);
    form->addRow(tr("Opacity"), m_opacitySlider);
    form->addRow(tr("Data"), m_dataEdit);
    form->addRow(applyButton, randomButton);
    form->addRow(tr("Series color"), colorButton);

    root->addWidget(m_canvas);
    root->addWidget(controls);

    connect(applyButton, &QPushButton::clicked, this, &ChartPage::applyData);
    connect(randomButton, &QPushButton::clicked, this, &ChartPage::randomData);
    connect(colorButton, &QPushButton::clicked, this, &ChartPage::chooseColor);
    connect(m_typeCombo, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &ChartPage::updateOptions);
    connect(m_labelsCheck, &QCheckBox::toggled, this, &ChartPage::updateOptions);
    connect(m_gridCheck, &QCheckBox::toggled, this, &ChartPage::updateOptions);
    connect(m_opacitySlider, &QSlider::valueChanged, this, &ChartPage::updateOptions);
    retranslateUi();
    applyData();
}

QString ChartPage::pageName() const { return tr("Charts"); }

void ChartPage::retranslateUi()
{
    m_labelsCheck->setText(tr("Show labels"));
    m_gridCheck->setText(tr("Show grid"));
    findChild<QPushButton *>("applyButton")->setText(tr("Apply data"));
    findChild<QPushButton *>("randomButton")->setText(tr("Random data"));
    findChild<QPushButton *>("colorButton")->setText(m_seriesColor.name());
}

void ChartPage::applyData()
{
    QVector<ChartDataPoint> data;
    const QStringList lines = m_dataEdit->toPlainText().split('\n', Qt::SkipEmptyParts);
    for (const QString &line : lines) {
        const QStringList parts = line.split(',');
        if (parts.size() != 2) {
            continue;
        }
        bool ok = false;
        const double value = parts.at(1).trimmed().toDouble(&ok);
        if (ok) {
            data.append(ChartDataPoint{parts.at(0).trimmed(), value});
        }
    }
    if (!data.isEmpty()) {
        m_canvas->setData(data);
    }
    updateOptions();
}

void ChartPage::randomData()
{
    QStringList lines;
    for (int i = 1; i <= 6; ++i) {
        lines << tr("Item %1,%2").arg(i).arg(QRandomGenerator::global()->bounded(50, 260));
    }
    m_dataEdit->setPlainText(lines.join('\n'));
    applyData();
}

void ChartPage::chooseColor()
{
    const QColor color = QColorDialog::getColor(m_seriesColor, this, tr("Series color"), QColorDialog::ShowAlphaChannel);
    if (color.isValid()) {
        m_seriesColor = color;
        findChild<QPushButton *>("colorButton")->setText(m_seriesColor.name(QColor::HexArgb));
        updateOptions();
    }
}

void ChartPage::updateOptions()
{
    m_canvas->setChartType(m_typeCombo->currentData().toString());
    m_canvas->setShowLabels(m_labelsCheck->isChecked());
    m_canvas->setShowGrid(m_gridCheck->isChecked());
    m_canvas->setOpacityValue(m_opacitySlider->value());
    m_canvas->setSeriesColor(m_seriesColor);
}

ChartCanvas::ChartCanvas(QWidget *parent)
    : QWidget(parent),
      m_type("bar"),
      m_showLabels(true),
      m_showGrid(true),
      m_seriesColor("#2f6fed"),
      m_opacity(90)
{
    setMouseTracking(true);
}

void ChartCanvas::setData(const QVector<ChartDataPoint> &data) { m_data = data; update(); }
void ChartCanvas::setChartType(const QString &type) { m_type = type; update(); }
void ChartCanvas::setShowLabels(bool enabled) { m_showLabels = enabled; update(); }
void ChartCanvas::setShowGrid(bool enabled) { m_showGrid = enabled; update(); }
void ChartCanvas::setSeriesColor(const QColor &color) { m_seriesColor = color; update(); }
void ChartCanvas::setOpacityValue(int opacity) { m_opacity = opacity; update(); }

void ChartCanvas::paintEvent(QPaintEvent *)
{
    QPainter p(this);
    p.setRenderHint(QPainter::Antialiasing);
    p.fillRect(rect(), QColor("#ffffff"));
    const QRect area = rect().adjusted(52, 24, -28, -42);
    p.setPen(QColor("#d6d9df"));
    p.drawRect(area);
    if (m_data.isEmpty()) {
        return;
    }
    double maxValue = 1;
    for (const ChartDataPoint &point : m_data) {
        maxValue = qMax(maxValue, point.value);
    }
    if (m_showGrid) {
        for (int i = 1; i < 5; ++i) {
            const int y = area.bottom() - area.height() * i / 5;
            p.drawLine(area.left(), y, area.right(), y);
        }
    }
    QColor color = m_seriesColor;
    color.setAlphaF(m_opacity / 100.0);
    p.setBrush(color);
    p.setPen(QPen(color.darker(), 1));
    if (m_type == "pie") {
        QRect pieRect = area.adjusted(80, 10, -80, -10);
        int start = 0;
        double sum = 0;
        for (const ChartDataPoint &point : m_data) sum += point.value;
        for (int i = 0; i < m_data.size(); ++i) {
            QColor slice = QColor::fromHsv((i * 45) % 360, 170, 220, color.alpha());
            p.setBrush(slice);
            int span = int(360.0 * m_data.at(i).value / sum * 16);
            p.drawPie(pieRect, start, span);
            start += span;
        }
        return;
    }
    const int n = m_data.size();
    const int slot = area.width() / qMax(1, n);
    QPoint last;
    for (int i = 0; i < n; ++i) {
        const ChartDataPoint point = m_data.at(i);
        const int h = int(area.height() * point.value / maxValue);
        if (m_type == "hbar") {
            const int row = area.height() / n;
            QRect bar(area.left(), area.top() + i * row + 8, int(area.width() * point.value / maxValue), row - 14);
            p.drawRect(bar);
            if (m_showLabels) p.drawText(bar.adjusted(4, 0, 120, 0), Qt::AlignVCenter, point.label + " " + QString::number(point.value));
        } else if (m_type == "line") {
            QPoint current(area.left() + slot * i + slot / 2, area.bottom() - h);
            p.setBrush(color);
            p.drawEllipse(current, 4, 4);
            if (i > 0) p.drawLine(last, current);
            last = current;
            if (m_showLabels) p.drawText(current + QPoint(-16, -10), QString::number(point.value));
        } else {
            QRect bar(area.left() + slot * i + slot / 5, area.bottom() - h, slot * 3 / 5, h);
            p.drawRect(bar);
            if (m_showLabels) p.drawText(QRect(bar.left() - 8, bar.top() - 22, bar.width() + 16, 20), Qt::AlignCenter, QString::number(point.value));
            p.drawText(QRect(area.left() + slot * i, area.bottom() + 4, slot, 26), Qt::AlignCenter, point.label);
        }
    }
}

void ChartCanvas::mouseMoveEvent(QMouseEvent *event)
{
    if (m_data.isEmpty()) {
        return;
    }
    const QRect area = rect().adjusted(52, 24, -28, -42);
    const int slot = area.width() / qMax(1, m_data.size());
    const int index = qBound(0, (event->pos().x() - area.left()) / qMax(1, slot), m_data.size() - 1);
    QToolTip::showText(event->globalPos(), QString("%1: %2").arg(m_data.at(index).label).arg(m_data.at(index).value), this);
}
