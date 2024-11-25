#include "examinationView.h"
#include "../../Controller/examinationController.h"
#include "scoreCardExaminationView.h"

#include <QCloseEvent>
#include <QToolTip>

ExaminationView::ExaminationView(QWidget* parent) : QWidget(parent), m_examinationController(new ExaminationController(this)) {
    // Create the widgets directly for the page
    m_amountOfQuestionsAnswered = new QLabel("", this);
    m_amountOfQuestionsAnswered->setStyleSheet(
        "color: palette(windowText); "
        "font-size: 25px; "
        "font-weight: bold;"
    );
    m_amountOfQuestionsAnswered->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    m_amountOfQuestionsAnswered->adjustSize();

    int timeLimitMinutes = 1;
    m_timePerQuestion = new CountdownTimer(this, timeLimitMinutes);
    m_timePerQuestion->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    m_timePerQuestion->setStyleSheet(
        "color: palette(windowText); "
        "font-size: 25px; "
        "font-weight: bold;"
    );
    connect(m_timePerQuestion, &CountdownTimer::countdownFinished, this, &ExaminationView::showAnswer);

    m_submitButton = new QPushButton("Submit Answer", this);
    m_submitButton->setStyleSheet(
        "QPushButton {"
        "   color: palette(ButtonText); "
        "   background-color: palette(button);"             // Background color (green)
        "   border: 2px solid palette(mid);"             // Border with a darker green
        "   border-radius: 5px;"                    // Rounded corners
        "   padding: 8px 16px;"                     // Padding for a better button shape
        "   margin-bottom: 25px;"
        "   font-size: 16px;"                       // Font size
        "   max-width: 175px;"                      // Limit the width of the button
        "}"
        "QPushButton:hover {"
        "   background-color: palette(mid);"             // Darker green on hover
        "}"
    );
    m_submitButton->setCursor(Qt::PointingHandCursor);
    connect(m_submitButton, &QPushButton::clicked, this, &ExaminationView::showAnswer);

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

    m_nextQuestionButton = new QPushButton("Next Question ->", this);
    m_nextQuestionButton->setStyleSheet(
        "QPushButton {"
        "   color: palette(ButtonText); "
        "   background-color: palette(button);"             // Background color (green)
        "   border: 2px solid palette(mid);"             // Border with a darker green
        "   border-radius: 5px;"                    // Rounded corners
        "   padding: 8px 16px;"                     // Padding for a better button shape
        "   margin-bottom: 25px;"
        "   font-size: 16px;"                       // Font size
        "   max-width: 175px;"                      // Limit the width of the button
        "}"
        "QPushButton:hover {"
        "   background-color: palette(mid);"             // Darker green on hover
        "}"
    );
    m_nextQuestionButton->setCursor(Qt::PointingHandCursor);
    connect(m_nextQuestionButton, &QPushButton::clicked, this, &ExaminationView::nextQuestionView);
    m_nextQuestionButton->hide();

    m_endExaminationButton = new QPushButton("End the examination", this);
    m_endExaminationButton->setStyleSheet(
        "QPushButton {"
        "   color: palette(ButtonText); "
        "   background-color: palette(button);"             // Background color (green)
        "   border: 2px solid palette(mid);"             // Border with a darker green
        "   border-radius: 5px;"                    // Rounded corners
        "   padding: 8px 16px;"                     // Padding for a better button shape
        "   margin-bottom: 25px;"
        "   font-size: 16px;"                       // Font size
        "   max-width: 175px;"                      // Limit the width of the button
        "}"
        "QPushButton:hover {"
        "   background-color: palette(mid);"             // Darker green on hover
        "}"
    );
    m_endExaminationButton->setCursor(Qt::PointingHandCursor);
    connect(m_endExaminationButton, &QPushButton::clicked, this, [this]() {
        m_closeFromExaminationEnd = true;
        closeWindow();  
        //scoreCardExaminationView scorecard = scoreCardExaminationView(3, 0);
    });    
    m_endExaminationButton->hide();

    // Layout for the widgets
    QVBoxLayout* mainLayout = new QVBoxLayout(this);

    // Add the close button at the top-right
    mainLayout->addWidget(m_closeButton, 0, Qt::AlignRight);

    // Create the layout you want to add inside the frame
    QHBoxLayout* questionInfoLayout = new QHBoxLayout();
    questionInfoLayout->addWidget(m_amountOfQuestionsAnswered, 0, Qt::AlignTop);
    questionInfoLayout->addWidget(m_timePerQuestion, 0, Qt::AlignTop);
    mainLayout->addLayout(questionInfoLayout);

    connect(&m_flashcardView, &FlashcardExaminationView::flashcardHasBeenFlipped, this, &ExaminationView::flashcardHasBeenFlipped);
    mainLayout->addWidget(&m_flashcardView, 0, Qt::AlignHCenter);
    mainLayout->addWidget(&m_multipleChoiceView, 0, Qt::AlignHCenter);
    mainLayout->addWidget(&m_fillInView, 0, Qt::AlignHCenter);

    // Add the question label, answer input, and submit button
    mainLayout->addWidget(m_submitButton, 0, Qt::AlignHCenter);
    mainLayout->addWidget(m_nextQuestionButton, 0, Qt::AlignHCenter);
    mainLayout->addWidget(m_endExaminationButton, 0, Qt::AlignHCenter);

    // Set the layout for the window
    setLayout(mainLayout);

    startExamination("C:/Users/calvi/Documents/3de Bach/Software Engineering/project-software-engineering-groep_7/frontend/leerhulpmiddel/questionSets/test");
}

void ExaminationView::questionLoadedView() {
    int questionIndex = m_examinationController->getCurrentQuestionNumber();
    int totalQuestionAmount = m_examinationController->getTotalAmountOfQuestions();
    m_amountOfQuestionsAnswered->setText(QString::number(questionIndex) + "/" + QString::number(totalQuestionAmount));
    if (questionIndex == 1) {
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

    m_timePerQuestion->resetTimer();
    m_timePerQuestion->startCountdown();
}

void ExaminationView::showAnswer() {
    m_timePerQuestion->pauseCountdown();

    QuestionType questionType = m_examinationController->getCurrentQuestionType();
    if (questionType == QuestionType::Flashcard) {
        m_flashcardView.handleQuestionClicked();
    }
    else if (questionType == QuestionType::MultipleChoice) {
        QString checkedAnswers = m_multipleChoiceView.getCheckedAnswers();
        int correctAnswer = m_examinationController->checkMultipleChoiceAnswer(checkedAnswers);
        m_multipleChoiceView.showAnswer(correctAnswer);
    }
    else if (questionType == QuestionType::FillIn) {
        QVector<QString> answerText = m_fillInView.getAllAnswerText();
        QVector<int> wrongAnswers = m_examinationController->checkFillInAnswer(answerText);
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

void ExaminationView::flashcardHasBeenFlipped() { 
    m_timePerQuestion->pauseCountdown();

    // Check if the flashcard is the last card in the examination
    if (m_examinationController->getCurrentQuestionNumber() == m_examinationController->getTotalAmountOfQuestions()
        && m_examinationController->finishedExamination())
    {
        m_endExaminationButton->show();
    }
    else {
        m_nextQuestionButton->show();
    }
}
