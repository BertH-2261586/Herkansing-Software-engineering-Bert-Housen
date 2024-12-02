#ifndef LEERHULPMIDDELMAINWINDOW_H
#define LEERHULPMIDDELMAINWINDOW_H

#include <QPushButton>
#include <QGridLayout>
#include <QBoxLayout>
#include <QLabel>
#include <QStack>

#include "../Controller/questionmanagercontroller.h"
#include "qmainwindow.h"

class LeerhulpmiddelMainWindow: public QMainWindow
{
public:
    LeerhulpmiddelMainWindow(QuestionManagerController* questionManagerController);
    ~LeerhulpmiddelMainWindow() { delete m_container; delete m_homeScreenWidget; }
    
    void PushMainViewport(QWidget* newViewport);

    void PopMainViewport();

private:
    QStack<QWidget*> m_mainViewStack;

    QLayout* m_container;
    QWidget* m_homeScreenWidget;
};

#endif // LEERHULPMIDDELMAINWINDOW_H
