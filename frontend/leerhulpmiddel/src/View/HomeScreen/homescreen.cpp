#include "homescreen.h"
#include "../Examination/createExaminationView.h"
#include "../Login/LoginView.h"
#include "../../Controller/LoginController.h"
#include "../leerhulpmiddelmainwindow.h"
#include "../add_friends/add_friend_view.h"
#include "../../Exceptions/NoSavedSessionException.h"

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
    
    setInboxView();

    main_container->addWidget(GenerateTopButtonBar());
    main_container->addLayout(m_container);

    setLayout(main_container);
}

void HomeScreen::setInboxView() {
    // Create the inbox sliding menu
    m_inboxView = new InboxView(this);
    m_container->addWidget(m_inboxView, 0, Qt::AlignTop);

    // Calculate the inbox menu width and screen width for initialization
    int screenWidth = this->width();
    int menuWidth = screenWidth * 0.25;
    // Create the animation for sliding the menu
    m_inboxAnimation = new QPropertyAnimation(m_inboxView, "pos");
    m_inboxAnimation->setDuration(500);
    m_inboxAnimation->setStartValue(QPoint(screenWidth, 50));            // Starting position off-screen
    m_inboxAnimation->setEndValue(QPoint(screenWidth - menuWidth, 50));
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
    

    // Add all the buttons to the layout
    QHBoxLayout* container = new QHBoxLayout();
    container->addWidget(startExamButton);
    container->addWidget(makeNewQsetButton);
    container->addWidget(loginButton);
    // Add the icon buttons to the layout
    setAddFriendButton(container);
    setInboxButton(container);

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

void HomeScreen::setAddFriendButton(QHBoxLayout* container) {
    // Create the inbox button
    m_addFriendButton = new QPushButton("");
    setIconButton(m_addFriendButton, "resources/add-friend.png");
    connect(m_addFriendButton, &QPushButton::pressed, this, [=]() {
        m_mainWindow->PushMainViewport(new AddFriendView());
    });

    // Add the widget to the container
    container->addWidget(m_addFriendButton);

    // Check if the user is logged in
    NetworkManager newNetworkManager = NetworkManager();

    // Show the button if the user is logged in
    newNetworkManager.cookieExists() ? m_addFriendButton->show() : m_addFriendButton->hide();
}

void HomeScreen::setInboxButton(QHBoxLayout* container) {
    // Create the inbox button
    m_inboxButton = new QPushButton("");
    setIconButton(m_inboxButton, "resources/inbox-icon.png");
    connect(m_inboxButton, &QPushButton::pressed, this, &HomeScreen::startInboxAnimation);

    // Set the label for the amount of requests inside of the inbox
    QWidget* buttonContainer = setInboxRequestLabel();

    // Add the buttonContainer to the main container layout
    container->addWidget(buttonContainer);

    // Check if the user is logged in
    NetworkManager newNetworkManager = NetworkManager();
    if(newNetworkManager.cookieExists()){
        m_inboxButton->show();
        m_requestAmountLabel->show();
    }
    // There is no cookie, so the user is not logged in yet
    else {
        m_inboxButton->hide();
        m_requestAmountLabel->hide();
    }
}

QWidget* HomeScreen::setInboxRequestLabel() {
    // Create a container for the button and request amount
    QWidget* buttonContainer = new QWidget();
    buttonContainer->setFixedSize(m_inboxButton->sizeHint());
    QVBoxLayout* inboxButtonLayout = new QVBoxLayout(buttonContainer);
    inboxButtonLayout->setContentsMargins(0, 0, 0, 0); 
    inboxButtonLayout->setSpacing(0);
    inboxButtonLayout->addWidget(m_inboxButton);

    // Create a request amount label
    m_requestAmountLabel = new QLabel(buttonContainer);
    // Set the request amount
    setInboxRequestAmount();

    // Set the stylesheet of the request amount
    m_requestAmountLabel->setAlignment(Qt::AlignCenter);
    m_requestAmountLabel->setStyleSheet(
        "QLabel {"
        "   background-color: red;"
        "   color: white;"
        "   font-size: 9px;"
        "   border-radius: 5px;"
        "   font-weight: bold;"
        "   max-width: 15px;"
        "   max-height: 15px;"
        "}"
    );
    m_requestAmountLabel->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
    m_requestAmountLabel->move(20, 0);
    m_requestAmountLabel->raise();                // Ensure the request amount is on top of the button

    return buttonContainer;
}

// Set the amount inside of the label that displays the amount of requests inside of the inbox
void HomeScreen::setInboxRequestAmount() {
    // Get the amount of requests and set the text correctly
    int requestAmount = m_inboxView->getAmountInboxRequests();
    if (requestAmount == 0) {
        m_requestAmountLabel->hide();
    }
    else {
        m_requestAmountLabel->setText(QString::number(requestAmount));
    }
}

/*
* Set the stylesheet of a button that contains an icon
* @pre you need a valid iconName
* @param button this is the button you want to change
* @param iconName this is the name of the icon
*/
void HomeScreen::setIconButton(QPushButton* button, QString iconName){
    // Set the icon
    QIcon startExamIcon(iconName);
    button->setIcon(startExamIcon);
    button->setIconSize(QSize(25, 25));

    // Initialize the button
    button->setStyleSheet(
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

    // Make it circular 
    button->setFixedSize(35, 35);
    button->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
}

void HomeScreen::DisplayWidget(QWidget* displayWidget)
{
    m_container->replaceWidget(m_rightSideScreen, displayWidget);

    m_rightSideScreen->setParent(nullptr);
    m_rightSideScreen->deleteLater();

    m_rightSideScreen = displayWidget;
}