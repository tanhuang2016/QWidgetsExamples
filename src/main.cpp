#include "MainWindow.h"
#include "core/AppContext.h"
#include "core/ExceptionHandler.h"
#include "core/Logger.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    QApplication::setApplicationName("Qt Widget Gallery");
    QApplication::setOrganizationName("qt_codex1");

    qInstallMessageHandler(Logger::messageHandler);

    try {
        AppContext::instance().initialize(&app);
        Logger::info("Application started");

        MainWindow window;
        window.resize(1100, 720);
        window.show();

        return QApplication::exec();
    } catch (const std::exception &e) {
        ExceptionHandler::handle(e);
    } catch (...) {
        ExceptionHandler::show(QObject::tr("Unexpected startup error."));
        Logger::error("Unknown startup exception");
    }

    return 1;
}
