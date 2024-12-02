#include "examinationView.h"
#include "../../Controller/examinationController.h"

#include <QCloseEvent>
#include <QToolTip>

ExaminationView::ExaminationView(QWidget* parent) : QWidget(parent), m_examinationController(new ExaminationController(this)) {
    this->setAttribute(Qt::WA_DeleteOnClose, true);

    // Create the widgets directly for the page
    setupAmountQuestionsAnswered();
    setupTimer();
    setupSubmitButton();
    setupCloseButton();
    setupNextQuestionButton();
    setupEndExaminationButton();

    // Set the layouts correctly
    initializeLayouts();

    // Set the layout for the window
    setLayout(m_mainLayout);
}

// Display the amount of question the user answered
void ExaminationView::setupAmountQuestionsAnswered() {
    m_amountOfQuestionsAnswered = new QLabel("", this);
    m_amountOfQuestionsAnswered->setStyleSheet(
        "color: palette(windowText); "
        "font-size: 25px; "
        "font-weight: bold;"
    );
    m_amountOfQuestionsAnswered->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    m_amountOfQuestionsAnswered->adjustSize();
}

// Display the time the user has remaining to answer the question
void ExaminationView::setupTimer() {
    m_timePerQuestion = new CountdownTimer(this);
    m_timePerQuestion->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    m_timePerQuestion->setStyleSheet(
        "color: palette(windowText); "
        "font-size: 25px; "
        "font-weight: bold;"
    );
    connect(m_timePerQuestion, &CountdownTimer::countdownFinished, this, [this]() {
        showAnswer(true);
        });

    m_timePerQuestion->hide();
}

// Display the button that the user can click to show the answer
void ExaminationView::setupSubmitButton() {
    m_submitButton = new QPushButton("Submit Answer", this);
    m_submitButton->setStyleSheet(
        "QPushButton {"
        "   color: palette(ButtonText); "
        "   background-color: palette(button);"
        "   border: 2px solid palette(mid);"
        "   border-radius: 5px;"
        "   padding: 8px 16px;"
        "   margin-bottom: 25px;"
        "   font-size: 16px;"
        "   max-width: 175px;"
        "}"
        "QPushButton:hover {"
        "   background-color: palette(mid);"
        "}"
    );
    m_submitButton->setCursor(Qt::PointingHandCursor);
    connect(m_submitButton, &QPushButton::clicked, this, &ExaminationView::showAnswer);
}

// Display the button where the user can click to close the examination prematurely 
void ExaminationView::setupCloseButton() {
    m_closeButton = new QPushButton("X", this);
    m_closeButton->setStyleSheet(
        "color: palette(windowText); "
        "border: none; "
        "font-size: 50px; "
        "padding-top: 10px;"
        "padding-right: 10px;"
    );
    m_closeButton->setCursor(Qt::PointingHandCursor);
    m_closeButton->setToolTip(
        "<div style='color: palette(windowText); font-size: 12pt;'>"
        + QString::fromStdString("End the examination") +
        "</div>"
    );
    connect(m_closeButton, &QPushButton::clicked, this, &ExaminationView::closeWindow);

}

// Display the button that the user can click to go the next question
void ExaminationView::setupNextQuestionButton() {
    m_nextQuestionButton = new QPushButton("Next Question ->", this);
    m_nextQuestionButton->setStyleSheet(
        "QPushButton {"
        "   color: palette(ButtonText); "
        "   background-color: palette(button);"
        "   border: 2px solid palette(mid);"
        "   border-radius: 5px;"
        "   padding: 8px 16px;"
        "   margin-bottom: 25px;"
        "   font-size: 16px;"
        "   max-width: 175px;"
        "}"
        "QPushButton:hover {"
        "   background-color: palette(mid);"
        "}"
    );
    m_nextQuestionButton->setCursor(Qt::PointingHandCursor);
    connect(m_nextQuestionButton, &QPushButton::clicked, this, &ExaminationView::nextQuestionView);
    m_nextQuestionButton->hide();
}

