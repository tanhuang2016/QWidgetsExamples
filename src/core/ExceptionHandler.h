#pragma once

#include <QString>
#include <exception>

/**
 * @brief 统一异常处理入口
 *
 * 【为什么需要ExceptionHandler？】
 * C++和Java的异常处理差异：
 * - Java：checked exception（必须捕获或声明抛出）
 * - C++：unchecked exception（编译器不强制捕获）
 * 
 * Qt的特殊性：
 * - Qt的信号槽代码运行在事件循环内部
 * - 异常如果从slot中直接逃逸，行为并不安全
 *   （可能导致事件循环崩溃，程序直接退出）
 * 
 * 【解决方案】
 * 所有UI层的slot都包裹try-catch，统一交给ExceptionHandler处理：
 * 1. 记录异常信息到日志文件
 * 2. 弹出友好的错误提示框（QMessageBox）
 * 3. 避免程序直接崩溃
 * 
 * Java对比：
 * - 类似于Spring的 @ControllerAdvice + @ExceptionHandler
 * - 或者全局的 UncaughtExceptionHandler
 */
class ExceptionHandler {
public:
    /**
     * @brief 处理std::exception异常
     *
     * 【处理流程】
     * 1. 调用 Logger::error() 记录异常信息到日志文件
     * 2. 调用 show() 弹出 QMessageBox::Critical 对话框
     * 
     * Java对比：类似于 logger.error(e.getMessage(), e) + showErrorDialog()
     * 
     * @param e 标准C++异常引用
     */
    static void handle(const std::exception &e);

    /**
     * @brief 向用户显示错误消息
     * 
     * 【实现细节】
     * 使用 QMessageBox::critical() 弹出模态对话框
     * 用户点击"OK"后才能继续操作
     * 
     * Java对比：类似于 JOptionPane.showMessageDialog(null, message, "Error", ERROR_MESSAGE)
     * 
     * @param message 错误消息文本（支持国际化）
     */
    static void show(const QString &message);
};
