#ifndef HOMESCREEN_H
#define HOMESCREEN_H

#include <QGridLayout>
#include <QHBoxLayout>

#include "../../Controller/LoginController.h"
#include "../../model/manager/NetworkManager.h"

class LeerhulpmiddelMainWindow;

#include "qwidget.h"
#include "questionsetbrowser.h"
#include "../../Controller/questionmanagercontroller.h"

class HomeScreen: public QWidget
{
public:
    HomeScreen(QuestionManagerController* questionManagerController, LeerhulpmiddelMainWindow* parent);
    ~HomeScreen();

private:
    QWidget* GenerateTopButtonBar();

    QuestionsetBrowser* m_vragensetBrowser;
    QWidget* m_rightSideScreen;
    QHBoxLayout* m_container;

    LeerhulpmiddelMainWindow* m_mainWindow;

    NetworkManager* m_networkManager;
    LoginController* m_loginController;
public slots:
    void DisplayWidget(QWidget* displayWidget);
};

#endif // HOMESCREEN_H
