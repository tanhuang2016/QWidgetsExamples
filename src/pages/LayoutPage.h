#pragma once

#include "ui/ControlPageBase.h"

#include <QVector>

class QGroupBox;
class QLabel;

class LayoutPage : public ControlPageBase {
    Q_OBJECT

public:
    explicit LayoutPage(QWidget *parent = nullptr);
    QString pageName() const override;

public slots:
    void retranslateUi() override;

private:
    QGroupBox *m_gridGroup;
    QGroupBox *m_boxGroup;
    QVector<QLabel *> m_gridLabels;
    QLabel *m_boxLabel;
};
