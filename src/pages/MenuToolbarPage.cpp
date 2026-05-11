#include "MenuToolbarPage.h"
#include "core/Logger.h"

#include <QAction>
#include <QActionGroup>
#include <QCheckBox>
#include <QComboBox>
#include <QGroupBox>
#include <QHBoxLayout>
#include <QLineEdit>
#include <QMainWindow>
#include <QMenu>
#include <QMenuBar>
#include <QPushButton>
#include <QSpinBox>
#include <QStatusBar>
#include <QTextEdit>
#include <QTime>
#include <QToolBar>
#include <QVBoxLayout>

MenuToolbarPage::MenuToolbarPage(QWidget *parent)
    : ControlPageBase(parent),
      m_demoWindow(new QMainWindow(this)),
      m_log(new QTextEdit(this)),
      m_fileMenu(nullptr),
      m_recentMenu(nullptr),
      m_viewMenu(nullptr),
      m_toolbar(nullptr),
      m_newAction(nullptr),
      m_openAction(nullptr),
      m_saveAction(nullptr),
      m_boldAction(nullptr),
      m_lightAction(nullptr),
      m_darkAction(nullptr)
{
    QVBoxLayout *root = new QVBoxLayout(this);
    root->setContentsMargins(24, 24, 24, 24);

    m_demoWindow->setMinimumHeight(260);
    m_demoWindow->setContextMenuPolicy(Qt::CustomContextMenu);
    QWidget *placeholder = new QWidget(m_demoWindow);
    QVBoxLayout *placeholderLayout = new QVBoxLayout(placeholder);
    QTextEdit *editor = new QTextEdit(placeholder);
    editor->setPlaceholderText(tr("Toolbar embeds search, combo and spin box controls."));
    placeholderLayout->addWidget(editor);
    m_demoWindow->setCentralWidget(placeholder);

    m_fileMenu = m_demoWindow->menuBar()->addMenu(QString());
    m_recentMenu = new QMenu(m_demoWindow);
    m_viewMenu = m_demoWindow->menuBar()->addMenu(QString());
    m_toolbar = m_demoWindow->addToolBar(QString());
    m_toolbar->setMovable(true);

    m_newAction = createAction(style()->standardIcon(QStyle::SP_FileIcon), tr("New"), QKeySequence::New);
    m_openAction = createAction(style()->standardIcon(QStyle::SP_DialogOpenButton), tr("Open"), QKeySequence::Open);
    m_saveAction = createAction(style()->standardIcon(QStyle::SP_DialogSaveButton), tr("Save"), QKeySequence::Save);
    m_boldAction = createAction(style()->standardIcon(QStyle::SP_DialogApplyButton), tr("Bold"), QKeySequence(Qt::CTRL | Qt::Key_B));
    m_boldAction->setCheckable(true);

    QActionGroup *themeGroup = new QActionGroup(this);
    m_lightAction = createAction(QIcon(), tr("Light"), QKeySequence());
    m_darkAction = createAction(QIcon(), tr("Dark"), QKeySequence());
    m_lightAction->setCheckable(true);
    m_darkAction->setCheckable(true);
    m_lightAction->setChecked(true);
    themeGroup->addAction(m_lightAction);
    themeGroup->addAction(m_darkAction);

    m_fileMenu->addAction(m_newAction);
    m_fileMenu->addAction(m_openAction);
    m_fileMenu->addAction(m_saveAction);
    m_fileMenu->addSeparator();
    m_fileMenu->addMenu(m_recentMenu);
    m_viewMenu->addAction(m_boldAction);
    m_viewMenu->addSeparator();
    m_viewMenu->addAction(m_lightAction);
    m_viewMenu->addAction(m_darkAction);

    m_toolbar->addAction(m_newAction);
    m_toolbar->addAction(m_openAction);
    m_toolbar->addAction(m_saveAction);
    m_toolbar->addSeparator();
    QLineEdit *search = new QLineEdit(m_toolbar);
    search->setPlaceholderText(tr("Search"));
    search->setMaximumWidth(160);
    QComboBox *mode = new QComboBox(m_toolbar);
    mode->addItems(QStringList() << tr("Icon only") << tr("Text beside icon") << tr("Text under icon"));
    QSpinBox *zoom = new QSpinBox(m_toolbar);
    zoom->setRange(50, 200);
    zoom->setValue(100);
    zoom->setSuffix("%");
    m_toolbar->addWidget(search);
    m_toolbar->addWidget(mode);
    m_toolbar->addWidget(zoom);

    QGroupBox *controls = new QGroupBox(tr("Control Data"), this);
    QHBoxLayout *controlLayout = new QHBoxLayout(controls);
    QPushButton *recentButton = new QPushButton(tr("Add recent file"), controls);
    QCheckBox *saveEnabled = new QCheckBox(tr("Save enabled"), controls);
    saveEnabled->setChecked(true);
    controlLayout->addWidget(recentButton);
    controlLayout->addWidget(saveEnabled);
    controlLayout->addStretch();

    m_log->setReadOnly(true);
    m_log->setMaximumHeight(100);
    root->addWidget(m_demoWindow);
    root->addWidget(m_log);
    root->addWidget(controls);

    connect(recentButton, &QPushButton::clicked, this, &MenuToolbarPage::addRecentFile);
    connect(saveEnabled, &QCheckBox::toggled, this, &MenuToolbarPage::toggleSaveEnabled);
    connect(m_demoWindow, &QMainWindow::customContextMenuRequested, this, &MenuToolbarPage::showContextMenu);
    retranslateUi();
    appendLog(tr("Menu and toolbar demo ready"));
}

