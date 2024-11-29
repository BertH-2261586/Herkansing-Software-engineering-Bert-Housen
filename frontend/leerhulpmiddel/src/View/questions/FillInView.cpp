#include "FillInView.h"

#include <QRegularExpression>
#include <QRegularExpressionMatchIterator>
#include <QTextEdit>
#include <QScrollArea>
/*
* This function sets up the fill in examination view properly
*
* @pre the question parameter is a question class that is filled with the correct and filled variables (for example: no empty answer)
* @param question this is the question where you'll get the data for the question from
*/
void FillInView::setQuestion(const FillInQuestion* question) {
    m_mainLayout = new QVBoxLayout(this);
    m_mainLayout->setSpacing(10);

    // Create a row of text with a space for user input
    m_questionLayout = new QFlowLayout;

    QString questionString = question->getQuestion();
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
            textLabel->setStyleSheet("font-size: 18px;");
            m_textLabels.append(textLabel);
            m_questionLayout->addWidget(textLabel);
        }

        QWidget* answerBlockWidget = new QWidget(this);
        QVBoxLayout* fillInLayout = new QVBoxLayout(answerBlockWidget);
        QString answerText = match.captured(1); // Get text inside []
        QTextEdit* answerEdit = new QTextEdit;
        QLabel* correctAnswer = new QLabel;

        answerEdit->setMaximumSize(85, 30);
        fillInLayout->addWidget(answerEdit);
        m_answerInputs.append(answerEdit);

        correctAnswer->setText(answerText);
        correctAnswer->setStyleSheet("font-weight: bold; color: green; font-size: 18px;");
        fillInLayout->addWidget(correctAnswer, 0, Qt::AlignHCenter);
        m_correctAnswer.append(correctAnswer);
        correctAnswer->hide();

        m_fillInLayouts.append(fillInLayout);
        m_questionLayout->addWidget(answerBlockWidget);
    }

    // Add the remaining part of the string after the last match
    QString remainingText = questionString.mid(lastIndex);
    if (!remainingText.isEmpty()) {
        QLabel* remainingLabel = new QLabel(remainingText, this);
        remainingLabel->setAlignment(Qt::AlignLeft);
        remainingLabel->setStyleSheet("font-size: 18px;");
        m_textLabels.append(remainingLabel);
        m_questionLayout->addWidget(remainingLabel);
    }

    QWidget* contentWidget = new QWidget(this);
    contentWidget->setLayout(m_questionLayout);

    // Create a QScrollArea and set the content widget inside it
    QScrollArea* scrollArea = new QScrollArea(this);
    scrollArea->setWidget(contentWidget);
    scrollArea->setWidgetResizable(true);
    scrollArea->setMinimumHeight(150);  // Set a minimum width for the scroll area
    scrollArea->setMinimumWidth(200);  // Set a minimum width for the scroll area
    scrollArea->setMaximumHeight(800);
    scrollArea->setMaximumWidth(1250);
    scrollArea->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    m_mainLayout->addWidget(scrollArea);
    // Set the layout
    setLayout(m_mainLayout);
}

void FillInView::showAnswer(QVector<int> wrongAnswers) {
    for (int i = 0; i < m_answerInputs.size(); ++i) {
        m_answerInputs[i]->setReadOnly(true);
        m_correctAnswer[i]->show();

        if (!wrongAnswers.contains(i)) {
            // Correct answer, set background to green
            m_answerInputs[i]->setStyleSheet("background-color: green;");
            m_correctAnswer[i]->setText("");      // Set the correct answer to empty so that it still pushes the textedit upwards but not displaying the text
        }
        else {
            // Incorrect answer, set background to red
            m_answerInputs[i]->setStyleSheet("background-color: red;");
        }
    }
}

QVector<QString> FillInView::getAllAnswerText() {
    QVector<QString> answerText;

    for (QTextEdit* input : m_answerInputs) {
        answerText.append(input->toPlainText().trimmed());  // Get the plain text entered            
    }

    return answerText;
}

void FillInView::clearPreviousQuestion() {
    // Clear all layouts in m_fillInLayouts
    qDeleteAll(m_fillInLayouts);
    m_fillInLayouts.clear();

    // Clear all labels in m_textLabels
    qDeleteAll(m_textLabels);
    m_textLabels.clear();

    // Clear all answer inputs in m_answerInputs
    qDeleteAll(m_answerInputs);
    m_answerInputs.clear();

    // Clear all correct answer labels in m_correctAnswer
    qDeleteAll(m_correctAnswer);
    m_correctAnswer.clear();


    // Clear the question layout if it is not nullptr
    if (m_questionLayout) {
        delete m_questionLayout;  // Deletes the layout and all its contents
        m_questionLayout = nullptr;  // Set to nullptr after deletion
    }

    // Clear the main layout if it is not nullptr
    if (m_mainLayout) {
        delete m_mainLayout;  // Deletes the layout and all its contents
        m_mainLayout = nullptr;  // Set to nullptr after deletion
    }
}
