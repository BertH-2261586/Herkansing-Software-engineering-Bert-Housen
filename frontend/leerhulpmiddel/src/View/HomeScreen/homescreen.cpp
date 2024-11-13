#include "homescreen.h"
#include "qlabel.h"
#include <QDebug>
#include "../../model/questionmanager.h"

HomeScreen::HomeScreen(QuestionManager& manager)
{
    //temp data

    m_container = new QHBoxLayout(this);
    m_container->setContentsMargins(0, 0, 0, 0);

    m_vragensetBrowser = new QuestionsetBrowser(manager.GetAllQuestionsets(), this);
    m_container->addWidget(m_vragensetBrowser, 1);

    m_rightSideScreen = new QWidget();
    m_container->addWidget(m_rightSideScreen, 4);


    setLayout(m_container);
}


void HomeScreen::DisplayVraag(QString tempString)
{
    QWidget* tempWidget = new QLabel(tempString);

    m_container->replaceWidget(m_rightSideScreen, tempWidget);

    m_rightSideScreen->setParent(nullptr);
    m_rightSideScreen->deleteLater();

    m_rightSideScreen = tempWidget;
}
