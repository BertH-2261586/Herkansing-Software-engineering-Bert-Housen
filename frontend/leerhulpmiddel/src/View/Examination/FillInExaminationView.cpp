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
void FillInExaminationView::setQuestion(const FillInQuestion* question) {
    m_mainLayout = new QVBoxLayout(this);

    // Create a row of text with a space for user input
    m_questionLayout = new QHBoxLayout;

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
            m_textLabels.append(textLabel);
            m_questionLayout->addWidget(textLabel);
        }

        QVBoxLayout* fillInLayout = new QVBoxLayout;
        QString answerText = match.captured(1); // Get text inside []
        QTextEdit* answerEdit = new QTextEdit;
        QLabel* correctAnswer = new QLabel;

        answerEdit->setMaximumHeight(25);
        answerEdit->setAlignment(Qt::AlignCenter);
        answerEdit->setMinimumWidth(75); // Set a minimum width
        answerEdit->setMaximumWidth(250); // Set a minimum width

        correctAnswer->setText(answerText);
        correctAnswer->setStyleSheet("font-weight: bold; color: green;");
        fillInLayout->addWidget(correctAnswer, 0, Qt::AlignHCenter);
        m_correctAnswer.append(correctAnswer);
        correctAnswer->hide();

        fillInLayout->addWidget(answerEdit);
        m_answerInputs.append(answerEdit);

        m_fillInLayouts.append(fillInLayout);
        m_questionLayout->addLayout(fillInLayout);
    }

    // Add the remaining part of the string after the last match
    QString remainingText = questionString.mid(lastIndex);
    if (!remainingText.isEmpty()) {
        QLabel* remainingLabel = new QLabel(remainingText, this);
        remainingLabel->setAlignment(Qt::AlignLeft);
        m_textLabels.append(remainingLabel);
        m_questionLayout->addWidget(remainingLabel);
    }

    m_mainLayout->addLayout(m_questionLayout);
    // Set the layout
    setLayout(m_mainLayout);
}

void FillInExaminationView::showAnswer(const FillInQuestion* question) {
    QList<QString> answer = question->getAnswer().getAnswers();
    for (int i = 0; i < m_answerInputs.size(); ++i) {
        QTextEdit* inputField = m_answerInputs[i];
        QString userAnswer = inputField->toPlainText().trimmed().toLower(); // Get the user input
        QString correctAnswer = answer[i].trimmed().toLower();     // Get the corresponding correct answer
        m_answerInputs[i]->setReadOnly(true);

        if (userAnswer == correctAnswer) {
            // Correct answer, set background to green
            inputField->setStyleSheet("background-color: green;");
        }
        else {
            // Incorrect answer, set background to red
            inputField->setStyleSheet("background-color: red;");
            m_correctAnswer[i]->show();
        }
    }
}