// Display the button where the user can end the examination
void ExaminationView::setupEndExaminationButton() {
    m_endExaminationButton = new QPushButton("End the examination", this);
    m_endExaminationButton->setStyleSheet(
        "QPushButton {"
        "   color: palette(ButtonText); "
        "   background-color: palette(button);"
        "   border: 2px solid palette(mid);"
        "   border-radius: 5px;"
        "   padding: 8px 16px;"
        "   margin-bottom: 25px;"
        "   font-size: 16px;"
        "   max-width: 175px;"
        "}"
        "QPushButton:hover {"
        "   background-color: palette(mid);"
        "}"
    );
    m_endExaminationButton->setCursor(Qt::PointingHandCursor);
    connect(m_endExaminationButton, &QPushButton::clicked, this, [this]() {
        m_closeFromExaminationEnd = true;
        emit getExaminationData();
        });
    m_endExaminationButton->hide();
}

// Initialize all the layouts so that you can display the GUI
void ExaminationView::initializeLayouts() {
    m_mainLayout = new QVBoxLayout(this);

    // Add the close button at the top-right
    m_mainLayout->addWidget(m_closeButton, 0, Qt::AlignRight);

    // Create the layout that displays the examination info (timer, current question and total questions)
    m_questionInfoLayout = new QHBoxLayout();
    m_questionInfoLayout->addWidget(m_amountOfQuestionsAnswered, 0, Qt::AlignTop);
    m_questionInfoLayout->addWidget(m_timePerQuestion, 0, Qt::AlignTop);
    m_mainLayout->addLayout(m_questionInfoLayout);

    // Add all the types of question widgets to the layout 
    connect(&m_flashcardView, &FlashcardView::flashcardHasBeenFlipped, this, [this]() {
        showAnswer();
        });
    m_mainLayout->addWidget(&m_flashcardView, 0, Qt::AlignHCenter);
    m_mainLayout->addWidget(&m_multipleChoiceView, 0, Qt::AlignHCenter);
    m_mainLayout->addWidget(&m_fillInView, 0, Qt::AlignHCenter);

    // Add the question label, answer input, and submit button
    m_mainLayout->addWidget(m_submitButton, 0, Qt::AlignHCenter);
    m_mainLayout->addWidget(m_nextQuestionButton, 0, Qt::AlignHCenter);
    m_mainLayout->addWidget(m_endExaminationButton, 0, Qt::AlignHCenter);

    // Create the scorecard for after the examination
    m_scoreCard = new scoreCardExaminationView(this);
    m_scoreCard->hide();
    m_mainLayout->addWidget(m_scoreCard, 0, Qt::AlignCenter);
}

void ExaminationView::startExamination(QString path, QTime timeLimit) {
    if (timeLimit != QTime(-1, -1, -1)) {
        m_timePerQuestion->setupTimer(timeLimit);
        m_timePerQuestion->show();
        m_examinationController->setShowTimer(true);
    }

    emit examinationStarted(path);
}

void ExaminationView::questionLoadedView() {
    int questionIndex = m_examinationController->getCurrentQuestionNumber();
    int totalQuestionAmount = m_examinationController->getTotalAmountOfQuestions();
    m_amountOfQuestionsAnswered->setText(QString::number(questionIndex) + "/" + QString::number(totalQuestionAmount));
    if (questionIndex == 1 && m_examinationController->showTimer()) {
        m_timePerQuestion->startCountdown();
    }
    if (m_examinationController->getCurrentQuestionType() == QuestionType::Flashcard) {
        m_submitButton->hide();
    }

    setCurrentQuestionView();
}

void ExaminationView::nextQuestionView() {
    clearPreviousQuestionView();

    emit nextQuestion();

    if (m_examinationController->getCurrentQuestionType() != QuestionType::Flashcard) {
        m_submitButton->show();
    }
    m_nextQuestionButton->hide();

    if (m_examinationController->showTimer()) {
        m_timePerQuestion->resetTimer();
        m_timePerQuestion->startCountdown();
    }
}

