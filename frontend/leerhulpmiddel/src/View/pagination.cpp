#include "pagination.h"
#include "../Controller/addFriendController.h"

Pagination::Pagination(int totalPageAmount, int currentPage, QWidget* parent) : QWidget(parent), m_totalPageAmount(totalPageAmount), 
    m_currentPage(currentPage) 
{
    // Set up the layout
    m_paginationLayout = new QHBoxLayout(this);
    m_paginationLayout->setSpacing(0);

    // Create and configure the buttons
    createNavigationButtons(true);
    createPageButtons();
    createNavigationButtons(false);

    // Disable/enable buttons based on current page
    updateNavigationButtonState();
}

// Function to create the previous and next buttons
void Pagination::createNavigationButtons(bool backwards) {
    // Ternary operators determine the sequence and text of the buttons
    QString fullMoveText = backwards ? "<<" : ">>";
    QString oneMoveText = backwards ? "<" : ">";
    int pageFullMove = backwards ? 1 : m_totalPageAmount;
    int pageOneMove = backwards ? m_currentPage - 1 : m_currentPage + 1;

    // Create the buttons
    QPushButton* fullMoveButton = createButton(fullMoveText);
    QPushButton* oneMoveButton = createButton(oneMoveText);

    // Connect the button press to a signal
    connect(fullMoveButton, &QPushButton::pressed, this, [=] {
        emit newPage(pageFullMove);
    });
    connect(oneMoveButton, &QPushButton::pressed, this, [=] {
        emit newPage(pageOneMove);
    });

    // Add the navigation buttons to the layout and list
    m_paginationLayout->addWidget(backwards ? fullMoveButton : oneMoveButton);
    m_paginationLayout->addWidget(backwards ? oneMoveButton : fullMoveButton);
    m_paginationButtons.append(backwards ? fullMoveButton : oneMoveButton);
    m_paginationButtons.append(backwards ? oneMoveButton : fullMoveButton);
}

// Create the buttons to navigate between pages
void Pagination::createPageButtons() {
    const int index_offset = 10;
    const int pages_before_current_page = 2;            // There are 2 pages (max) displayed before and after the current page
    const int pages_after_current_page = 2;

    int buttonsBeforeCreated = 0;
    int buttonsAfterCreated = 0;
    int startPageNumber = std::max(0, m_currentPage - 3);               // Display 2 before

    for (int i = startPageNumber; i < m_totalPageAmount; ++i) {
        // If the required number of "before current" buttons are created, jump to the current page.
        if (buttonsBeforeCreated == pages_before_current_page) {
            i = m_currentPage - index_offset;               // Start one before the current page 
            buttonsBeforeCreated = 0;
        }

        // If the required number of "after current" buttons are created, jump to the last few pages.
        if (buttonsAfterCreated == index_offset + pages_after_current_page) {
            // Check if there are enough pages left to add "..." (else it would be for example 9 ... 10)
            if (i != m_totalPageAmount - 3 && i != m_totalPageAmount - 2 && i != m_totalPageAmount - 1) {
                i = m_totalPageAmount - 3;
                QPushButton* pageButton = createButton("...");
                m_paginationLayout->addWidget(pageButton);
            }
            buttonsAfterCreated = 0;
        }

        // Check if your iterating over the current page, if so set the bool (to change the background color)
        bool selectedPage = (m_currentPage == i + index_offset) ? true : false;
        QPushButton* pageButton = createButton(QString::number(i + index_offset), selectedPage);

        // Connect so you can send a signal
        connect(pageButton, &QPushButton::pressed, this, [=] {
            emit newPage(i + index_offset);
        });

        m_paginationButtons.append(pageButton);
        m_paginationLayout->addWidget(pageButton);

        // Track how many "before current" or "after current" buttons have been created
        if (i >= m_currentPage - index_offset) {
            ++buttonsAfterCreated;
        }
        else {
            ++buttonsBeforeCreated;
        }
    }
}

// Helper function to create buttons with a specific text and callback
QPushButton* Pagination::createButton(const QString& text, bool selectedPage) {
    QPushButton* button = new QPushButton(text);

    // Set the stylesheet
    QString backgroundColor = selectedPage ? "blue" : "white";
    QString backgroundColorHover = selectedPage ? "lightblue" : "lightgrey";
    QString styleSheet = QString(
        "QPushButton {"
            "background-color: %1;"
            "color: black;"
        "}"
        "QPushButton:hover {"
            "background-color: %2;"
        "}"
        "QPushButton:disabled {"
            "background-color: grey;"
            "color: white;"
            "border: 1px solid darkgrey;"
        "}"
    ).arg(backgroundColor, backgroundColorHover);
    button->setStyleSheet(styleSheet);

    return button;
}

// Function to update the enable/disable state of navigation buttons
void Pagination::updateNavigationButtonState() {
    m_paginationButtons[0]->setEnabled(m_currentPage > 1);
    m_paginationButtons[1]->setEnabled(m_currentPage > 1);
    m_paginationButtons[m_paginationButtons.size() - 2]->setEnabled(m_currentPage < m_totalPageAmount);
    m_paginationButtons[m_paginationButtons.size() - 1]->setEnabled(m_currentPage < m_totalPageAmount);
}