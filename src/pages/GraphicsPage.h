#pragma once

#include "ui/ControlPageBase.h"

class QGraphicsItem;
class QGraphicsScene;
class QGraphicsView;
class QSlider;
class QSpinBox;
class QPushButton;

/**
 * @brief QGraphicsView 场景编辑示例页。
 *
 * 展示如何创建可移动、可选中的 QGraphicsItem，并修改选中图元的旋转、
 * 透明度、填充颜色等属性。
 */
class GraphicsPage : public ControlPageBase {
    Q_OBJECT

public:
    explicit GraphicsPage(QWidget *parent = nullptr);
    /** 返回翻译后的页面标题。 */
    QString pageName() const override;

public slots:
    /** 语言切换后重新应用按钮文本。 */
    void retranslateUi() override;

private slots:
    /** 添加一个可选中的矩形图元。 */
    void addRect();
    /** 添加一个可选中的椭圆图元。 */
    void addEllipse();
    /** 添加一个可选中的文本图元。 */
    void addText();
    /** 旋转当前选中的图元。 */
    void rotateSelected(int value);
    /** 修改当前选中图元的透明度。 */
    void opacitySelected(int value);
    /** 打开 QColorDialog 并应用填充色。 */
    void chooseFill();
    /** 清空并恢复初始场景图元。 */
    void resetScene();

private:
    /** 返回第一个被选中的图元；如果没有选中则返回 nullptr。 */
    QGraphicsItem *selectedItem() const;
    /** 给图元启用移动、选择标志，并设置 Tooltip。 */
    void styleItem(QGraphicsItem *item);

    QGraphicsScene *m_scene;
    QGraphicsView *m_view;
    QSlider *m_rotationSlider;
    QSlider *m_opacitySlider;
    QColor m_fillColor;
};
