#include "MultipleChoiceView.h"

/*
* This function sets up the MultipleChoice examination view properly
*
* @pre the question parameter is a question class that is filled with the correct and filled variables (for example: no empty answer)
* @param question this is the question where you'll get the data for the question from
*/
void MultipleChoiceExaminationView::setQuestion(const MultipleChoiceQuestion* question) {
    // Clear previous question widgets and layout (to avoid memory leaks)
    clearPreviousQuestion();    

    m_mainQuestionLayout = new QVBoxLayout(this);

    // Set the question label and radio buttons correctly
    setQuestionLabel(question);
    setRadioButtons(question);

    // Add them to the layout
    m_mainQuestionLayout->addWidget(m_questionLabel, Qt::AlignHCenter);
    m_mainQuestionLayout->addLayout(m_radioButtonLayout);

    setLayout(m_mainQuestionLayout);
}

// This function sets the question label correctly
void MultipleChoiceExaminationView::setQuestionLabel(const MultipleChoiceQuestion* question) {
    // Initialize the label 
    m_questionLabel = new QLabel(question->getQuestion(), this);
    m_questionLabel->setStyleSheet(
        "color: palette(windowText); "
        "font-size: 25px; "
        "margin-bottom: 15px;"
    );
    m_questionLabel->setWordWrap(true);  // Allow the text to wrap within the label

    // Set the minimum and maximum sizes and the correct alignments and sizepolicy
    m_questionLabel->setMinimumWidth(300);
    m_questionLabel->setMinimumHeight(100);
    m_questionLabel->setMaximumWidth(1150);
    m_questionLabel->setMaximumHeight(1500);
    m_questionLabel->setAlignment(Qt::AlignCenter);
    m_questionLabel->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
}

// This function sets the radio buttons for the multiple choice correctly
void MultipleChoiceExaminationView::setRadioButtons(const MultipleChoiceQuestion* question) {
    // Initialize the grid layout for the radio buttons
    m_radioButtonLayout = new QGridLayout;
    m_radioButtonLayout->setHorizontalSpacing(225);
    m_radioButtonLayout->setVerticalSpacing(15);
    m_radioButtonLayout->setSizeConstraint(QLayout::SetFixedSize);
    m_buttonGroup = new QButtonGroup(this);

    // Initialize the needed variables to be able to setup the radio buttons correctly
    int IDnum = 0;
    int row = 0;
    int column = 0;
    QList<QString> possibilities = question->getAnswer().getAnswers();
    // Iterate over every possible solution and make a radio button for it
    for (const QString possibility : possibilities) {
        // Initialize the radio button
        QRadioButton* radio = new QRadioButton(possibility);
        m_radioButtonList.append(radio);
        m_buttonGroup->addButton(radio, IDnum);
        radio->setMaximumWidth(325);

        // Initialize a metric for the font 
        QFontMetrics metrics(radio->font());
        int textWidth = metrics.horizontalAdvance(possibility);
        // If the text width exceeds the available width of the radio button, setup a tooltip that displays the full answer that got cut short
        if (textWidth > radio->width()) {
            radio->setToolTip(
                "<div style='color: palette(windowText); font-size: 8pt;'>"
                + possibility +
                "</div>"
            );
        }
        else {
            radio->setToolTip(""); // No tooltip if the text fits
        }

        radio->setStyleSheet(
            "QRadioButton {"
                "color: palette(windowText); "
            "}"
            // The indicator of the radio button
            "QRadioButton::indicator {"
                "width: 10px;"
                "height: 10px;"
                "border-radius: 6px;"
                "border: 1px solid palette(dark);"
                "background-color: palette(base);"
            "}"
            // The indicator is checked/selected
            "QRadioButton::indicator:checked {"
                "background-color: palette(highlight);"  
                "border: 2px solid palette(dark);"       
                "border-radius: 6px;"
            "}"
            // The indicator is not checked but you are hovering over it
            "QRadioButton::indicator:unchecked:hover {"
                "background-color: palette(light);"
                "border: 2px solid palette(mid);"
                "border-radius: 6px;"
            "}"
            // The radio button is checked but you're also hovering over it
            "QRadioButton::indicator:checked:hover {"
                "background-color: palette(highlight);"  
                "border: 2px solid palette(dark);"
                "border-radius: 6px;"
            "}"
        );

        // Add the radio button to the grid layout at the appropriate position (row, column)
        m_radioButtonLayout->addWidget(radio, row, column, Qt::AlignCenter);

        // Update column and row for the next radio button
        row++;
        // Move to the next column after 5 rows
        if (row == 5) { 
            row = 0;
            column++;
        }
        ++IDnum;
    }
}

// Delete all the data from a previous question so that you can display a new one 
void MultipleChoiceExaminationView::clearPreviousQuestion() {
    // Remove and delete widgets in the main layout if it exists
    if (m_mainQuestionLayout) {
        // Delete all widgets in the layout
        QLayoutItem* item;
        while ((item = m_mainQuestionLayout->takeAt(0)) != nullptr) {
            if (item->widget()) {
                item->widget()->deleteLater(); // Schedule the widgets for deletion
            }
            delete item;
        }
        // Delete and reset the layout
        delete m_mainQuestionLayout; 
        m_mainQuestionLayout = nullptr; 
    }

    for (QRadioButton* button : m_radioButtonList) {
        if (button) {
            button->deleteLater();
        }
    }

    // Delete and reset the button group if it exists
    if (m_buttonGroup) {
        delete m_buttonGroup; 
        m_buttonGroup = nullptr; 
    }

    // Reset the remaining member variables associated with the previous question
    m_questionLabel = nullptr; 
    m_radioButtonList.clear();
}

void MultipleChoiceExaminationView::showAnswer(int correctAnswer) {
    // Disable all the radio buttons so it cant accept more input
    for (QRadioButton* radio : m_radioButtonList) {
        radio->setEnabled(false);
    }

    for (int i = 0; i < m_radioButtonList.size(); ++i) {      
        // Only change the style sheet for selected and correct answers
        if (m_radioButtonList[i]->isChecked() || i == correctAnswer) {
            // Check if the selected answer is correct 
            QString color = (i == correctAnswer || correctAnswer == -1) ? "green" : "red";

            // Set the stylesheet using the determined color
            m_radioButtonList[i]->setStyleSheet(
                QString(
                    "QRadioButton {"
                        "color: %1; "
                    "}"
                    "QRadioButton::indicator {"
                        "width: 10px;"
                        "height: 10px;"
                        "border-radius: 6px;"
                        "border: 1px solid palette(dark);"
                        "background-color: palette(base);"
                    "}"
                    "QRadioButton::indicator:checked {"
                        "background-color: palette(highlight);"
                        "border: 2px solid palette(dark);"
                        "border-radius: 6px;"
                    "}"
                ).arg(color));
        }
    }
}

QString MultipleChoiceExaminationView::getCheckedAnswers() {
    // Iterate through all radio buttons with their index
    for (int i = 0; i < m_radioButtonList.size(); ++i) {
        QRadioButton* button = m_radioButtonList[i];

        if (button && button->isChecked()) {
            // Since you can only select one radio button return the selected one 
            return button->text();
        }
    }

    // There is no answer selected
    return "";
}