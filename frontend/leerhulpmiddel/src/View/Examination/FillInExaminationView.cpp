#include "FillInExaminationView.h"

#include <QRegularExpression>
#include <QRegularExpressionMatchIterator>
#include <QTextEdit>

/*
* This function sets up the fill in examination view properly
*
* @pre the question parameter is a question class that is filled with the correct and filled variables (for example: no empty answer)
* @param question this is the question where you'll get the data for the question from
*/
void FillInExaminationView::setQuestion(FillInQuestion* question) {
    m_currentQuestion = question;

    QVBoxLayout* mainLayout = new QVBoxLayout(this);

    // Create a row of text with a space for user input
    QHBoxLayout* questionLayout = new QHBoxLayout;

    QString questionString = QString::fromStdString(question->getQuestion());

    QRegularExpression re("\\[([^\\]]+)\\]");
    QRegularExpressionMatchIterator i = re.globalMatch(questionString);

    int lastIndex = 0;

    // Iterate through the matches and build the display
    while (i.hasNext()) {
        QRegularExpressionMatch match = i.next();

        // Get the text before the match
        QString textBefore = questionString.mid(lastIndex, match.capturedStart() - lastIndex);
        lastIndex = match.capturedEnd();

        // Add the text before as a normal QLabel
        if (!textBefore.isEmpty()) {
            QLabel* textLabel = new QLabel(textBefore, this);
            textLabel->setAlignment(Qt::AlignLeft);
            questionLayout->addWidget(textLabel);
        }

        QString answerText = match.captured(1); // Get text inside []
        QTextEdit* answerEdit = new QTextEdit;
        answerEdit->setMaximumHeight(25);
        answerEdit->setAlignment(Qt::AlignCenter);
        answerEdit->setMinimumWidth(75); // Set a minimum width
        answerEdit->setMaximumWidth(250); // Set a minimum width
        int currentWidth = 75;

        // Automatically adjust width as the text changes
        connect(answerEdit, &QTextEdit::textChanged, [answerEdit, &currentWidth]() {
            // Calculate the required width for the current text
            int docWidth = answerEdit->document()->size().width();
            int newWidth = std::max(currentWidth, docWidth + 10); // Add some padding

            // Ensure the new width doesn't exceed the maximum
            if (newWidth > 250) {
                newWidth = 250; // Cap the width at the maximum
            }

            // Only update if the width has increased
            if (newWidth > currentWidth) {
                currentWidth = newWidth; // Update the current width
                answerEdit->setFixedWidth(newWidth); // Adjust the width
            }
            });
        questionLayout->addWidget(answerEdit);
    }

    // Add the remaining part of the string after the last match
    QString remainingText = questionString.mid(lastIndex);
    if (!remainingText.isEmpty()) {
        QLabel* remainingLabel = new QLabel(remainingText, this);
        remainingLabel->setAlignment(Qt::AlignLeft);
        questionLayout->addWidget(remainingLabel);
    }
    
    mainLayout->addLayout(questionLayout);
    // Set the layout
    setLayout(mainLayout);
}
