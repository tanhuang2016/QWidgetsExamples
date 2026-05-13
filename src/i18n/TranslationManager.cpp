#include "TranslationManager.h"
#include "core/Logger.h"

#include <QApplication>
#include <QCoreApplication>
#include <QDir>
#include <QStringList>

/**
 * @brief 匿名命名空间:翻译文件搜索路径工具函数
 * 
 * 【问题背景】
 * Qt应用的资源文件在不同平台、不同打包方式下位置不同:
 * - 开发环境(Debug): exe所在目录/resources/translations
 * - Windows安装包: bin/../resources/translations
 * - Linux安装: /usr/bin/../share/translations
 * - macOS Bundle: Contents/MacOS/../Resources/translations
 * 
 * 【解决方案】
 * 尝试多个可能的路径,找到第一个存在的路径
 * 类似于Java的 classpath 搜索机制
 */
namespace {
QStringList translationSearchPaths()
{
    // 获取应用程序可执行文件所在目录
    // 例如: "E:/code/cpp/qt_codex1/build" 或 "C:/Program Files/MyApp/bin"
    const QDir appDir(QCoreApplication::applicationDirPath());
    
    QStringList paths;
    
    // 【路径策略】从近到远尝试多个相对路径
    // 覆盖不同的目录结构可能性
    
    // ① 当前目录下: build/resources/translations (开发环境)
    paths << appDir.filePath("resources/translations");
    
    // ② 上级目录: build/../resources/translations = resources/translations
    paths << appDir.filePath("../resources/translations");
    
    // ③ 上两级目录: build/../../resources/translations
    paths << appDir.filePath("../../resources/translations");
    
    // ④ 上三级目录(某些深层嵌套结构)
    paths << appDir.filePath("../../../resources/translations");
    
    // ⑤ macOS Bundle特殊路径: Contents/MacOS/../Resources/translations
    // macOS应用结构:
    // MyApp.app/
    //   └─ Contents/
    //       ├─ MacOS/myapp (可执行文件)
    //       └─ Resources/translations/zh_CN.qm
    paths << QDir(QCoreApplication::applicationDirPath() + "/../Resources").filePath("translations");
    
    // 【去重】移除重复路径(如果多个相对路径指向同一位置)
    paths.removeDuplicates();
    
    return paths;
}
}

/**
 * @brief 构造函数:初始化翻译管理器
 * 
 * 【参数说明】
 * @param app QApplication指针 - 必须传入,因为翻译器需要安装到QApplication上
 * @param parent QObject父对象 - Qt父子内存管理机制
 * 
 * 【初始化状态】
 * - m_app: 保存QApplication引用,后续用于 installTranslator/removeTranslator
 * - m_locale: 默认语言为 "en_US" (英语)
 * - m_translator: QTranslator对象,初始为空(未加载任何翻译文件)
 * 
 * Java对比:
 * 类似于 Spring 的:
 * @Autowired
 * private ApplicationContext context;  // 需要全局上下文
 */
TranslationManager::TranslationManager(QApplication *app, QObject *parent)
    : QObject(parent),  // 调用父类构造函数,建立Qt父子关系
      m_app(app),        // 保存QApplication指针
      m_locale("en_US")  // 默认使用英语
{
    // 注意:构造时不加载任何翻译文件
    // 翻译文件在首次调用 switchLanguage() 时才加载
}

/**
 * @brief 获取当前语言区域标识
 * 
 * 【返回值示例】
 * - "en_US": 美式英语
 * - "zh_CN": 简体中文
 * - "ja_JP": 日语
 * - "de_DE": 德语
 * 
 * 【用途】
 * UI上显示当前语言,或用于条件判断:
 * if (translationManager.currentLocale() == "zh_CN") {
 *     // 中文特定逻辑
 * }
 * 
 * @return QString 当前语言区域ID
 */
QString TranslationManager::currentLocale() const
{
    return m_locale;
}

