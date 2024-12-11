#ifndef HOMESCREEN_H
#define HOMESCREEN_H

#include <QGridLayout>
#include <QHBoxLayout>
#include <QWidget>

class LeerhulpmiddelMainWindow;

#include "questionsetbrowser.h"
#include "../../Controller/questionmanagercontroller.h"
#include "inboxView.h"

class HomeScreen: public QWidget
{
public:
    HomeScreen(QuestionManagerController* questionManagerController, LeerhulpmiddelMainWindow* parent);

public slots:
    void DisplayWidget(QWidget* displayWidget);

private slots:
    void startInboxAnimation();

private:
    QWidget* GenerateTopButtonBar();

    QuestionsetBrowser* m_vragensetBrowser;
    QWidget* m_rightSideScreen;
    QHBoxLayout* m_container;

    LeerhulpmiddelMainWindow* m_mainWindow;

    InboxView* m_inboxView;
    QPropertyAnimation* m_inboxAnimation;
};

#endif // HOMESCREEN_H
