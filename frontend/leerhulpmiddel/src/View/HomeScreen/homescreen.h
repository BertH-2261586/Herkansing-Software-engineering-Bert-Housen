#ifndef HOMESCREEN_H
#define HOMESCREEN_H

#include "qwidget.h"
#include "questionsetbrowser.h"
#include "../../model/manager/questionmanager.h"
#include <QGridLayout>
#include <QHBoxLayout>



class HomeScreen: public QWidget
{
public:
    HomeScreen(QuestionManager &manager);


private:
    QuestionsetBrowser* m_vragensetBrowser;
    QWidget* m_rightSideScreen;
    QHBoxLayout* m_container;


public slots:
    void DisplayWidget(QWidget* displayWidget);
};

#endif // HOMESCREEN_H
