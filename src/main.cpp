#include "MainWindow.h"
#include "core/AppContext.h"
#include "core/ExceptionHandler.h"
#include "core/Logger.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    // QApplication owns the Qt event loop and application-wide resources.
    // Every Qt Widgets program needs exactly one QApplication object.
    QApplication app(argc, argv);
    QApplication::setApplicationName("Qt Widget Gallery");
    QApplication::setOrganizationName("qt_codex1");

    // Route qDebug/qWarning/qCritical output into our Logger.
    qInstallMessageHandler(Logger::messageHandler);

    try {
        // Initialize services after QApplication exists because QTranslator
        // must be installed on the application object.
        AppContext::instance().initialize(&app);
        Logger::info("Application started");

        MainWindow window;
        window.resize(1100, 720);
        window.show();

        // Enters Qt's event loop. Control returns when the last main window is
        // closed or QApplication::quit() is called.
        return QApplication::exec();
    } catch (const std::exception &e) {
        ExceptionHandler::handle(e);
    } catch (...) {
        ExceptionHandler::show(QObject::tr("Unexpected startup error."));
        Logger::error("Unknown startup exception");
    }

    return 1;
}
