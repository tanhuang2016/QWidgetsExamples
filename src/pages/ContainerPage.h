#pragma once

#include "ui/ControlPageBase.h"

class QMdiArea;
class QTabWidget;
class QTextEdit;

/**
 * @brief 容器类控件示例页。
 *
 * 展示 QTabWidget、QToolBox、QSplitter、QMdiArea。这些控件常用于构建类似
 * IDE 的多面板桌面应用。
 */
class ContainerPage : public ControlPageBase {
    Q_OBJECT

public:
    explicit ContainerPage(QWidget *parent = nullptr);
    /** 返回翻译后的页面标题。 */
    QString pageName() const override;

public slots:
    /** 语言刷新占位函数；本页大部分文本是静态示例数据。 */
    void retranslateUi() override;

private slots:
    /** 添加一个动态可关闭 Tab。 */
    void addTab();
    /** 添加一个 MDI 子窗口。 */
    void addMdiWindow();
    /** 平铺 MDI 子窗口。 */
    void tileMdi();
    /** 层叠 MDI 子窗口。 */
    void cascadeMdi();

private:
    /** 追加界面日志，并写入文件日志。 */
    void appendLog(const QString &message);

    QTabWidget *m_tabs;
    QMdiArea *m_mdi;
    QTextEdit *m_log;
};
