#include "Logger.h"

#include <QCoreApplication>
#include <QDateTime>
#include <QDebug>
#include <QDir>
#include <QFileInfo>
#include <QTextStream>

namespace {
const qint64 MaxLogSize = 1024 * 1024;

QString logPath()
{
    // Logs are placed beside the executable, not in the source tree. This works
    // for local builds, installed apps and portable archives.
    QDir dir(QCoreApplication::applicationDirPath());
    if (!dir.exists("logs")) {
        dir.mkpath("logs");
    }
    return dir.filePath("logs/app.log");
}
}

void Logger::debug(const QString &message) { log(LogLevel::Debug, message); }
void Logger::info(const QString &message) { log(LogLevel::Info, message); }
void Logger::warning(const QString &message) { log(LogLevel::Warning, message); }
void Logger::error(const QString &message) { log(LogLevel::Error, message); }
void Logger::fatal(const QString &message) { log(LogLevel::Fatal, message); }

void Logger::log(LogLevel level, const QString &message)
{
    // Multiple Qt threads could log at the same time. The mutex prevents
    // interleaved writes to app.log.
    QMutexLocker locker(&mutex());
    const QString path = logPath();
    rotateIfNeeded(path);

    QFile file(path);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Append | QIODevice::Text)) {
        return;
    }

    const QString line = QString("[%1] [%2] %3")
            .arg(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss"))
            .arg(levelName(level), message);

    QTextStream out(&file);
    out << line << '\n';

#ifdef QT_DEBUG
    QTextStream console(stdout);
    console << line << Qt::endl;
#endif
}

void Logger::messageHandler(QtMsgType type, const QMessageLogContext &, const QString &message)
{
    // Qt calls this function for qDebug(), qInfo(), qWarning(), qCritical()
    // and qFatal() after qInstallMessageHandler() is installed in main().
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
        abort();
    }
}

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
    return "INFO";
}

void Logger::rotateIfNeeded(const QString &path)
{
    // Simple size-based rotation keeps the demo log from growing forever.
    QFileInfo info(path);
    if (!info.exists() || info.size() < MaxLogSize) {
        return;
    }

    const QString backupPath = path + "." + QDateTime::currentDateTime().toString("yyyyMMddhhmmss");
    QFile::rename(path, backupPath);
}

QMutex &Logger::mutex()
{
    static QMutex value;
    return value;
}