void ExaminationView::showAnswer(bool timeout) {
    if (m_examinationController->showTimer()) {
        m_timePerQuestion->pauseCountdown();
    }

    QuestionType questionType = m_examinationController->getCurrentQuestionType();
    if (questionType == QuestionType::Flashcard) {
        bool repeat = m_flashcardView.getToggleStatus();
        m_examinationController->checkFlashCardAnswer(repeat, timeout);
    }
    else if (questionType == QuestionType::MultipleChoice) {
        QString checkedAnswers = m_multipleChoiceView.getCheckedAnswers();
        int correctAnswer = m_examinationController->checkMultipleChoiceAnswer(checkedAnswers, timeout);
        m_multipleChoiceView.showAnswer(correctAnswer);
    }
    else if (questionType == QuestionType::FillIn) {
        QVector<QString> answerText = m_fillInView.getAllAnswerText();
        QVector<int> wrongAnswers = m_examinationController->checkFillInAnswer(answerText, timeout);
        m_fillInView.showAnswer(wrongAnswers);
    }

    if (m_examinationController->getCurrentQuestionNumber() == m_examinationController->getTotalAmountOfQuestions()
        && m_examinationController->finishedExamination())
    {
        m_endExaminationButton->show();
    }
    else {
        m_nextQuestionButton->show();
    }
    m_submitButton->hide();
}

void ExaminationView::setCurrentQuestionView() {
    QuestionType questionType = m_examinationController->getCurrentQuestionType();
    if (questionType == QuestionType::Flashcard) {
        m_multipleChoiceView.hide();
        m_fillInView.hide();
        m_flashcardView.show();
        m_flashcardView.setQuestion(dynamic_cast<const Flashcard*>(m_examinationController->getQuestion()));
    }
    else if (questionType == QuestionType::MultipleChoice) {
        m_flashcardView.hide();
        m_fillInView.hide();
        m_multipleChoiceView.show();
        m_multipleChoiceView.setQuestion(dynamic_cast<const MultipleChoiceQuestion*>(m_examinationController->getQuestion()));
    }
    else if (questionType == QuestionType::FillIn) {
        m_multipleChoiceView.hide();
        m_flashcardView.hide();
        m_fillInView.show();
        m_fillInView.setQuestion(dynamic_cast<const FillInQuestion*>(m_examinationController->getQuestion()));
    }
}

void ExaminationView::clearPreviousQuestionView() {
    QuestionType questionType = m_examinationController->getCurrentQuestionType();
    if (questionType == QuestionType::Flashcard) {
        m_flashcardView.clearPreviousQuestion();
    }
    else if (questionType == QuestionType::MultipleChoice) {
        m_multipleChoiceView.clearPreviousQuestion();
    }
    else if (questionType == QuestionType::FillIn) {
        m_fillInView.clearPreviousQuestion();
    }
}

void ExaminationView::receiveExaminationData(QMap<QString, QString> examinationData) {
    // Hide all widgets except the score card
    hideAllWidgets(m_mainLayout);
    m_scoreCard->show();
    m_scoreCard->showExaminationData(examinationData);
}

// Make sure to hide all widgets so that you properly show the scorecard
void ExaminationView::hideAllWidgets(QLayout* layout) {
    for (int i = 0; i < layout->count(); ++i) {
        QLayoutItem* item = layout->itemAt(i);

        if (QWidget* widget = qobject_cast<QWidget*>(item->widget())) {
            widget->hide();
        }
        else if (QLayout* subLayout = item->layout()) {
            hideAllWidgets(subLayout);
        }
    }
}

void ExaminationView::closeEvent(QCloseEvent* event) {
    if (!m_closeFromExaminationEnd) {
        QMessageBox messageBox;
        messageBox.setWindowTitle("End examination?");
        messageBox.setText("Are you sure you want to end the examination? Your progress will not be saved.");
        messageBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
        messageBox.setIcon(QMessageBox::Question);

        // Apply custom styles to the message box (background to black, text to white)
        messageBox.setStyleSheet("color: palette(windowText);");

        // Show the message box and get the user's response
        int reply = messageBox.exec();

        // De gebruiker wil de ondervraging beeindigen en de window sluiten
        if (reply == QMessageBox::Yes) {
            event->accept();
        }
        // De gebruiker wil doorgaan met de ondervraging
        else {
            event->ignore();
        }
    }
}