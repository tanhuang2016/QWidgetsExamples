#pragma once

#include <QWidget>

class QLabel;
class QPushButton;
class QSpinBox;
class QComboBox;

class PaginationWidget : public QWidget {
    Q_OBJECT

public:
    explicit PaginationWidget(QWidget *parent = nullptr);
    int currentPage() const;
    int pageSize() const;
    int totalCount() const;
    int pageCount() const;

public slots:
    void setTotalCount(int totalCount);
    void setCurrentPage(int page);
    void retranslateUi();

signals:
    void pageChanged(int page, int pageSize);

private slots:
    void first();
    void previous();
    void next();
    void last();
    void jump();
    void pageSizeChanged();

private:
    void updateState(bool emitChange);

    QPushButton *m_firstButton;
    QPushButton *m_prevButton;
    QPushButton *m_nextButton;
    QPushButton *m_lastButton;
    QPushButton *m_jumpButton;
    QLabel *m_infoLabel;
    QLabel *m_pageSizeLabel;
    QSpinBox *m_pageSpin;
    QComboBox *m_pageSizeCombo;
    int m_totalCount;
    int m_currentPage;
};
