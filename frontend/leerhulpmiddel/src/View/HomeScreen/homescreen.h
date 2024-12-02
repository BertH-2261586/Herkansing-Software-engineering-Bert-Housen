#ifndef HOMESCREEN_H
#define HOMESCREEN_H

#include <QGridLayout>
#include <QHBoxLayout>

class LeerhulpmiddelMainWindow;

#include "qwidget.h"
#include "questionsetbrowser.h"
#include "../../Controller/questionmanagercontroller.h"

class HomeScreen: public QWidget
{
public:
    HomeScreen(QuestionManagerController* questionManagerController, LeerhulpmiddelMainWindow* parent);

private:
    QWidget* GenerateTopButtonBar();

    QuestionsetBrowser* m_vragensetBrowser;
    QWidget* m_rightSideScreen;
    QHBoxLayout* m_container;

    LeerhulpmiddelMainWindow* m_mainWindow;

public slots:
    void DisplayWidget(QWidget* displayWidget);
};

#endif // HOMESCREEN_H
