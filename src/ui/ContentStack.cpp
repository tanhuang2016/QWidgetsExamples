#include "ContentStack.h"
#include "core/AppContext.h"
#include "core/ExceptionHandler.h"
#include "core/Logger.h"
#include "ui/ControlPageBase.h"

#include <QLabel>
#include <QVBoxLayout>
#include <stdexcept>

ContentStack::ContentStack(QWidget *parent)
    : QStackedWidget(parent)
{
}

void ContentStack::showPage(const QString &key)
{
    try {
        // loadPage() performs lazy creation and returns a cached QWidget.
        ControlPageBase *page = loadPage(key);
        setCurrentWidget(page);
    } catch (const std::exception &e) {
        Logger::error(QString("Failed to show page %1: %2").arg(key, e.what()));
        QWidget *errorPage = createErrorPage(tr("Failed to load this page."));
        addWidget(errorPage);
        setCurrentWidget(errorPage);
        ExceptionHandler::handle(e);
    }
}

void ContentStack::retranslatePages()
{
    for (ControlPageBase *page : m_pages) {
        page->retranslateUi();
    }
}

ControlPageBase *ContentStack::loadPage(const QString &key)
{
    if (m_pages.contains(key)) {
        return m_pages.value(key);
    }

    // First visit: ask the registry to instantiate the concrete page class.
    ControlPageBase *page = AppContext::instance().registry().create(key);
    if (!page) {
        throw std::runtime_error("Page factory returned null.");
    }

    m_pages.insert(key, page);
    addWidget(page);
    Logger::info(QString("Lazy loaded page: %1").arg(key));
    return page;
}

QWidget *ContentStack::createErrorPage(const QString &message)
{
    QWidget *page = new QWidget(this);
    QVBoxLayout *layout = new QVBoxLayout(page);
    QLabel *label = new QLabel(message, page);
    label->setAlignment(Qt::AlignCenter);
    layout->addWidget(label);
    return page;
}
