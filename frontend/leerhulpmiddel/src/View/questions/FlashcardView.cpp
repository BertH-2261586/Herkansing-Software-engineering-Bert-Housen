#include "FlashcardView.h"

/*
* This function sets up the flashcard examination view properly
*
* @pre the question parameter is a question class that is filled with the correct and filled variables (for example: no empty answer)
* @param question this is the question where you'll get the data for the question from
*/
void FlashcardView::setQuestion(const Flashcard* question) {
    // Clear previous question widgets and layout (to avoid memory leaks)
    clearPreviousQuestion();

    // Initialize the members
    m_currentQuestion = question;
    m_showingQuestion = true;

    // Initialze the question label and scroll area of the flashcard
    setQuestionLabel();
    setScrollArea();
    setupToggleSwitch();

    initializeLayouts();
}

// This function sets the question label correctly
void FlashcardView::setQuestionLabel() {
    m_questionLabel = new QLabel(this);
    m_questionLabel->setText(m_currentQuestion->getQuestion());
    m_questionLabel->setStyleSheet(
        "color: palette(windowText); "
        "border: 3px solid palette(mid); "
        "background-color: palette(base);"
        "font-size: 25px; "
    );
    m_questionLabel->setAlignment(Qt::AlignCenter);
    m_questionLabel->setCursor(Qt::PointingHandCursor);
    m_questionLabel->setToolTip(
        "<div style='color: palette(WindowText); font-size: 10pt;'>"
        + QString::fromStdString("Click to flip and see the answer") +
        "</div>"
    );
    m_questionLabel->installEventFilter(this);
    m_questionLabel->setMaximumWidth(500);  // Limit the width of the scroll area
    m_questionLabel->setMaximumHeight(500);
    m_questionLabel->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding); // Make scroll area expand

    // Set word wrap for long texts in the QLabel
    m_questionLabel->setWordWrap(true);
}

void FlashcardView::setScrollArea() {
    // Create a QScrollArea to make the text scrollable when it exceeds the visible area
    m_scrollArea = new QScrollArea(this);
    m_scrollArea->setWidget(m_questionLabel); // Set the QLabel as the widget inside the scroll area
    m_scrollArea->setWidgetResizable(true);  // Make the widget inside resize as needed
    m_scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff); // Disable horizontal scrolling

    // Set a fixed maximum height and width to control clipping
    m_scrollArea->setMaximumWidth(500);  // Limit the width of the scroll area
    m_scrollArea->setMaximumHeight(500); // Limit the height of the scroll area, text will be clipped here

    // Ensure the scroll area expands properly within the layout
    m_scrollArea->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding); // Make scroll area expand

}

void FlashcardView::setupToggleSwitch() {
    m_toggleLabel = new QLabel("Repeat the question later");
    m_toggleSwitch = new Switch(this);
}

void FlashcardView::initializeLayouts() {
    m_toggleLayout = new QVBoxLayout;
    m_toggleLayout->addWidget(m_toggleLabel);
    m_toggleLayout->addWidget(m_toggleSwitch);

    m_mainQuestionLayout = new QVBoxLayout(this);
    m_mainQuestionLayout->addWidget(m_scrollArea);
    m_mainQuestionLayout->addLayout(m_toggleLayout);

    setLayout(m_mainQuestionLayout);
}

bool FlashcardView::eventFilter(QObject* watched, QEvent* event) {
    if (watched == m_questionLabel && event->type() == QEvent::MouseButtonPress) {
        QMouseEvent* mouseEvent = static_cast<QMouseEvent*>(event);
        if (mouseEvent->button() == Qt::LeftButton) {
            handleQuestionClicked();
            return true;  // Event handled
        }
    }
    // Pass the event to the base class
    return QWidget::eventFilter(watched, event);
}

void FlashcardView::handleQuestionClicked() {
    if (m_showingQuestion) {
        Answer answer = m_currentQuestion->getAnswer();
        m_questionLabel->setText(answer.getAnswers().first());
        emit flashcardHasBeenFlipped();
    }
    else {
        m_questionLabel->setText(m_currentQuestion->getQuestion());
    }

    m_showingQuestion = !m_showingQuestion;
    m_toggleSwitch->hide();
    m_toggleLabel->hide();
}

// Delete all the data from a previous question so that you can display a new one 
void FlashcardView::clearPreviousQuestion() {
    if (m_toggleLayout) {
        // Iterate through and delete all items in m_toggleLayout
        QLayoutItem* item;
        while ((item = m_toggleLayout->takeAt(0)) != nullptr) {
            if (item->widget()) {
                item->widget()->deleteLater(); // Schedule the widget for deletion
            }
            delete item; // Delete the layout item
        }

        // Delete the layout itself
        delete m_toggleLayout;
        m_toggleLayout = nullptr;
    }

    // Remove the existing layout if it exists
    if (m_mainQuestionLayout) {
        // Delete all the widgets that were added to the layout
        QLayoutItem* item;
        while ((item = m_mainQuestionLayout->takeAt(0)) != nullptr) {
            if (item->widget()) {
                item->widget()->deleteLater(); // Schedule widget for deletion
            }
            delete item;
        }
        // Delete and reset the layout
        delete m_mainQuestionLayout;
        m_mainQuestionLayout = nullptr;
    }

    // Clean up member variables associated with the previous question
    m_questionLabel = nullptr; // Reset QLabel pointer
    m_scrollArea = nullptr;    // Reset QScrollArea pointer
    m_currentQuestion = nullptr; // Reset the Flashcard pointer
}