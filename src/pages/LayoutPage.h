#pragma once

#include "ui/ControlPageBase.h"

#include <QVector>

class QGroupBox;
class QLabel;

/**
 * @brief 布局管理器示例页。
 *
 * 展示网格布局、盒式布局、表单布局。在 Qt Widgets 中，布局管理器是构建
 * 可伸缩界面的常规方式，绝对定位一般很少使用。
 */
class LayoutPage : public ControlPageBase {
    Q_OBJECT

public:
    explicit LayoutPage(QWidget *parent = nullptr);
    /** 返回翻译后的页面标题。 */
    QString pageName() const override;

public slots:
    /** 语言切换后重新应用分组标题和单元格文本。 */
    void retranslateUi() override;

private:
    QGroupBox *m_gridGroup;
    QGroupBox *m_boxGroup;
    QVector<QLabel *> m_gridLabels;
    QLabel *m_boxLabel;
};
