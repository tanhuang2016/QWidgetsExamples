#include "ExceptionHandler.h"
#include "Logger.h"

#include <QMessageBox>
#include <QObject>

/**
 * @brief 处理std::exception异常
 * 
 * 【处理流程】
 * 1. 调用Logger::error()记录异常信息到日志文件
 *    - 格式："Exception: 异常消息"
 *    - 例如："Exception: Failed to load page 'table'"
 * 
 * 2. 调用show()弹出警告对话框
 *    - 标题："Application Error"（已国际化）
 *    - 内容：异常消息（从e.what()获取）
 * 
 * 【使用场景】
 * 在所有UI层的try-catch块中调用：
 * ```cpp
 * try {
 *     // UI逻辑代码
 * } catch (const std::exception &e) {
 *     ExceptionHandler::handle(e);
 * }
 * ```
 * 
 * Java对比：类似于 logger.error(e.getMessage(), e) + JOptionPane.showMessageDialog()
 * 
 * @param e 标准C++异常引用
 */
void ExceptionHandler::handle(const std::exception &e)
{
    // 【步骤1】记录到日志文件
    Logger::error(QString("Exception: %1").arg(e.what()));
    
    // 【步骤2】显示给用户
    // QString::fromLocal8Bit()将C字符串转换为Qt字符串，支持中文等多字节字符
    show(QString::fromLocal8Bit(e.what()));
}

/**
 * @brief 向用户显示错误消息对话框
 * 
 * 【实现细节】
 * 使用QMessageBox::warning()弹出模态警告对话框
 * - 图标：黄色警告三角形
 * - 按钮：只有一个"OK"按钮
 * - 模态：用户必须点击OK才能继续操作
 * 
 * 【参数说明】
 * - nullptr: 没有父窗口，对话框独立显示
 * - tr("Application Error"): 对话框标题（支持国际化）
 * - message: 错误消息内容
 * 
 * Java对比：类似于 JOptionPane.showMessageDialog(null, message, "Error", WARNING_MESSAGE)
 * 
 * @param message 错误消息文本（支持国际化）
 */
void ExceptionHandler::show(const QString &message)
{
    QMessageBox::warning(nullptr, QObject::tr("Application Error"), message);
}
