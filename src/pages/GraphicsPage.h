#pragma once

#include "ui/ControlPageBase.h"

class QGraphicsItem;
class QGraphicsScene;
class QGraphicsView;
class QSlider;
class QSpinBox;
class QPushButton;

class GraphicsPage : public ControlPageBase {
    Q_OBJECT

public:
    explicit GraphicsPage(QWidget *parent = nullptr);
    QString pageName() const override;

public slots:
    void retranslateUi() override;

private slots:
    void addRect();
    void addEllipse();
    void addText();
    void rotateSelected(int value);
    void opacitySelected(int value);
    void chooseFill();
    void resetScene();

private:
    QGraphicsItem *selectedItem() const;
    void styleItem(QGraphicsItem *item);

    QGraphicsScene *m_scene;
    QGraphicsView *m_view;
    QSlider *m_rotationSlider;
    QSlider *m_opacitySlider;
    QColor m_fillColor;
};
