#include "MainWindow.h"
#include "core/AppContext.h"
#include "core/ExceptionHandler.h"
#include "i18n/TranslationManager.h"
#include "ui/ContentStack.h"
#include "ui/NavigationPanel.h"

#include <QAction>
#include <QApplication>
#include <QEvent>
#include <QHBoxLayout>
#include <QMenu>
#include <QMenuBar>
#include <QSplitter>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
      m_navigation(nullptr),
      m_content(nullptr),
      m_languageMenu(nullptr),
      m_englishAction(nullptr),
      m_chineseAction(nullptr)
{
    buildUi();
    buildMenus();
    applyStyle();
    retranslateUi();

    connect(&AppContext::instance().translations(), &TranslationManager::languageChanged,
            this, &MainWindow::retranslateUi);
}

void MainWindow::changeEvent(QEvent *event)
{
    if (event->type() == QEvent::LanguageChange) {
        retranslateUi();
    }
    QMainWindow::changeEvent(event);
}

void MainWindow::switchToEnglish()
{
    try {
        AppContext::instance().translations().switchLanguage("en_US");
    } catch (const std::exception &e) {
        ExceptionHandler::handle(e);
    }
}

void MainWindow::switchToChinese()
{
    try {
        AppContext::instance().translations().switchLanguage("zh_CN");
    } catch (const std::exception &e) {
        ExceptionHandler::handle(e);
    }
}

void MainWindow::retranslateUi()
{
    setWindowTitle(tr("Qt Widget Gallery"));
    if (m_languageMenu) {
        m_languageMenu->setTitle(tr("Language"));
        m_englishAction->setText(tr("English"));
        m_chineseAction->setText(tr("Chinese"));
    }
    if (m_navigation) {
        m_navigation->retranslateUi();
    }
    if (m_content) {
        m_content->retranslatePages();
    }
}

void MainWindow::buildUi()
{
    QWidget *central = new QWidget(this);
    QHBoxLayout *layout = new QHBoxLayout(central);
    layout->setContentsMargins(0, 0, 0, 0);

    QSplitter *splitter = new QSplitter(Qt::Horizontal, central);
    m_navigation = new NavigationPanel(splitter);
    m_content = new ContentStack(splitter);

    splitter->addWidget(m_navigation);
    splitter->addWidget(m_content);
    splitter->setStretchFactor(0, 0);
    splitter->setStretchFactor(1, 1);
    layout->addWidget(splitter);
    setCentralWidget(central);

    connect(m_navigation, &NavigationPanel::pageSelected,
            m_content, &ContentStack::showPage);
    m_navigation->setPages(AppContext::instance().registry().keys());
}

void MainWindow::buildMenus()
{
    m_languageMenu = menuBar()->addMenu(QString());
    m_englishAction = m_languageMenu->addAction(QString(), this, &MainWindow::switchToEnglish);
    m_chineseAction = m_languageMenu->addAction(QString(), this, &MainWindow::switchToChinese);
}

void MainWindow::applyStyle()
{
    qApp->setStyleSheet(
        "QMainWindow { background: #f5f6f8; }"
        "QTreeWidget { background: #ffffff; border: 0; border-right: 1px solid #d6d9df; font-size: 14px; }"
        "QTreeWidget::item { height: 36px; padding-left: 12px; }"
        "QTreeWidget::item:selected { background: #2f6fed; color: white; }"
        "QGroupBox { font-weight: 600; border: 1px solid #d6d9df; border-radius: 4px; margin-top: 12px; padding: 10px; }"
        "QGroupBox::title { subcontrol-origin: margin; left: 8px; padding: 0 4px; }"
        "QPushButton { min-height: 28px; padding: 4px 12px; }"
        "QLineEdit, QComboBox, QSpinBox, QDoubleSpinBox, QTextEdit, QTableWidget, QListWidget { border: 1px solid #c9ced6; border-radius: 3px; padding: 4px; background: white; }"
    );
}
