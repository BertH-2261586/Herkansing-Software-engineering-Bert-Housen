#include "inboxView.h"

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

    for (int i = 0; i < 20; ++i) {
        m_menuItemLayouts.append(new QGridLayout());
        m_menuItemInfo.append(new QLabel("Friend " + QString::number(i) + " has sent a questionset"));
        m_menuItemInfo[i]->setWordWrap(true);
    
        setAcceptButton(i);
        setRejectButton(i);

        setMenuItemLayout(i);
        addFrame(i);
    }

    setMainLayout();
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
    QFrame* itemFrame = new QFrame(m_menuContentWidget);
    itemFrame->setLayout(m_menuItemLayouts[index]);
    itemFrame->setFrameShape(QFrame::Box);
    itemFrame->setFrameShadow(QFrame::Raised);

    // Set the stylesheet
    itemFrame->setObjectName("itemFrame");      // Set an object name for specific styling of the frame
    itemFrame->setStyleSheet(
        "#itemFrame {"
        "  border-width: 1px;"
        "  border-style: solid;"
        "  border-color: black;"
        "}"
    );

    m_menuLayout->addWidget(itemFrame);
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
    m_slidingMenu->setFixedSize(menuWidth, screenGeometry.height());    // Set the size of the menu
    m_slidingMenu->move(screenWidth, 0);                                // Positioned off-screen to the right

    // Set a layout for the sliding menu
    m_slidingMenuLayout = new QVBoxLayout();
    m_slidingMenu->setLayout(m_slidingMenuLayout);  
}

void InboxView::acceptedQuestionSet(int index) {
    qDebug() << "accept " + QString::number(index);
}

void InboxView::rejectedQuestionSet(int index) {
    qDebug() << "reject " + QString::number(index);
}