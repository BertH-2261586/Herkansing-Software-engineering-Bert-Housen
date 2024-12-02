#include "homescreen.h"
#include "../Examination/createExaminationView.h"
#include "../Login/LoginView.h"
#include "../../Controller/LoginController.h"
#include "../../model/manager/NetworkManager.h"
#include "../leerhulpmiddelmainwindow.h"
#include <QDebug>

HomeScreen::HomeScreen(QuestionManagerController* questionManagerController, LeerhulpmiddelMainWindow* parent) : m_mainWindow(parent), QWidget(parent)
{
    QVBoxLayout* main_container = new QVBoxLayout();
    main_container->setContentsMargins(0, 0, 0, 0);
    main_container->setSpacing(0);

    m_container = new QHBoxLayout();
    m_container->setContentsMargins(0, 0, 0, 0);

    m_vragensetBrowser = new QuestionsetBrowser(questionManagerController->getAllQuestionsets(), questionManagerController, this);
    m_container->addWidget(m_vragensetBrowser, 1);

    m_rightSideScreen = new QWidget();
    m_container->addWidget(m_rightSideScreen, 4);

    main_container->addWidget(GenerateTopButtonBar());
    main_container->addLayout(m_container);

    setLayout(main_container);
}


QWidget* HomeScreen::GenerateTopButtonBar()
{
    QPushButton* startExamButton = new QPushButton("Start examination");
    connect(startExamButton, &QPushButton::pressed, this, [=] {
        m_mainWindow->PushMainViewport(new CreateExaminationView());
    });

    QPushButton* makeNewQsetButton = new QPushButton("Make new Question set");

    QPushButton* logoutButton = new QPushButton("Logout");
    logoutButton->hide();
    //TODO: connect logout

    QPushButton* loginButton = new QPushButton("Login/Register");
    connect(loginButton, &QPushButton::pressed, this, [=] {
        NetworkManager* networkManager = new NetworkManager();
        LoginView* loginView = new LoginView(new LoginController(networkManager));

        connect(networkManager, &NetworkManager::loginFailed,
            loginView, &LoginView::failedLoginFeedback);
        connect(networkManager, &NetworkManager::registerFailed,
            loginView, &LoginView::failedRegisterFeedback);
        connect(networkManager, &NetworkManager::loginSuccess,
            this, [=] {
                loginButton->hide();
                logoutButton->show();

                m_mainWindow->PopMainViewport();
            });

        m_mainWindow->PushMainViewport(loginView);
    });


    QHBoxLayout* container = new QHBoxLayout();
    container->addWidget(startExamButton);
    container->addWidget(makeNewQsetButton);
    container->addWidget(logoutButton);
    container->addWidget(loginButton);

    QWidget* outputWidget = new QWidget();
    outputWidget->setStyleSheet(""""
                                "background-color: #5c5c5c;"
                                ""
                                ""
                                ""
                                ""
                                """");


    outputWidget->setLayout(container);
    return outputWidget;
}


void HomeScreen::DisplayWidget(QWidget* displayWidget)
{
    m_container->replaceWidget(m_rightSideScreen, displayWidget);

    m_rightSideScreen->setParent(nullptr);
    m_rightSideScreen->deleteLater();

    m_rightSideScreen = displayWidget;
}
