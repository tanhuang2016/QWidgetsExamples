#include "MainWindow.h"
#include "core/AppContext.h"
#include "core/ExceptionHandler.h"
#include "core/Logger.h"

#include <QApplication>

/**
 * @brief Qt应用程序的主入口函数
 * 
 * Java对比：类似于Java的 public static void main(String[] args)
 * 
 * 参数说明：
 * - argc: 命令行参数数量（类似Java args.length）
 * - argv: 命令行参数数组（类似Java args[]）
 * 
 * 返回值：
 * - 0: 正常退出
 * - 非0: 异常退出
 */
int main(int argc, char *argv[])
{
    // 【重要】创建QApplication对象
    // Java对比：这相当于Spring Boot应用的 SpringApplication.run()
    // QApplication管理整个应用的生命周期和事件循环
    // 每个Qt Widgets程序必须有且仅有一个QApplication实例
    QApplication app(argc, argv);
    
    // 设置应用程序元信息（在窗口标题、关于对话框中显示）
    QApplication::setApplicationName("Qt Widget Gallery");
    QApplication::setOrganizationName("qt_codex1");

    // 安装自定义消息处理器
    // Java对比：类似于配置Log4j/SLF4J的全局日志拦截器
    // 所有qDebug()、qWarning()等输出都会被重定向到Logger::messageHandler
    qInstallMessageHandler(Logger::messageHandler);

    try {
        // 【关键】初始化应用上下文
        // 必须在QApplication创建之后调用，因为TranslationManager需要访问app对象
        // Java对比：类似于Spring容器的初始化 ApplicationContext.refresh()
        AppContext::instance().initialize(&app);
        
        // 记录应用启动日志
        Logger::info("Application started");

        // 创建并显示主窗口
        MainWindow window;
        window.resize(1100, 720);  // 设置初始窗口大小：宽1100px，高720px
        window.show();              // 显示窗口（异步操作，不会阻塞）

        // 【核心】进入Qt事件循环
        // Java对比：类似于JavaFX的 Platform.runLater() + 事件泵
        // 程序会在这里阻塞，直到最后一个窗口关闭或调用QApplication::quit()
        // 所有UI交互（点击、键盘、绘图）都通过这个事件循环处理
        return QApplication::exec();
        
    } catch (const std::exception &e) {
        // 捕获标准C++异常（类似Java的 Exception）
        ExceptionHandler::handle(e);
    } catch (...) {
        // 捕获所有未知异常（Java中没有这种语法，这是C++特有）
        ExceptionHandler::show(QObject::tr("Unexpected startup error."));
        Logger::error("Unknown startup exception");
    }

    // 如果发生异常，返回错误码1
    return 1;
}
