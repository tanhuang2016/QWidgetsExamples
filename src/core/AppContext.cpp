#include "AppContext.h"
#include "i18n/TranslationManager.h"
#include "pages/ButtonPage.h"
#include "pages/AdvancedPage.h"
#include "pages/ChartPage.h"
#include "pages/ContainerPage.h"
#include "pages/DialogPage.h"
#include "pages/FeedbackPage.h"
#include "pages/GraphicsPage.h"
#include "pages/InputPage.h"
#include "pages/LayoutPage.h"
#include "pages/MenuToolbarPage.h"
#include "pages/PaginationPage.h"
#include "pages/StylePage.h"
#include "pages/TablePage.h"
#include "pages/ViewPage.h"

#include <QApplication>

AppContext &AppContext::instance()
{
    static AppContext context;
    return context;
}

AppContext::AppContext()
    : m_translations(nullptr),
      m_initialized(false)
{
}

void AppContext::initialize(QApplication *app)
{
    if (m_initialized) {
        return;
    }

    // TranslationManager is parented to QApplication, so Qt will delete it
    // automatically when the application shuts down.
    m_translations = new TranslationManager(app);

    // Register factories only once. Actual pages are created later by
    // ContentStack when the user selects them.
    registerPages();
    m_initialized = true;
}

ControlRegistry &AppContext::registry()
{
    return m_registry;
}

TranslationManager &AppContext::translations()
{
    return *m_translations;
}

void AppContext::registerPages()
{
    // Each registration is: group key, page key, display title, factory lambda.
    // The factory returns a heap-allocated QWidget; once inserted into
    // QStackedWidget, Qt parent ownership manages its lifetime.
    m_registry.registerPage("basic", "buttons", QObject::tr("Buttons"), []() { return new ButtonPage(); });
    m_registry.registerPage("basic", "inputs", QObject::tr("Inputs"), []() { return new InputPage(); });
    m_registry.registerPage("data", "views", QObject::tr("Views"), []() { return new ViewPage(); });
    m_registry.registerPage("data", "table", QObject::tr("Tables"), []() { return new TablePage(); });
    m_registry.registerPage("data", "pagination", QObject::tr("Pagination"), []() { return new PaginationPage(); });
    m_registry.registerPage("actions", "menuToolbar", QObject::tr("Menu && Toolbar"), []() { return new MenuToolbarPage(); });
    m_registry.registerPage("feedback", "dialogs", QObject::tr("Dialogs"), []() { return new DialogPage(); });
    m_registry.registerPage("feedback", "feedback", QObject::tr("Dialogs && Tips"), []() { return new FeedbackPage(); });
    m_registry.registerPage("graphics", "graphics", QObject::tr("Graphics View"), []() { return new GraphicsPage(); });
    m_registry.registerPage("graphics", "charts", QObject::tr("Charts"), []() { return new ChartPage(); });
    m_registry.registerPage("style", "layouts", QObject::tr("Layouts"), []() { return new LayoutPage(); });
    m_registry.registerPage("style", "style", QObject::tr("Style Controls"), []() { return new StylePage(); });
    m_registry.registerPage("advanced", "containers", QObject::tr("Containers"), []() { return new ContainerPage(); });
    m_registry.registerPage("advanced", "advanced", QObject::tr("Advanced"), []() { return new AdvancedPage(); });
}
