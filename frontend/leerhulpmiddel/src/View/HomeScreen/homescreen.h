#ifndef HOMESCREEN_H
#define HOMESCREEN_H

#include <QGridLayout>
#include <QHBoxLayout>

#include "qwidget.h"
#include "questionsetbrowser.h"
#include "../../Controller/questionmanagercontroller.h"

class HomeScreen: public QWidget
{
public:
    HomeScreen(QuestionManagerController* questionManagerController);

private:
    QuestionsetBrowser* m_vragensetBrowser;
    QWidget* m_rightSideScreen;
    QHBoxLayout* m_container;

public slots:
    void DisplayWidget(QWidget* displayWidget);
};

#endif // HOMESCREEN_H