/**
 * @brief 切换应用程序语言(核心方法)
 * 
 * 【工作流程】
 * 1. 检查是否与当前语言相同 → 相同则直接发出信号(刷新UI)
 * 2. 卸载旧的翻译器(如果有)
 * 3. 加载新的翻译文件(.qm)
 * 4. 安装新的翻译器到QApplication
 * 5. 发出 languageChanged 信号 → 所有监听的页面重新翻译UI
 * 
 * 【Qt翻译机制原理】
 * - 代码中使用 tr("Hello") 包裹需要翻译的文本
 * - Qt Linguist工具从代码提取字符串到 .ts 文件(XML格式)
 * - 翻译完成后编译成 .qm 文件(二进制格式,加载更快)
 * - QTranslator加载 .qm 文件,拦截所有 tr() 调用并返回翻译后的文本
 * 
 * Java对比:
 * 类似于 ResourceBundle + Locale 的动态切换:
 * ResourceBundle bundle = ResourceBundle.getBundle("messages", new Locale("zh", "CN"));
 * String text = bundle.getString("hello");  // 类似 tr("Hello")
 * 
 * @param locale 目标语言区域ID,如 "zh_CN", "en_US"
 * @return bool 是否成功加载翻译文件
 *         - true: 成功加载(或切换到英语,不需要文件)
 *         - false: 找不到翻译文件
 * 
 * 【调用示例】
 * // 用户点击菜单: Language → 中文
 * bool success = translationManager.switchLanguage("zh_CN");
 * if (success) {
 *     qDebug() << "语言切换成功";
 * } else {
 *     qDebug() << "翻译文件不存在,回退到英语";
 * }
 */
bool TranslationManager::switchLanguage(const QString &locale)
{
    // 【优化1】如果目标语言与当前语言相同,无需重新加载
    // 但仍然发出信号,让UI有机会刷新(可能外部修改了翻译文件)
    if (locale == m_locale) {
        emit languageChanged();  // 通知所有监听者:语言"改变"了(实际没变)
        return true;
    }

    // 【步骤1】卸载旧的翻译器
    // 这会移除之前加载的所有翻译规则
    // 之后 tr() 调用将返回原始字符串(英语)
    m_app->removeTranslator(&m_translator);
    
    // 【步骤2】更新当前语言标识
    m_locale = locale;

    // 【步骤3】加载新的翻译文件
    bool loaded = true;  // 默认为true,因为英语是默认语言,不需要文件
    
    // 特殊情况:英语(en_US)是默认语言
    // 代码中的 tr("Hello") 默认就是英语,所以不需要加载翻译文件
    if (locale != "en_US") {
        loaded = false;  // 非英语需要加载文件
        
        // 【遍历搜索路径】尝试从多个位置加载翻译文件
        // QTranslator::load("zh_CN", path) 会查找 path/zh_CN.qm
        for (const QString &basePath : translationSearchPaths()) {
            // 尝试加载: basePath/zh_CN.qm
            if (m_translator.load(locale, basePath)) {
                loaded = true;
                Logger::info(QString("Loaded translation %1 from %2").arg(locale, basePath));
                break;  // 找到第一个可用的就停止
            }
        }
        
        // 【错误处理】如果所有路径都找不到翻译文件
        if (!loaded) {
            Logger::warning(QString("Translation file not found for %1. Search paths: %2")
                            .arg(locale, translationSearchPaths().join("; ")));
            // 注意:即使加载失败,m_locale已经更新
            // 这样UI会显示原始字符串(通常是英语),而不是崩溃
        } else {
            // 【步骤4】安装翻译器到QApplication
            // 这会让所有后续的 tr() 调用都使用新的翻译规则
            // 必须在发出信号之前安装,否则UI刷新时会用错翻译
            m_app->installTranslator(&m_translator);
        }
    }

    // 【步骤5】发出语言改变信号
    // 所有连接到这个信号的槽函数都会被调用
    // 典型用法: 页面重新调用 retranslateUi() 刷新文本
    // 
    // 信号接收者示例:
    // connect(translationManager, &TranslationManager::languageChanged,
    //         this, &MyPage::retranslateUi);
    emit languageChanged();
    
    return loaded;
}
