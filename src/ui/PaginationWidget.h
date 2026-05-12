#pragma once

#include <QWidget>

class QLabel;
class QPushButton;
class QSpinBox;
class QComboBox;

/**
 * @brief 可复用的分页条控件。
 *
 * 这个类演示如何用多个标准控件组合成一个自定义 QWidget。TablePage、
 * PaginationPage 这类页面连接 pageChanged(page, pageSize) 信号后，
 * 就可以按当前页和每页条数刷新数据。
 */
class PaginationWidget : public QWidget {
    Q_OBJECT

public:
    explicit PaginationWidget(QWidget *parent = nullptr);
    /** 当前页码，从 1 开始。 */
    int currentPage() const;
    /** 每页显示多少条记录。 */
    int pageSize() const;
    /** 绑定数据源的总记录数。 */
    int totalCount() const;
    /** 总页数，最少为 1。 */
    int pageCount() const;

public slots:
    /** 更新总记录数，并刷新按钮状态。 */
    void setTotalCount(int totalCount);
    /** 跳转到指定页，同时自动限制到有效范围。 */
    void setCurrentPage(int page);
    /** 重新应用按钮翻译文本。 */
    void retranslateUi();

signals:
    /** 用户切换页码或每页条数时发出。 */
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
