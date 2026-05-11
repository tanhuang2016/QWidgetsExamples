#pragma once

#include "ui/ControlPageBase.h"

class ChartCanvas;
class QCheckBox;
class QComboBox;
class QSlider;
class QTextEdit;

struct ChartDataPoint {
    QString label;
    double value;
};

class ChartPage : public ControlPageBase {
    Q_OBJECT

public:
    explicit ChartPage(QWidget *parent = nullptr);
    QString pageName() const override;

public slots:
    void retranslateUi() override;

private slots:
    void applyData();
    void randomData();
    void chooseColor();
    void updateOptions();

private:
    ChartCanvas *m_canvas;
    QTextEdit *m_dataEdit;
    QComboBox *m_typeCombo;
    QCheckBox *m_labelsCheck;
    QCheckBox *m_gridCheck;
    QSlider *m_opacitySlider;
    QColor m_seriesColor;
};

class ChartCanvas : public QWidget {
    Q_OBJECT

public:
    explicit ChartCanvas(QWidget *parent = nullptr);
    void setData(const QVector<ChartDataPoint> &data);
    void setChartType(const QString &type);
    void setShowLabels(bool enabled);
    void setShowGrid(bool enabled);
    void setSeriesColor(const QColor &color);
    void setOpacityValue(int opacity);

protected:
    void paintEvent(QPaintEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;

private:
    QVector<ChartDataPoint> m_data;
    QString m_type;
    bool m_showLabels;
    bool m_showGrid;
    QColor m_seriesColor;
    int m_opacity;
};
