#include "homescreen.h"
#include "../Examination/createExaminationView.h"
#include "../Login/LoginView.h"
#include "../leerhulpmiddelmainwindow.h"
#include <QDebug>

HomeScreen::HomeScreen(QuestionManagerController* questionManagerController, LeerhulpmiddelMainWindow* parent) : m_mainWindow(parent), QWidget(parent)
{
    m_networkManager = new NetworkManager();
    m_loginController = new LoginController(m_networkManager);

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

HomeScreen::~HomeScreen() {
    delete m_loginController;
    delete m_networkManager;
}


QWidget* HomeScreen::GenerateTopButtonBar()
{
    QPushButton* startExamButton = new QPushButton("Start examination");
    connect(startExamButton, &QPushButton::pressed, this, [=] {
        m_mainWindow->PushMainViewport(new CreateExaminationView());
    });

    QPushButton* makeNewQsetButton = new QPushButton("Make new Question set");

    QPushButton* logoutButton = new QPushButton("Logout");
    logoutButton->setFixedSize(100, 30);    //Fixed size to prevent Qt from throwing errors when switching between hide and show

    QPushButton* loginButton = new QPushButton("Login/Register");
    loginButton->setFixedSize(100, 30);

    connect(m_networkManager, &NetworkManager::loggedIn, this, [=] {
        loginButton->hide();
        logoutButton->show();
    });

    connect(m_networkManager, &NetworkManager::loggedOut, this, [=] {
        logoutButton->hide();
        loginButton->show();
    });
    
    connect(logoutButton, &QPushButton::pressed, this, [=] {
        m_loginController->logout();

        loginButton->show();
        logoutButton->hide();
    });

    connect(loginButton, &QPushButton::pressed, this, [=] {
        LoginView* loginView = new LoginView(new LoginController(m_networkManager));

        connect(m_networkManager, &NetworkManager::loginFailed,
            loginView, &LoginView::failedLoginFeedback);
        connect(m_networkManager, &NetworkManager::registerFailed,
            loginView, &LoginView::failedRegisterFeedback);
        connect(m_networkManager, &NetworkManager::loginSuccess,
            this, [=] {
                loginButton->hide();
                logoutButton->show();

                m_mainWindow->PopMainViewport();
            });

        m_mainWindow->PushMainViewport(loginView);
    });

    m_loginController->getLoggedInStatus();

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
