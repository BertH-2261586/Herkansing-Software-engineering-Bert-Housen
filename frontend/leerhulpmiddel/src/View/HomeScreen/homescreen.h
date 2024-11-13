
#ifndef HOMESCREEN_H
#define HOMESCREEN_H

#include "qwidget.h"
#include "questionsetbrowser.h"
#include "../../model/questionmanager.h"
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
    void DisplayVraag(QString tempString);      //Dit veranderen naar een vraag klasse
};

#endif // HOMESCREEN_H
