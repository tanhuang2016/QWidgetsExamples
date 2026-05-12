#include "Logger.h"

#include <QCoreApplication>
#include <QDateTime>
#include <QDebug>
#include <QDir>
#include <QFileInfo>
#include <QTextStream>

/**
 * @brief 匿名命名空间：存放内部辅助函数和常量
 * 
 * 【C++特性】
 * 匿名命名空间中的符号只在当前文件可见，类似Java的private static方法
 * 这是一种封装实现细节的良好实践
 */
namespace {
// 日志文件最大大小：1MB（超过此大小会自动滚动）
const qint64 MaxLogSize = 1024 * 1024;

/**
 * @brief 获取日志文件的完整路径
 * 
 * 【路径选择】
 * 日志文件放在可执行文件旁边的logs目录下，而不是源码目录
 * 这样做的好处：
 * 1. 本地开发时可以正常写入
 * 2. 安装后的应用程序也能正常工作
 * 3. 便携式应用（绿色版）可以随身携带日志
 * 
 * Java对比：类似于 System.getProperty("user.dir") + "/logs/app.log"
 * 
 * @return QString 日志文件的绝对路径
 */
QString logPath()
{
    // 获取可执行文件所在目录
    QDir dir(QCoreApplication::applicationDirPath());
    
    // 如果logs目录不存在，则创建它
    if (!dir.exists("logs")) {
        dir.mkpath("logs");  // 递归创建目录
    }
    
    // 返回完整路径：例如 "E:/code/cpp/qt_codex1/build/logs/app.log"
    return dir.filePath("logs/app.log");
}
}

// === 便捷日志方法 ===
// 这些方法都是log()的包装器，简化调用
void Logger::debug(const QString &message) { log(LogLevel::Debug, message); }
void Logger::info(const QString &message) { log(LogLevel::Info, message); }
void Logger::warning(const QString &message) { log(LogLevel::Warning, message); }
void Logger::error(const QString &message) { log(LogLevel::Error, message); }
void Logger::fatal(const QString &message) { log(LogLevel::Fatal, message); }

/**
 * @brief 核心日志写入方法
 * 
 * 【线程安全】
 * 使用QMutexLocker自动加锁/解锁，防止多线程同时写入导致日志混乱
 * Java对比：类似于 synchronized块 或 ReentrantLock.lock()
 * 
 * 【工作流程】
 * 1. 获取互斥锁（自动管理，离开作用域时自动释放）
 * 2. 获取日志文件路径
 * 3. 检查是否需要滚动（文件大小超过1MB）
 * 4. 打开文件并追加写入
 * 5. 格式化日志行：[时间] [级别] 消息
 * 6. Debug模式下同时输出到控制台
 * 
 * 【日志格式】
 * [2026-05-12 14:30:00] [INFO] Application started
 * 
 * @param level 日志级别
 * @param message 日志消息
 */
void Logger::log(LogLevel level, const QString &message)
{
    // 【线程安全】QMutexLocker在构造时加锁，析构时自动解锁（RAII模式）
    // Java对比：类似于 try (Lock lock = mutex.lock()) { ... }
    QMutexLocker locker(&mutex());
    
    const QString path = logPath();  // 获取日志文件路径
    rotateIfNeeded(path);            // 检查是否需要滚动

    // 【打开文件】
    // QIODevice::WriteOnly: 只写模式
    // QIODevice::Append: 追加模式（不会覆盖已有内容）
    // QIODevice::Text: 文本模式（自动处理换行符）
    QFile file(path);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Append | QIODevice::Text)) {
        // 如果打开失败，静默返回（避免日志系统本身导致崩溃）
        return;
    }

    // 【格式化日志行】
    // 格式：[yyyy-MM-dd hh:mm:ss] [LEVEL] message
    const QString line = QString("[%1] [%2] %3")
            .arg(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss"))  // 时间戳
            .arg(levelName(level), message);  // 级别和消息

    // 【写入文件】
    QTextStream out(&file);
    out << line << '\n';  // 写入一行日志

    // 【控制台输出】
    // 仅在Debug模式下输出到控制台，方便开发调试
    // Release模式下只写入文件，减少控制台噪音
#ifdef QT_DEBUG
    QTextStream console(stdout);
    console << line << Qt::endl;  // Qt::endl会刷新缓冲区
#endif
}

