#ifndef LEERHULPMIDDELMAINWINDOW_H
#define LEERHULPMIDDELMAINWINDOW_H

#include "qmainwindow.h"
#include <QPushButton>
#include <QGridLayout>
#include <QBoxLayout>
#include <QLabel>
#include "../model/manager/questionmanager.h"


class LeerhulpmiddelMainWindow: public QMainWindow
{
public:
    LeerhulpmiddelMainWindow(QuestionManager &questionManager);

    void SetMainViewport(QWidget* newViewPort);

private:
    QLayout* m_container;
    QWidget* m_containedWidget;
};

#endif // LEERHULPMIDDELMAINWINDOW_H
