#pragma once

#include <QWidget>

class ControlPageBase : public QWidget {
    Q_OBJECT

public:
    explicit ControlPageBase(QWidget *parent = nullptr) : QWidget(parent) {}
    virtual QString pageName() const = 0;

public slots:
    virtual void retranslateUi() {}
};
