#pragma once

#include <QFile>
#include <QMessageLogContext>
#include <QMutex>
#include <QString>

enum class LogLevel {
    Debug,
    Info,
    Warning,
    Error,
    Fatal
};

/**
 * @brief 示例程序使用的简单线程安全文件日志。
 *
 * 日志写入可执行文件旁边的 logs/app.log。main() 中通过
 * qInstallMessageHandler() 把 Qt 自带的 qDebug/qWarning/qCritical
 * 等消息也转发到这里。
 */
class Logger {
public:
    /** 写入 DEBUG 日志。 */
    static void debug(const QString &message);
    /** 写入 INFO 日志。 */
    static void info(const QString &message);
    /** 写入 WARNING 日志。 */
    static void warning(const QString &message);
    /** 写入 ERROR 日志。 */
    static void error(const QString &message);
    /** 写入 FATAL 日志。 */
    static void fatal(const QString &message);
    /** 按指定级别写入日志。 */
    static void log(LogLevel level, const QString &message);
    /** 安装给 qInstallMessageHandler() 的 Qt 消息处理函数。 */
    static void messageHandler(QtMsgType type, const QMessageLogContext &context, const QString &message);

private:
    static QString levelName(LogLevel level);
    static void rotateIfNeeded(const QString &path);
    static QMutex &mutex();
};
