#include "examinationView.h"

#include <QCloseEvent>
#include <QToolTip>
#include <QScrollArea>
#include <QPalette>

// QPalet

ExaminationView::ExaminationView(QWidget* parent) : QWidget(parent), m_currentQuestionIndex(0) {
    // Dummy data
    m_questions.append(new MultipleChoiceQuestion("multi", "Wat is 2+2? Hoeveel kom je uit met deze rekenkundige som. Dit is een test qua lengte", "4"));
    //m_questions.append(new FillInQuestion("fillin", "[Bob] is cool net gelijk [Bob2] en [bob3]", "Bob"));
    m_questions.append(new Flashcard("flashcard", "Question testing word how", "Answer!"));
    m_questions.append(new MultipleChoiceQuestion("multi", "Wat is 2+2? Hoeveel kom je uit met deze rekenkundige som. Dit is een test qua lengte", "4"));
    m_questions.append(new Flashcard("flashcard", "Question testing word how", "Answer!"));
    m_totalAmountQuestions = m_questions.count();

    setCurrentQuestionView();

    // Create the widgets directly for the page
    amountOfQuestionsAnswered = new QLabel(QString::number(m_currentQuestionIndex + 1) + "/" + QString::number(m_totalAmountQuestions), this);
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
    connect(timePerQuestion, &CountdownTimer::countdownFinished, this, &ExaminationView::onCountdownFinished);

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
    nextQuestionButton->setStyleSheet("color: palette(windowText);");
    nextQuestionButton->setCursor(Qt::PointingHandCursor);
    connect(nextQuestionButton, &QPushButton::clicked, this, &ExaminationView::nextQuestion);
    nextQuestionButton->hide();

    endExaminationButton = new QPushButton("End the examination", this);
    endExaminationButton->setStyleSheet("color: palette(windowText);");
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

    mainLayout->addWidget(&flashcardView, 0, Qt::AlignHCenter);
    mainLayout->addWidget(&multipleChoiceView, 0, Qt::AlignHCenter);
    mainLayout->addWidget(&fillInView, 0, Qt::AlignHCenter);

    // Add the question label, answer input, and submit button
    mainLayout->addWidget(submitButton, 0, Qt::AlignHCenter);
    mainLayout->addWidget(nextQuestionButton);

    // Set the layout for the window
    setLayout(mainLayout);

    // Connect submitButton's signal to checkAnswer
    connect(submitButton, &QPushButton::clicked, this, [this]() {
        checkAnswer();  // Pass the captured text to checkAnswer function
    });
}

void ExaminationView::nextQuestion() {
    clearPreviousQuestionView();
    qDebug() << "next";
    submitButton->show();
    nextQuestionButton->hide();
    ++m_currentQuestionIndex;
    timePerQuestion->resetTimer();
    timePerQuestion->startCountdown();

    setCurrentQuestionView();

    amountOfQuestionsAnswered->setText(QString::number(m_currentQuestionIndex + 1) + "/" + QString::number(m_totalAmountQuestions));
}

void ExaminationView::checkAnswer() {
    timePerQuestion->pauseCountdown();

    if (m_currentQuestionIndex + 1 == m_questions.count()) {        // Tel bij de index + 1, want het begint te tellen van 0 i.p.v. 1
        endExaminationButton->show();
    }
    else {
        nextQuestionButton->show();
    }
    submitButton->hide();

    Question* currentQuestion = m_questions[m_currentQuestionIndex];
    QuestionType questionType = currentQuestion->getQuestionType();
    //questionLabel->setText(QString::fromStdString(currentQuestion->getAnswer()));
}

void ExaminationView::onCountdownFinished() {
    qDebug() << "times up";
    checkAnswer();
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
    QuestionType questionType = m_questions[m_currentQuestionIndex]->getQuestionType();
    if (questionType == QuestionType::Flashcard) {
        flashcardView.setQuestion(dynamic_cast<Flashcard*>(m_questions[m_currentQuestionIndex]));
    }
    else if (questionType == QuestionType::MultipleChoice) {
        multipleChoiceView.setQuestion(dynamic_cast<MultipleChoiceQuestion*>(m_questions[m_currentQuestionIndex]));
    }
    //else if (questionType == QuestionType::FillIn) {
    //    fillInView.setQuestion(dynamic_cast<FillInQuestion*>(m_questions[m_currentQuestionIndex]));
    //}
}

void ExaminationView::clearPreviousQuestionView() {
    QuestionType questionType = m_questions[m_currentQuestionIndex]->getQuestionType();
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