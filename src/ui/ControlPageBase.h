#pragma once

#include <QWidget>

/**
 * @brief 所有右侧示例页面的公共基类（抽象类）
 *
 * 【设计目的】
 * 为所有示例页面提供统一的接口，使得ContentStack可以统一管理不同类型的页面
 * 
 * 【核心功能】
 * 1. pageName(): 获取页面的显示名称（用于导航树）
 * 2. retranslateUi(): 语言切换时刷新页面文本
 * 
 * Java对比：
 * - 类似于 abstract class BasePage 或 interface Page
 * - 或者 Spring MVC中的 Controller 基类
 * 
 * 【继承关系】
 * ControlPageBase -> QWidget -> QObject
 * 所有具体页面（ButtonPage, InputPage等）都继承自这个基类
 */
class ControlPageBase : public QWidget {
    Q_OBJECT  // Qt宏，启用信号槽机制

public:
    /**
     * @brief 构造函数
     * 
     * @param parent 父窗口指针，传递给QWidget
     */
    explicit ControlPageBase(QWidget *parent = nullptr) : QWidget(parent) {}

    /**
     * @brief 纯虚函数：获取页面显示名称
     * 
     * 【重要】所有子类必须实现这个方法
     * 通常返回 tr("页面标题") 以支持国际化
     * 
     * Java对比：类似于 abstract String getPageName()
     * 
     * @return QString 页面的翻译后名称
     */
    virtual QString pageName() const = 0;

public slots:
    /**
     * @brief 纯虚函数：语言变化后重新设置页面上的可见文本
     *
     * 【调用时机】
     * 当用户通过菜单切换语言时，MainWindow会递归调用所有已创建页面的此方法
     * 
     * 【默认实现】
     * 提供空实现（{}），子类可以选择性重写
     * 如果页面没有需要翻译的文本，可以不重写此方法
     * 
     * Java对比：类似于 void retranslateUI() { /* 刷新所有label.setText(tr(...)) * / }
     */
    virtual void retranslateUi() {}
};
