#pragma once

#include <QFile>
#include <QMessageLogContext>
#include <QMutex>
#include <QString>

/**
 * @brief 日志级别枚举
 * 
 * Java对比：类似于 org.slf4j.event.Level 或 java.util.logging.Level
 */
enum class LogLevel {
    Debug,   // 调试信息，仅在开发环境输出
    Info,    // 一般信息，如应用启动、页面加载
    Warning, // 警告信息，不影响正常运行
    Error,   // 错误信息，某项功能失败
    Fatal    // 致命错误，可能导致程序崩溃
};

/**
 * @brief 线程安全的文件日志系统
 *
 * 【功能说明】
 * - 日志写入可执行文件旁边的 logs/app.log
 * - 支持按文件大小自动滚动（避免单个日志文件过大）
 * - Debug模式下同步输出到控制台
 * 
 * Java对比：
 * - 类似于 Log4j2 + RollingFileAppender
 * - 或者 SLF4J + Logback 的文件日志配置
 * 
 * 【使用方式】
 * Logger::info("Application started");
 * Logger::error("Failed to load page");
 * 
 * 【线程安全】
 * 使用QMutex保护并发写入，类似Java的 synchronized 或 ReentrantLock
 */
class Logger {
public:
    /** 
     * @brief 写入DEBUG日志
     * 
     * 用途：记录详细的调试信息，生产环境通常会关闭
     * 示例：Logger::debug("Button clicked, current state: checked=true");
     */
    static void debug(const QString &message);
    
    /** 
     * @brief 写入INFO日志
     * 
     * 用途：记录关键业务流程，如应用启动、页面切换
     * 示例：Logger::info("ButtonPage initialized");
     */
    static void info(const QString &message);
    
    /** 
     * @brief 写入WARNING日志
     * 
     * 用途：记录潜在问题，但不影响功能
     * 示例：Logger::warning("Translation file not found, using default");
     */
    static void warning(const QString &message);
    
    /** 
     * @brief 写入ERROR日志
     * 
     * 用途：记录错误信息，某项操作失败
     * 示例：Logger::error("Failed to create page: invalid key");
     */
    static void error(const QString &message);
    
    /** 
     * @brief 写入FATAL日志
     * 
     * 用途：记录致命错误，通常会导致程序退出
     * 示例：Logger::fatal("Database connection lost");
     */
    static void fatal(const QString &message);
    
    /** 
     * @brief 按指定级别写入日志
     * 
     * @param level 日志级别
     * @param message 日志消息
     */
    static void log(LogLevel level, const QString &message);
    
    /** 
     * @brief Qt消息处理函数（安装给qInstallMessageHandler）
     * 
     * 【重要】这个函数会被Qt框架自动调用
     * 当代码中使用 qDebug()、qWarning()、qCritical() 时
     * 
     * Java对比：类似于配置Logback的 Appender
     * 
     * @param type Qt消息类型（QtDebugMsg, QtWarningMsg等）
     * @param context 消息上下文（文件名、行号等）
     * @param message 消息内容
     */
    static void messageHandler(QtMsgType type, const QMessageLogContext &context, const QString &message);

private:
    // 将LogLevel枚举转换为字符串名称
    static QString levelName(LogLevel level);
    
    // 检查日志文件是否需要滚动（按大小分割）
    static void rotateIfNeeded(const QString &path);
    
    // 返回全局互斥锁引用（保证线程安全）
    static QMutex &mutex();
};
