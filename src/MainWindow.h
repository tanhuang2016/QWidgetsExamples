#pragma once

#include <QMainWindow>

class QAction;
class ContentStack;
class NavigationPanel;

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);

protected:
    void changeEvent(QEvent *event) override;

private slots:
    void switchToEnglish();
    void switchToChinese();
    void retranslateUi();

private:
    void buildUi();
    void buildMenus();
    void applyStyle();

    NavigationPanel *m_navigation;
    ContentStack *m_content;
    QMenu *m_languageMenu;
    QAction *m_englishAction;
    QAction *m_chineseAction;
};
