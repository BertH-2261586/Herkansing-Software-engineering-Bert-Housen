#include "homescreen.h"
#include "../Examination/createExaminationView.h"
#include "../Login/LoginView.h"
#include "../../Controller/LoginController.h"
#include "../leerhulpmiddelmainwindow.h"

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


    // Create the inbox sliding menu
    m_inboxView = new InboxView(this);
    m_container->addWidget(m_inboxView);

    // Calculate the inbox menu width and screen width for initialization
    int screenWidth = this->width();
    int menuWidth = screenWidth * 0.25;
    // Create the animation for sliding the menu
    m_inboxAnimation = new QPropertyAnimation(m_inboxView, "pos");
    m_inboxAnimation->setDuration(500);
    m_inboxAnimation->setStartValue(QPoint(screenWidth, 50));            // Starting position off-screen
    m_inboxAnimation->setEndValue(QPoint(screenWidth - menuWidth, 50));

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

    QPushButton* loginButton = new QPushButton("Login/Register");
    connect(loginButton, &QPushButton::pressed, this, [=] {
        m_mainWindow->PushMainViewport(new LoginView(new LoginController()));
    });
    
    // Create the inbox button
    QPushButton* inboxButton = new QPushButton("");
    // Set the inbox icon
    QIcon startExamIcon("resources/inbox-icon.png");
    inboxButton->setIcon(startExamIcon);
    inboxButton->setIconSize(QSize(25, 25));

    // Initialize the button
    inboxButton->setStyleSheet(
        "QPushButton {"
        "   border: 1px solid black;"
        "   border-radius: 15px;"
        "   background-color: #5c5c5c;"
        "   width: 35px;"
        "   height: 35px;"
        "}"
        "QPushButton:hover {"
        "   background-color: white;"
        "}"
    );
    inboxButton->setFixedSize(35, 35); 
    inboxButton->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    connect(inboxButton, &QPushButton::pressed, this, &HomeScreen::startInboxAnimation);

    // Add all the buttons to the layout
    QHBoxLayout* container = new QHBoxLayout();
    container->addWidget(startExamButton);
    container->addWidget(makeNewQsetButton);
    container->addWidget(loginButton);
    container->addWidget(inboxButton);

    QWidget* outputWidget = new QWidget();
    outputWidget->setStyleSheet("background-color: #5c5c5c;");
    outputWidget->setLayout(container);
    return outputWidget;
}

void HomeScreen::startInboxAnimation() {
    // Calculate the inbox menu width and screen width for initialization
    int screenWidth = this->width();
    int menuWidth = screenWidth * 0.25;

    if (m_inboxView->x() == screenWidth - menuWidth) {
        // If the menu is visible, slide it out (to the right)
        m_inboxAnimation->setStartValue(QPoint(screenWidth - menuWidth, 50));
        m_inboxAnimation->setEndValue(QPoint(screenWidth, 50));
    }
    else {
        // If the menu is hidden, slide it in (to the left)
        m_inboxAnimation->setStartValue(QPoint(screenWidth, 50));
        m_inboxAnimation->setEndValue(QPoint(screenWidth - menuWidth, 50));
    }

    m_inboxAnimation->start();  
}


void HomeScreen::DisplayWidget(QWidget* displayWidget)
{
    m_container->replaceWidget(m_rightSideScreen, displayWidget);

    m_rightSideScreen->setParent(nullptr);
    m_rightSideScreen->deleteLater();

    m_rightSideScreen = displayWidget;
}
