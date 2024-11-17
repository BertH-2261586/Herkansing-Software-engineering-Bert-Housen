#ifndef LEERHULPMIDDELMAINWINDOW_H
#define LEERHULPMIDDELMAINWINDOW_H

#include <QPushButton>
#include <QGridLayout>
#include <QBoxLayout>
#include <QLabel>

#include "../Controller/questionmanagercontroller.h"
#include "qmainwindow.h"

class LeerhulpmiddelMainWindow: public QMainWindow
{
public:
    LeerhulpmiddelMainWindow(QuestionManagerController* questionManagerController);
    ~LeerhulpmiddelMainWindow() { delete m_container; delete m_containedWidget; }

    void SetMainViewport(QWidget* newViewPort);

private:
    QLayout* m_container;
    QWidget* m_containedWidget;
};

#endif // LEERHULPMIDDELMAINWINDOW_H
