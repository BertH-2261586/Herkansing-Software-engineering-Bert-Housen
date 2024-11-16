#include "examinationView.h"
#include "../../Controller/examinationController.h"

#include <QCloseEvent>
#include <QToolTip>

ExaminationView::ExaminationView(QWidget* parent) : QWidget(parent), m_examinationController(new ExaminationController(this)) {
    // Create the widgets directly for the page
    amountOfQuestionsAnswered = new QLabel("", this);
    amountOfQuestionsAnswered->setStyleSheet(
        "color: palette(windowText); "
        "font-size: 25px; "
        "font-weight: bold;"
    );
    amountOfQuestionsAnswered->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    amountOfQuestionsAnswered->adjustSize();

    int timeLimitMinutes = 1;
    timePerQuestion = new CountdownTimer(this, timeLimitMinutes); 
    timePerQuestion->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    timePerQuestion->setStyleSheet(
        "color: palette(windowText); "
        "font-size: 25px; "
        "font-weight: bold;"
    );
    connect(timePerQuestion, &CountdownTimer::countdownFinished, this, &ExaminationView::checkAnswer);

    submitButton = new QPushButton("Submit Answer", this);
    submitButton->setStyleSheet(
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
    submitButton->setCursor(Qt::PointingHandCursor);
    connect(submitButton, &QPushButton::clicked, this, &ExaminationView::checkAnswer);

    closeButton = new QPushButton("X", this);
    closeButton->setStyleSheet(
        "color: palette(windowText); "
        "border: none; "
        "font-size: 50px; "
        "padding-top: 10px;"
        "padding-right: 10px;"
    );
    closeButton->setCursor(Qt::PointingHandCursor);
    closeButton->setToolTip(
        "<div style='color: palette(windowText); font-size: 12pt;'>"
        + QString::fromStdString("End the examination") +
        "</div>"
    );
    connect(closeButton, &QPushButton::clicked, this, &ExaminationView::closeWindow);

    nextQuestionButton = new QPushButton("Next Question ->", this);
    nextQuestionButton->setStyleSheet(
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
    nextQuestionButton->setCursor(Qt::PointingHandCursor);
    connect(nextQuestionButton, &QPushButton::clicked, this, &ExaminationView::nextQuestionView);
    nextQuestionButton->hide();

    endExaminationButton = new QPushButton("End the examination", this);
    endExaminationButton->setStyleSheet(
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
    endExaminationButton->setCursor(Qt::PointingHandCursor);
    connect(endExaminationButton, &QPushButton::clicked, this, [this]() {
        m_closeFromExaminationEnd = true;
        closeWindow();  
    });    
    endExaminationButton->hide();

    // Layout for the widgets
    QVBoxLayout* mainLayout = new QVBoxLayout(this);

    // Add the close button at the top-right
    mainLayout->addWidget(closeButton, 0, Qt::AlignRight);

    // Create the layout you want to add inside the frame
    QHBoxLayout* questionInfoLayout = new QHBoxLayout();
    questionInfoLayout->addWidget(amountOfQuestionsAnswered, 0, Qt::AlignTop);
    questionInfoLayout->addWidget(timePerQuestion, 0, Qt::AlignTop);
    mainLayout->addLayout(questionInfoLayout);

    connect(&flashcardView, &FlashcardExaminationView::flashcardHasBeenFlipped, this, &ExaminationView::flashcardHasBeenFlipped);
    mainLayout->addWidget(&flashcardView, 0, Qt::AlignHCenter);
    mainLayout->addWidget(&multipleChoiceView, 0, Qt::AlignHCenter);
    mainLayout->addWidget(&fillInView, 0, Qt::AlignHCenter);

    // Add the question label, answer input, and submit button
    mainLayout->addWidget(submitButton, 0, Qt::AlignHCenter);
    mainLayout->addWidget(nextQuestionButton, 0, Qt::AlignHCenter);
    mainLayout->addWidget(endExaminationButton, 0, Qt::AlignHCenter);

    // Set the layout for the window
    setLayout(mainLayout);

    startExamination("C:/Users/evens/project-software-engineering-groep_7/frontend/leerhulpmiddel/questionSets/test");
}

void ExaminationView::questionLoadedView() {
    int questionIndex = m_examinationController->getCurrentQuestionNumber();
    int totalQuestionAmount = m_examinationController->getTotalAmountOfQuestions();
    amountOfQuestionsAnswered->setText(QString::number(questionIndex) + "/" + QString::number(totalQuestionAmount));

    setCurrentQuestionView();
}

void ExaminationView::nextQuestionView() {
    clearPreviousQuestionView();

    emit nextQuestion();

    if (m_examinationController->getCurrentQuestionType() != QuestionType::Flashcard) {
        submitButton->show();
    }
    nextQuestionButton->hide();

    timePerQuestion->resetTimer();
    timePerQuestion->startCountdown();
}

void ExaminationView::checkAnswer() {
    timePerQuestion->pauseCountdown();

    //m_examinationController.checkAnswer();
    QuestionType questionType = m_examinationController->getCurrentQuestionType();
    if (questionType == QuestionType::Flashcard) {
        flashcardView.handleQuestionClicked();
        nextQuestionButton->show();
    }
    else if (questionType == QuestionType::MultipleChoice) {
        multipleChoiceView.showAnswer(dynamic_cast<const MultipleChoiceQuestion*>(m_examinationController->getQuestion()));
    }
    //else if (questionType == QuestionType::FillIn) {
    //    fillInView.setQuestion(dynamic_cast<FillInQuestion*>(m_questions[m_currentQuestionIndex]));
    //}

    if (m_examinationController->getCurrentQuestionNumber() == m_examinationController->getTotalAmountOfQuestions()) {       
        endExaminationButton->show();
    }
    else {
        nextQuestionButton->show();
    }
    submitButton->hide();
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
        flashcardView.setQuestion(dynamic_cast<const Flashcard*>(m_examinationController->getQuestion()));
    }
    else if (questionType == QuestionType::MultipleChoice) {
        multipleChoiceView.setQuestion(dynamic_cast<const MultipleChoiceQuestion*>(m_examinationController->getQuestion()));
    }
    //else if (questionType == QuestionType::FillIn) {
    //    fillInView.setQuestion(dynamic_cast<FillInQuestion*>(m_questions[m_currentQuestionIndex]));
    //}
}

void ExaminationView::clearPreviousQuestionView() {
    QuestionType questionType = m_examinationController->getCurrentQuestionType();
    if (questionType == QuestionType::Flashcard) {
        flashcardView.clearPreviousQuestion();
    }
    else if (questionType == QuestionType::MultipleChoice) {
        multipleChoiceView.clearPreviousQuestion();
    }
    //else if (questionType == QuestionType::FillIn) {
    //    fillInView.setQuestion(dynamic_cast<FillInQuestion*>(m_questions[m_currentQuestionIndex]));
    //}
}

void ExaminationView::flashcardHasBeenFlipped() { 
    nextQuestionButton->show(); 
    timePerQuestion->pauseCountdown(); 
}
