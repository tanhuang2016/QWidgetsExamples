#include "ContainerPage.h"
#include "core/Logger.h"

#include <QDockWidget>
#include <QGroupBox>
#include <QLabel>
#include <QListWidget>
#include <QMdiArea>
#include <QMdiSubWindow>
#include <QPushButton>
#include <QSplitter>
#include <QTabWidget>
#include <QTextEdit>
#include <QTime>
#include <QToolBox>
#include <QVBoxLayout>

ContainerPage::ContainerPage(QWidget *parent)
    : ControlPageBase(parent),
      m_tabs(new QTabWidget(this)),
      m_mdi(new QMdiArea(this)),
      m_log(new QTextEdit(this))
{
    QVBoxLayout *root = new QVBoxLayout(this);
    root->setContentsMargins(24, 24, 24, 24);
    QSplitter *splitter = new QSplitter(Qt::Horizontal, this);

    QToolBox *toolBox = new QToolBox(splitter);
    QListWidget *list = new QListWidget(toolBox);
    list->addItems(QStringList() << tr("Dock") << tr("MDI") << tr("Tabs") << tr("Splitter"));
    QLabel *help = new QLabel(tr("ToolBox page with arbitrary widgets."), toolBox);
    help->setAlignment(Qt::AlignCenter);
    toolBox->addItem(list, style()->standardIcon(QStyle::SP_DirIcon), tr("Navigation"));
    toolBox->addItem(help, style()->standardIcon(QStyle::SP_MessageBoxInformation), tr("Help"));

    QWidget *right = new QWidget(splitter);
    QVBoxLayout *rightLayout = new QVBoxLayout(right);
    m_tabs->setTabsClosable(true);
    m_tabs->setMovable(true);
    m_tabs->setTabPosition(QTabWidget::North);
    m_tabs->addTab(new QLabel(tr("Closable and movable tab"), m_tabs), style()->standardIcon(QStyle::SP_FileIcon), tr("Tab 1"));
    m_tabs->addTab(new QLabel(tr("Second tab"), m_tabs), style()->standardIcon(QStyle::SP_FileDialogDetailedView), tr("Tab 2"));

    QGroupBox *buttons = new QGroupBox(tr("Control Data"), right);
    QHBoxLayout *buttonLayout = new QHBoxLayout(buttons);
    QPushButton *addTabButton = new QPushButton(tr("Add Tab"), buttons);
    QPushButton *addMdiButton = new QPushButton(tr("Add MDI"), buttons);
    QPushButton *tileButton = new QPushButton(tr("Tile"), buttons);
    QPushButton *cascadeButton = new QPushButton(tr("Cascade"), buttons);
    buttonLayout->addWidget(addTabButton);
    buttonLayout->addWidget(addMdiButton);
    buttonLayout->addWidget(tileButton);
    buttonLayout->addWidget(cascadeButton);
    buttonLayout->addStretch();

    m_log->setReadOnly(true);
    m_log->setMaximumHeight(80);
    rightLayout->addWidget(m_tabs);
    rightLayout->addWidget(m_mdi, 1);
    rightLayout->addWidget(buttons);
    rightLayout->addWidget(m_log);

    splitter->addWidget(toolBox);
    splitter->addWidget(right);
    splitter->setStretchFactor(1, 1);
    root->addWidget(splitter);

    connect(m_tabs, &QTabWidget::tabCloseRequested, this, [this](int index) {
        QWidget *widget = m_tabs->widget(index);
        m_tabs->removeTab(index);
        widget->deleteLater();
        appendLog(tr("Tab closed"));
    });
    connect(addTabButton, &QPushButton::clicked, this, &ContainerPage::addTab);
    connect(addMdiButton, &QPushButton::clicked, this, &ContainerPage::addMdiWindow);
    connect(tileButton, &QPushButton::clicked, this, &ContainerPage::tileMdi);
    connect(cascadeButton, &QPushButton::clicked, this, &ContainerPage::cascadeMdi);
    addMdiWindow();
}

QString ContainerPage::pageName() const { return tr("Containers"); }

void ContainerPage::retranslateUi()
{
}

void ContainerPage::addTab()
{
    const int number = m_tabs->count() + 1;
    m_tabs->addTab(new QLabel(tr("Dynamic tab %1").arg(number), m_tabs), style()->standardIcon(QStyle::SP_FileIcon), tr("Tab %1").arg(number));
    appendLog(tr("Tab added"));
}

void ContainerPage::addMdiWindow()
{
    QTextEdit *editor = new QTextEdit;
    editor->setPlainText(tr("MDI sub window content"));
    QMdiSubWindow *sub = m_mdi->addSubWindow(editor);
    sub->setWindowTitle(tr("Document %1").arg(m_mdi->subWindowList().size()));
    sub->resize(240, 140);
    sub->show();
    appendLog(tr("MDI window added"));
}

void ContainerPage::tileMdi()
{
    m_mdi->tileSubWindows();
    appendLog(tr("MDI tiled"));
}

void ContainerPage::cascadeMdi()
{
    m_mdi->cascadeSubWindows();
    appendLog(tr("MDI cascaded"));
}

void ContainerPage::appendLog(const QString &message)
{
    m_log->append(QTime::currentTime().toString("hh:mm:ss") + "  " + message);
    Logger::info(QString("ContainerPage: %1").arg(message));
}
