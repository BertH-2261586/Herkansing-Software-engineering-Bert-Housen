#include "leerhulpmiddelmainwindow.h"
#include "HomeScreen/homescreen.h"
#include <QDebug>
#include <QPushButton>

#include "Examination/createExaminationView.h"

LeerhulpmiddelMainWindow::LeerhulpmiddelMainWindow(QuestionManagerController* questionManagerController)
{
    setWindowTitle("Learning aid");
    setWindowState(Qt::WindowMaximized);

    m_container = new QVBoxLayout();

    QPushButton* btnStartExamination = new QPushButton("Start Examination");

    connect(btnStartExamination, &QPushButton::pressed, this, [=] {
        SetMainViewport(new CreateExaminationView());
    });

    //TODO nog een default widget zetten
    m_containedWidget = new HomeScreen(questionManagerController);
    m_containedWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    m_container->addWidget(btnStartExamination);
    m_container->addWidget(m_containedWidget);
    m_container->setContentsMargins(0, 0, 0, 0);

    QWidget* window = new QWidget();
    window->setLayout(m_container);

    setCentralWidget(window);
    //setStyleSheet("background-color:white;");

}


// Zet de nieuwe mainViewPort, wordt gebruikt als je van viewport wilt switchen.
//TODO kijken of dit beter met een stack systeem kan gebueren.
void LeerhulpmiddelMainWindow::SetMainViewport(QWidget* newViewPort)
{
    m_container->replaceWidget(m_containedWidget, newViewPort);

    m_containedWidget->setParent(nullptr);
    m_containedWidget->deleteLater();

    m_containedWidget = newViewPort;
}

