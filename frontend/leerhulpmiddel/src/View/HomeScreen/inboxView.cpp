#include "inboxView.h"
#include "homeScreen.h"  // Include the full definition of HomeScreen here

InboxView::InboxView(QWidget* parent) : QWidget(parent) {
    setSlidingMenu();

    // Create a QScrollArea to make the menu scrollable
    m_scrollArea = new QScrollArea(m_slidingMenu);
    m_scrollArea->setWidgetResizable(true);
    m_slidingMenuLayout->addWidget(m_scrollArea);

    // Initialize a layout so that you can add content to the scrollarea
    m_menuContentWidget = new QWidget();
    m_menuContentWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    m_menuLayout = new QVBoxLayout(m_menuContentWidget);
    m_scrollArea->setWidget(m_menuContentWidget);

    // Cast the qwidget parent to homescreen to make connect recognize it
    HomeScreen* homeScreen = qobject_cast<HomeScreen*>(parent);
    connect(this, &InboxView::removeInboxItem, homeScreen, &HomeScreen::removeInboxItem);

    // Set the text for when there are no inbox items
    m_noItemsInInbox = new QLabel("The inbox is currently empty");
    m_noItemsInInbox->setStyleSheet("font-size: 15px; font-weight: bold;");
    m_noItemsInInbox->hide();
    m_menuLayout->addWidget(m_noItemsInInbox, 0, Qt::AlignCenter);
    setInboxRequests();

    setMainLayout();
}

void InboxView::setInboxRequests() {
    if (m_inboxRequests.size() == 0) {
        m_noItemsInInbox->show();
    }

    for (int i = 0; i < m_inboxRequests.size(); ++i) {
        m_menuItemLayouts.append(new QGridLayout());
        m_menuItemInfo.append(new QLabel("Friend " + m_inboxRequests[i] + " has sent a questionset"));
        m_menuItemInfo[i]->setWordWrap(true);

        setAcceptButton(i);
        setRejectButton(i);

        setMenuItemLayout(i);
        addFrame(i);
    }
}

// Set the GUI of the reject button in a inbox item
void InboxView::setAcceptButton(int index) {
    m_acceptButtons.append(new QPushButton("Accept"));
    connect(m_acceptButtons[index], &QPushButton::clicked, this, [=]() {
        acceptedQuestionSet(index);
    });

    // Set the stylesheet
    m_acceptButtons[index]->setStyleSheet(
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
}

// Set the GUI of the reject button in a inbox item
void InboxView::setRejectButton(int index) {
    m_rejectButtons.append(new QPushButton("Reject"));
    connect(m_rejectButtons[index], &QPushButton::clicked, this, [=]() {
        rejectedQuestionSet(index);
    });

    // Set the stylesheet
    m_rejectButtons[index]->setStyleSheet(
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
}

void InboxView::setMenuItemLayout(int index) {
    // Add all items to the layout
    m_menuItemLayouts[index]->addWidget(m_menuItemInfo[index], 0, 0, 1, 3);                 // Let the text span across all 3 columns
    m_menuItemLayouts[index]->addWidget(m_acceptButtons[index], 1, 0, Qt::AlignHCenter);
    m_menuItemLayouts[index]->addWidget(m_rejectButtons[index], 1, 2, Qt::AlignHCenter);

    // Create spacing
    m_menuItemLayouts[index]->setSpacing(5); 
    m_menuItemLayouts[index]->setContentsMargins(5, 5, 5, 5);
}

void InboxView::addFrame(int index) {
    m_itemFrames.append(new QFrame(m_menuContentWidget));
    m_itemFrames[index]->setLayout(m_menuItemLayouts[index]);
    m_itemFrames[index]->setFrameShape(QFrame::Box);
    m_itemFrames[index]->setFrameShadow(QFrame::Raised);

    // Set the stylesheet
    m_itemFrames[index]->setObjectName("itemFrame");      // Set an object name for specific styling of the frame
    m_itemFrames[index]->setStyleSheet(
        "#itemFrame {"
        "  border-width: 1px;"
        "  border-style: solid;"
        "  border-color: black;"
        "}"
    );
    m_itemFrames[index]->setMaximumHeight(125);
    m_itemFrames[index]->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);

    m_menuLayout->addWidget(m_itemFrames[index]);
}

void InboxView::setMainLayout() {
    m_mainLayout = new QVBoxLayout();
    m_mainLayout->addWidget(m_slidingMenu);
    // Make it so there is no spacing around the slidingMenu
    m_mainLayout->setContentsMargins(0, 0, 0, 0);
    m_mainLayout->setSpacing(0);

    setLayout(m_mainLayout);
}

void InboxView::setSlidingMenu() {
    // Calculate the inbox menu width and screen width for initialization
    QScreen* screen = QGuiApplication::primaryScreen();
    QRect screenGeometry = screen->geometry();
    int screenWidth = screenGeometry.width();
    int menuWidth = screenWidth * 0.25;

    // Create the sliding menu widget
    m_slidingMenu = new QWidget(this);
    m_slidingMenu->setStyleSheet("background-color: #5c5c5c;");
    m_slidingMenu->setFixedSize(menuWidth, screenGeometry.height() - 150);       

    // Set a layout for the sliding menu
    m_slidingMenuLayout = new QVBoxLayout();
    m_slidingMenu->setLayout(m_slidingMenuLayout);  
}

void InboxView::acceptedQuestionSet(int index) {
    deleteInboxItem(index);
    emit removeInboxItem();
}

void InboxView::rejectedQuestionSet(int index) {
    deleteInboxItem(index);
    emit removeInboxItem();
}

void InboxView::deleteInboxItem(int index) {
    // Removing and re-adding all items is necessary, or else the indices wont reset and will cause index out of bounds errors
    // For example item in place 3 stays 3 even when 2 is removed

    // Remove all the items associated with the item from the inbox 
    for (int i = 0; i < m_inboxRequests.size(); ++i) {
        delete m_menuItemLayouts[0];
        m_menuItemLayouts.removeAt(0);          // Remove 0 every time because m_inboxRequests gets smaller with the deletions
        delete m_menuItemInfo[0];
        m_menuItemInfo.removeAt(0);
        delete m_acceptButtons[0];
        m_acceptButtons.removeAt(0);
        delete m_rejectButtons[0];
        m_rejectButtons.removeAt(0);
        delete m_itemFrames[0];
        m_itemFrames.removeAt(0);
    }

    m_inboxRequests.removeAt(index);

    // Re add the remaining items than wont need to be removed
    setInboxRequests();
}