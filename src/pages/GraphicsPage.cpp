#include "GraphicsPage.h"

#include <QColorDialog>
#include <QFormLayout>
#include <QGraphicsEllipseItem>
#include <QGraphicsRectItem>
#include <QGraphicsScene>
#include <QGraphicsTextItem>
#include <QGraphicsView>
#include <QGroupBox>
#include <QPushButton>
#include <QSlider>
#include <QVBoxLayout>

GraphicsPage::GraphicsPage(QWidget *parent)
    : ControlPageBase(parent),
      m_scene(new QGraphicsScene(this)),
      m_view(new QGraphicsView(m_scene, this)),
      m_rotationSlider(new QSlider(Qt::Horizontal, this)),
      m_opacitySlider(new QSlider(Qt::Horizontal, this)),
      m_fillColor("#2f6fed")
{
    QVBoxLayout *root = new QVBoxLayout(this);
    root->setContentsMargins(24, 24, 24, 24);
    m_scene->setSceneRect(0, 0, 760, 360);
    m_view->setRenderHint(QPainter::Antialiasing);
    m_view->setDragMode(QGraphicsView::RubberBandDrag);

    QGroupBox *toolbar = new QGroupBox(this);
    QHBoxLayout *bar = new QHBoxLayout(toolbar);
    QPushButton *rectButton = new QPushButton(this);
    QPushButton *ellipseButton = new QPushButton(this);
    QPushButton *textButton = new QPushButton(this);
    QPushButton *colorButton = new QPushButton(this);
    QPushButton *resetButton = new QPushButton(this);
    rectButton->setObjectName("rectButton");
    ellipseButton->setObjectName("ellipseButton");
    textButton->setObjectName("textButton");
    colorButton->setObjectName("colorButton");
    resetButton->setObjectName("resetButton");
    bar->addWidget(rectButton);
    bar->addWidget(ellipseButton);
    bar->addWidget(textButton);
    bar->addWidget(colorButton);
    bar->addWidget(resetButton);
    bar->addStretch();

    QGroupBox *controls = new QGroupBox(this);
    QFormLayout *form = new QFormLayout(controls);
    m_rotationSlider->setRange(0, 360);
    m_opacitySlider->setRange(10, 100);
    m_opacitySlider->setValue(100);
    form->addRow(tr("Rotation"), m_rotationSlider);
    form->addRow(tr("Opacity"), m_opacitySlider);

    root->addWidget(toolbar);
    root->addWidget(m_view, 1);
    root->addWidget(controls);

    connect(rectButton, &QPushButton::clicked, this, &GraphicsPage::addRect);
    connect(ellipseButton, &QPushButton::clicked, this, &GraphicsPage::addEllipse);
    connect(textButton, &QPushButton::clicked, this, &GraphicsPage::addText);
    connect(colorButton, &QPushButton::clicked, this, &GraphicsPage::chooseFill);
    connect(resetButton, &QPushButton::clicked, this, &GraphicsPage::resetScene);
    connect(m_rotationSlider, &QSlider::valueChanged, this, &GraphicsPage::rotateSelected);
    connect(m_opacitySlider, &QSlider::valueChanged, this, &GraphicsPage::opacitySelected);
    retranslateUi();
    resetScene();
}

QString GraphicsPage::pageName() const { return tr("Graphics View"); }

void GraphicsPage::retranslateUi()
{
    findChild<QPushButton *>("rectButton")->setText(tr("Add Rect"));
    findChild<QPushButton *>("ellipseButton")->setText(tr("Add Ellipse"));
    findChild<QPushButton *>("textButton")->setText(tr("Add Text"));
    findChild<QPushButton *>("colorButton")->setText(tr("Fill Color"));
    findChild<QPushButton *>("resetButton")->setText(tr("Reset"));
}

void GraphicsPage::addRect()
{
    QGraphicsRectItem *item = m_scene->addRect(40, 40, 120, 80, QPen(Qt::black, 2), QBrush(m_fillColor));
    styleItem(item);
}

void GraphicsPage::addEllipse()
{
    QGraphicsEllipseItem *item = m_scene->addEllipse(210, 60, 110, 90, QPen(Qt::black, 2), QBrush(m_fillColor.lighter()));
    styleItem(item);
}

void GraphicsPage::addText()
{
    QGraphicsTextItem *item = m_scene->addText(tr("Drag / rotate / fade"));
    item->setPos(380, 90);
    item->setDefaultTextColor(m_fillColor.darker());
    styleItem(item);
}

void GraphicsPage::rotateSelected(int value)
{
    if (QGraphicsItem *item = selectedItem()) {
        item->setTransformOriginPoint(item->boundingRect().center());
        item->setRotation(value);
    }
}

void GraphicsPage::opacitySelected(int value)
{
    if (QGraphicsItem *item = selectedItem()) {
        item->setOpacity(value / 100.0);
    }
}

void GraphicsPage::chooseFill()
{
    const QColor color = QColorDialog::getColor(m_fillColor, this, tr("Fill Color"), QColorDialog::ShowAlphaChannel);
    if (color.isValid()) {
        m_fillColor = color;
        if (QGraphicsItem *item = selectedItem()) {
            if (QAbstractGraphicsShapeItem *shape = dynamic_cast<QAbstractGraphicsShapeItem *>(item)) {
                shape->setBrush(QBrush(m_fillColor));
            }
        }
    }
}

void GraphicsPage::resetScene()
{
    m_scene->clear();
    addRect();
    addEllipse();
    addText();
}

QGraphicsItem *GraphicsPage::selectedItem() const
{
    const QList<QGraphicsItem *> items = m_scene->selectedItems();
    return items.isEmpty() ? nullptr : items.first();
}

void GraphicsPage::styleItem(QGraphicsItem *item)
{
    item->setFlags(QGraphicsItem::ItemIsMovable | QGraphicsItem::ItemIsSelectable | QGraphicsItem::ItemSendsGeometryChanges);
    item->setToolTip(tr("Select this item, then change rotation, opacity or color."));
}