QString MenuToolbarPage::pageName() const { return tr("Menu && Toolbar"); }

void MenuToolbarPage::retranslateUi()
{
    m_fileMenu->setTitle(tr("File"));
    m_recentMenu->setTitle(tr("Recent Files"));
    m_viewMenu->setTitle(tr("View"));
    m_toolbar->setWindowTitle(tr("Main Toolbar"));
    m_newAction->setText(tr("New"));
    m_openAction->setText(tr("Open"));
    m_saveAction->setText(tr("Save"));
    m_boldAction->setText(tr("Bold"));
    m_lightAction->setText(tr("Light"));
    m_darkAction->setText(tr("Dark"));
    rebuildRecentMenu();
}

void MenuToolbarPage::addRecentFile()
{
    m_recentFiles.prepend(tr("document_%1.txt").arg(m_recentFiles.size() + 1));
    rebuildRecentMenu();
    appendLog(tr("Recent file added"));
}

void MenuToolbarPage::toggleSaveEnabled(bool enabled)
{
    m_saveAction->setEnabled(enabled);
    appendLog(tr("Save action enabled: %1").arg(enabled ? tr("yes") : tr("no")));
}

void MenuToolbarPage::showContextMenu(const QPoint &pos)
{
    QMenu menu(this);
    menu.addAction(m_newAction);
    menu.addAction(m_openAction);
    menu.addAction(m_saveAction);
    menu.exec(m_demoWindow->mapToGlobal(pos));
}

void MenuToolbarPage::logAction()
{
    QAction *action = qobject_cast<QAction *>(sender());
    if (action) {
        appendLog(tr("Action triggered: %1, shortcut=%2").arg(action->text(), action->shortcut().toString()));
        m_demoWindow->statusBar()->showMessage(tr("Triggered %1").arg(action->text()), 3000);
    }
}

QAction *MenuToolbarPage::createAction(const QIcon &icon, const QString &text, const QKeySequence &shortcut)
{
    QAction *action = new QAction(icon, text, this);
    action->setShortcut(shortcut);
    action->setToolTip(text);
    action->setStatusTip(tr("Trigger %1").arg(text));
    action->setWhatsThis(tr("This action demonstrates icon, shortcut, tooltip and status tip."));
    connect(action, &QAction::triggered, this, &MenuToolbarPage::logAction);
    return action;
}

void MenuToolbarPage::rebuildRecentMenu()
{
    m_recentMenu->clear();
    if (m_recentFiles.isEmpty()) {
        QAction *empty = m_recentMenu->addAction(tr("No recent files"));
        empty->setEnabled(false);
        return;
    }
    for (const QString &file : m_recentFiles) {
        QAction *action = m_recentMenu->addAction(style()->standardIcon(QStyle::SP_FileIcon), file);
        connect(action, &QAction::triggered, this, [this, file]() { appendLog(tr("Recent file opened: %1").arg(file)); });
    }
}

void MenuToolbarPage::appendLog(const QString &message)
{
    m_log->append(QTime::currentTime().toString("hh:mm:ss") + "  " + message);
    Logger::info(QString("MenuToolbarPage: %1").arg(message));
}
