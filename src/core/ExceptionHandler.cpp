#include "ExceptionHandler.h"
#include "Logger.h"

#include <QMessageBox>
#include <QObject>

void ExceptionHandler::handle(const std::exception &e)
{
    Logger::error(QString("Exception: %1").arg(e.what()));
    show(QString::fromLocal8Bit(e.what()));
}

void ExceptionHandler::show(const QString &message)
{
    QMessageBox::warning(nullptr, QObject::tr("Application Error"), message);
}
