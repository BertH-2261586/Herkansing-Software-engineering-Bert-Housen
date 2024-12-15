#include "add_friend_view.h"
#include "../ToastMessage.h"

#include <QScreen>

AddFriendView::AddFriendView(QWidget* parent) : QWidget(parent) {
    this->setAttribute(Qt::WA_DeleteOnClose, true);

    // Get the first page of users
    m_addFriendController.getUsersByPage(1);

    setGoBack();
    setSearchBar();
    setUserSearch();

    setMainLayout();

    // Update the view with the fetched users
    connect(&m_addFriendController, &addFriendController::usersFetched, this, [=]() {
        setAllUsers();
    });
}

void AddFriendView::setGoBack() {
    m_goBack = new QPushButton("<- Go back");
    m_goBack->setStyleSheet(
        "border: 0px;"
        "font-size: 25px;"
    );
    m_goBack->setCursor(Qt::PointingHandCursor);
    connect(m_goBack, &QPushButton::clicked, this, &AddFriendView::goBack);
}

void AddFriendView::setSearchBar() {
    m_searchBar = new QLineEdit();
    m_searchBar->setPlaceholderText("Search for user...");

    // Add the magnifying glass icon
    QAction* searchAction = new QAction(QIcon("resources/magnifying-glass.png"), "", m_searchBar);
    m_searchBar->addAction(searchAction, QLineEdit::LeadingPosition);

    m_searchBar->setStyleSheet(
        "QLineEdit {"
        "    border: 2px solid gray;"     
        "    border-radius: 10px;"        
        "    padding: 5px;"              
        "    font-size: 14px;"            
        "    background: white;"        
        "    color: black;"               
        "}"
        "QLineEdit:focus {"
        "    border: 2px solid blue;"     
        "    background: #f0f8ff;"       
        "}");

    // Connect signal for text changes
    connect(m_searchBar, &QLineEdit::textChanged, this, [=](const QString text) {
        deleteUsersFromList();
        m_addFriendController.getUsersByPage(1, text);         // Always start from the first page when typing something new
    });
}

void AddFriendView::setUserSearch() {
    m_userSearch = new QVBoxLayout;
    m_userSearchWidget = new QWidget();

    m_noUserFound = new QLabel("No users found");
    m_noUserFound->setStyleSheet(
        "QLabel {"
            "border: none;"  // Ensure no border is applied to the label by the frame
        "}"
    );
    if (m_addFriendController.getUserPageSize() != 0) {
        m_noUserFound->hide();
    }
    m_userSearch->addWidget(m_noUserFound);
    
    setAllUsers();

    m_userSearchFrame = new QFrame(m_userSearchWidget);
    m_userSearchFrame->setLayout(m_userSearch);
    m_userSearchFrame->setFrameShape(QFrame::Box);
    m_userSearchFrame->setFrameShadow(QFrame::Raised);

    // Set the stylesheet
    m_userSearchFrame->setStyleSheet(
    "  border-width: 1px;"
    "  border-style: solid;"
    "  border-color: black;"
    "  background-color: grey;"
    );
    QScreen* screen = QGuiApplication::primaryScreen();
    QRect screenGeometry = screen->geometry();
    int screenWidth = screenGeometry.width();
    m_userSearchFrame->setFixedWidth(screenWidth * 0.3);
    m_userSearchFrame->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Maximum);
}

void AddFriendView::setAllUsers() {
    if (m_addFriendController.getUserPageSize() == 0) {
        m_noUserFound->show();
    }
    else {
        m_noUserFound->hide();
    }
    qDebug() << m_addFriendController.getUserPageSize();
    for (int i = 0; i < m_addFriendController.getUserPageSize(); ++i) {
        QString username = m_addFriendController.getUser(i);
        qDebug() << "name " <<  username;
        m_userLayouts.append(new QHBoxLayout());
        m_userNames.append(new QLabel(username));
        m_userNames[i]->setStyleSheet(
            "QLabel {"
                "border: none;"  // Ensure no border is applied to the label by the frame
            "}"
        );
        m_addUserButtons.append(new QPushButton("Add as friend"));
        connect(m_addUserButtons[i], &QPushButton::clicked, this, [=]() {
            qDebug() << "Added user: " + m_addFriendController.getUser(i);
            ToastMessage* toast = new ToastMessage("Sent " + m_addFriendController.getUser(i) + " a friend request.", this);
            toast->setFixedWidth(200);
            toast->move((width() - toast->width()) / 2, height() - 85);
            toast->show();
        });

        // Set the stylesheet
        m_addUserButtons[i]->setStyleSheet(
            "QPushButton  {"
                "background-color: lightgray;"
                "padding-left: 10px;"
                "padding-right: 10px;"
                "margin-left: 15px;"
                "color: black;"
                "border: 1px solid gray;"
                "border-radius: 5px;"
            "}"
            "QPushButton:hover {"
                "background-color: white;"
                "color: black;"
            "}"
        );
        m_userLayouts[i]->addWidget(m_userNames[i]);
        m_userLayouts[i]->addWidget(m_addUserButtons[i]);

        m_frames.append(new QFrame());
        m_frames[i]->setLayout(m_userLayouts[i]);
        m_frames[i]->setFrameShape(QFrame::Box);
        m_frames[i]->setFrameShadow(QFrame::Raised);
        m_frames[i]->setStyleSheet(
            "  border-width: 1px;"
            "  border-style: solid;"
            "  border-color: black;"
        );
        m_userSearch->addWidget(m_frames[i]);
    }

}

void AddFriendView::setMainLayout() {
	m_mainLayout = new QVBoxLayout;
    m_mainLayout->addWidget(m_goBack, 0, Qt::AlignLeft | Qt::AlignTop);
    m_mainLayout->addWidget(m_searchBar, 0, Qt::AlignHCenter);
    m_mainLayout->addWidget(m_userSearchFrame, 0, Qt::AlignHCenter);

    setLayout(m_mainLayout);
}

void AddFriendView::deleteUsersFromList() {
    // Removing and re-adding all users is necessary, or else the indices wont reset and will cause index out of bounds errors
    // For example item in place 3 stays 3 even when 2 is removed

    // Remove all the widgets associated with the item from the inbox 
    for (int i = 0; i < m_addFriendController.getUserPageSize(); ++i) {
        delete m_userLayouts[0];
        m_userLayouts.removeAt(0);          // Remove 0 every time because m_inboxRequests gets smaller with the deletions
        delete m_userNames[0];
        m_userNames.removeAt(0);
        delete m_addUserButtons[0];
        m_addUserButtons.removeAt(0);
        delete m_frames[0];
        m_frames.removeAt(0);
    }
}