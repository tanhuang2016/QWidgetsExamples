#pragma once

#include <QWidget>

/**
 * @brief 所有右侧示例页面的公共基类。
 *
 * 每个页面本质上都是 QWidget，因此可以放进 QStackedWidget。这个基类
 * 为 MainWindow / ContentStack 提供统一接口：获取页面名、刷新翻译文本。
 */
class ControlPageBase : public QWidget {
    Q_OBJECT

public:
    explicit ControlPageBase(QWidget *parent = nullptr) : QWidget(parent) {}

    /**
     * @brief 页面显示名称，通常用 tr() 返回可翻译文本。
     */
    virtual QString pageName() const = 0;

public slots:
    /**
     * @brief 语言变化后重新设置页面上的可见文本。
     *
     * Qt 会向控件发送 LanguageChange 事件，但本项目页面是动态创建的，
     * 因此统一调用这个虚函数会更清晰。
     */
    virtual void retranslateUi() {}
};