/**
 * @brief Qt消息处理函数（全局钩子）
 * 
 * 【工作原理】
 * main()中通过qInstallMessageHandler()安装此函数后
 * 所有qDebug()、qWarning()、qCritical()等调用都会转发到这里
 * 
 * 【映射关系】
 * QtDebugMsg     -> Logger::debug()
 * QtInfoMsg      -> Logger::info()
 * QtWarningMsg   -> Logger::warning()
 * QtCriticalMsg  -> Logger::error()
 * QtFatalMsg     -> Logger::fatal() + abort()
 * 
 * 【特殊处理】
 * QtFatalMsg会调用abort()终止程序
 * 这是Qt的默认行为，确保致命错误不会被忽略
 * 
 * Java对比：类似于配置Logback的 Appender
 * 
 * @param type Qt消息类型
 * @param context 消息上下文（文件名、行号、函数名等，此处未使用）
 * @param message 消息内容
 */
void Logger::messageHandler(QtMsgType type, const QMessageLogContext &, const QString &message)
{
    // 根据Qt消息类型分发到对应的日志级别
    switch (type) {
    case QtDebugMsg:
        debug(message);
        break;
    case QtInfoMsg:
        info(message);
        break;
    case QtWarningMsg:
        warning(message);
        break;
    case QtCriticalMsg:
        error(message);
        break;
    case QtFatalMsg:
        fatal(message);
        abort();  // 致命错误：终止程序
    }
}

/**
 * @brief 将LogLevel枚举转换为字符串名称
 * 
 * @param level 日志级别枚举值
 * @return QString 对应的字符串名称（"DEBUG"、"INFO"等）
 */
QString Logger::levelName(LogLevel level)
{
    switch (level) {
    case LogLevel::Debug:
        return "DEBUG";
    case LogLevel::Info:
        return "INFO";
    case LogLevel::Warning:
        return "WARNING";
    case LogLevel::Error:
        return "ERROR";
    case LogLevel::Fatal:
        return "FATAL";
    }
    return "INFO";  // 默认返回值（防止编译器警告）
}

/**
 * @brief 检查日志文件是否需要滚动（按大小分割）
 * 
 * 【滚动策略】
 * 当日志文件超过MaxLogSize（1MB）时：
 * 1. 重命名当前文件为 app.log.yyyyMMddhhmmss（带时间戳备份）
 * 2. 下次写入时会自动创建新的app.log
 * 
 * 【优点】
 * - 避免单个日志文件无限增长
 * - 保留历史日志供排查问题
 * - 按时间戳命名，便于查找特定时间的日志
 * 
 * Java对比：类似于 Logback的 RollingFileAppender + SizeBasedTriggeringPolicy
 * 
 * @param path 日志文件路径
 */
void Logger::rotateIfNeeded(const QString &path)
{
    // 简单的基于大小的滚动策略
    QFileInfo info(path);
    
    // 如果文件不存在，或者大小未达到阈值，不需要滚动
    if (!info.exists() || info.size() < MaxLogSize) {
        return;
    }

    // 【滚动操作】
    // 生成备份文件名：app.log.20260512143000
    const QString backupPath = path + "." + QDateTime::currentDateTime().toString("yyyyMMddhhmmss");
    
    // 重命名当前文件为备份文件
    QFile::rename(path, backupPath);
    
    // 注意：这里不需要创建新文件，下次log()调用时会自动创建
}

/**
 * @brief 获取全局互斥锁引用（单例模式）
 * 
 * 【线程安全】
 * 使用静态局部变量，C++11保证初始化是线程安全的
 * 
 * 【为什么需要互斥锁？】
 * 多个线程可能同时调用Logger::info()等方法
 * 如果没有锁保护，会导致：
 * 1. 日志内容交错混乱
 * 2. 文件写入冲突
 * 3. 数据损坏
 * 
 * Java对比：类似于 private static final ReentrantLock lock = new ReentrantLock()
 * 
 * @return QMutex& 全局互斥锁的引用
 */
QMutex &Logger::mutex()
{
    static QMutex value;  // Meyers Singleton模式
    return value;
}
