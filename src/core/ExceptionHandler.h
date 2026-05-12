#pragma once

#include <QString>
#include <exception>

/**
 * @brief 统一异常处理入口。
 *
 * Qt 的信号槽代码通常运行在事件循环内部。异常如果从 slot 中直接逃逸，
 * 行为并不安全，所以页面里捕获异常后统一交给这里记录日志并提示用户。
 */
class ExceptionHandler {
public:
    /**
     * @brief 记录 std::exception，并弹出友好的错误提示框。
     */
    static void handle(const std::exception &e);

    /**
     * @brief 向用户显示错误消息。
     */
    static void show(const QString &message);
};
