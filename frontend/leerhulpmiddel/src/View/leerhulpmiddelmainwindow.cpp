#include "leerhulpmiddelmainwindow.h"
#include "HomeScreen/homescreen.h"
#include <QDebug>
#include <QPushButton>

#include "Examination/examinationView.h"

LeerhulpmiddelMainWindow::LeerhulpmiddelMainWindow(QuestionManagerController* questionManagerController)
{
    setWindowTitle("Qt 6.8 Boilerplate Window");
    setWindowState(Qt::WindowMaximized);

    m_container = new QVBoxLayout();

    QPushButton* btnStartExamination = new QPushButton("Start Examination");
    btnStartExamination->hide();



    m_homeScreenWidget = new HomeScreen(questionManagerController, this);
    m_homeScreenWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    m_container->addWidget(btnStartExamination);
    m_container->addWidget(m_homeScreenWidget);

    m_mainViewStack.push(m_homeScreenWidget);

    m_container->setContentsMargins(0, 0, 0, 0);

    QWidget* window = new QWidget();
    window->setLayout(m_container);

    setCentralWidget(window);
    //setStyleSheet("background-color:white;");

}


// Zet de nieuwe mainViewPort, wordt gebruikt als je van viewport wilt switchen.
void LeerhulpmiddelMainWindow::PushMainViewport(QWidget* newViewport)
{
    m_container->replaceWidget(m_mainViewStack.top(), newViewport);
    m_mainViewStack.top()->setParent(nullptr);

    connect(newViewport, &QWidget::destroyed, this, [=]{
        qDebug() << "pooped";
        PopMainViewport();
    });

    m_mainViewStack.push(newViewport);
}

//Gaat het huidige viewport poppen, gaat nooit het homescreen wegdoen aangezien er dan niks meer displayed wordt
void LeerhulpmiddelMainWindow::PopMainViewport()
{
    if (m_mainViewStack.length() == 1)
    {
        return;
    }

    QWidget* prevViewport = m_mainViewStack.pop();
    m_container->replaceWidget(prevViewport, m_mainViewStack.top());

    prevViewport->setParent(nullptr);
    prevViewport->deleteLater();
}

