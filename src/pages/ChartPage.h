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

/**
 * @brief 自绘图表示例页。
 *
 * 为了避免依赖可选的 QtCharts 模块，本项目用 QPainter 手动画简单图表。
 * 这里展示柱状图、条形图、折线图、饼图的基本绘制方式。
 */
class ChartPage : public ControlPageBase {
    Q_OBJECT

public:
    explicit ChartPage(QWidget *parent = nullptr);
    /** 返回翻译后的页面标题。 */
    QString pageName() const override;

public slots:
    /** 语言切换后重新应用标签文本。 */
    void retranslateUi() override;

private slots:
    /** 解析用户输入的类 CSV 数据，并更新画布。 */
    void applyData();
    /** 生成随机图表数据。 */
    void randomData();
    /** 打开 QColorDialog 选择数据系列颜色。 */
    void chooseColor();
    /** 把图表类型和选项同步到 ChartCanvas。 */
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

/**
 * @brief 使用 QPainter 手动绘制图表的 QWidget。
 *
 * 自定义 QWidget 通常通过重写 paintEvent() 来完成绘制。你可以把它理解成
 * 其他 GUI 框架里的“自绘控件”。
 */
class ChartCanvas : public QWidget {
    Q_OBJECT

public:
    explicit ChartCanvas(QWidget *parent = nullptr);
    /** 设置图表数据点。 */
    void setData(const QVector<ChartDataPoint> &data);
    /** 设置图表类型：bar、hbar、line、pie。 */
    void setChartType(const QString &type);
    /** 显示或隐藏数值标签。 */
    void setShowLabels(bool enabled);
    /** 显示或隐藏网格线。 */
    void setShowGrid(bool enabled);
    /** 设置主数据系列颜色。 */
    void setSeriesColor(const QColor &color);
    /** 设置数据系列透明度百分比。 */
    void setOpacityValue(int opacity);

protected:
    /** 绘制图表。Qt 在控件需要重绘时会调用这个函数。 */
    void paintEvent(QPaintEvent *event) override;
    /** 为鼠标附近的数据点显示 Tooltip。 */
    void mouseMoveEvent(QMouseEvent *event) override;

private:
    QVector<ChartDataPoint> m_data;
    QString m_type;
    bool m_showLabels;
    bool m_showGrid;
    QColor m_seriesColor;
    int m_opacity;
};
