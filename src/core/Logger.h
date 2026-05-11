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

class Logger {
public:
    static void debug(const QString &message);
    static void info(const QString &message);
    static void warning(const QString &message);
    static void error(const QString &message);
    static void fatal(const QString &message);
    static void log(LogLevel level, const QString &message);
    static void messageHandler(QtMsgType type, const QMessageLogContext &context, const QString &message);

private:
    static QString levelName(LogLevel level);
    static void rotateIfNeeded(const QString &path);
    static QMutex &mutex();
};
