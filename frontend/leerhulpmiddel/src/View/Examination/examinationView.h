#ifndef EXAMINATIONWINDOW_H
#define EXAMINATIONWINDOW_H

#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QLineEdit>
#include <QVBoxLayout>
#include <QMessageBox>

#include "../../model/fileManager.h"
#include "../../model/Questions/Question.h"
#include "../../model/Questions/MultipleChoiceQuestion.h"
#include "../../model/Questions/FillInQuestion.h"
#include "../../model/Questions/Flashcard.h"
#include "../../model/countdownTimer.h"
#include "../../model/Questions/Answer.h"
#include "FlashcardExaminationView.h"
#include "FillInExaminationView.h"
#include "MultipleChoiceExaminationView.h"

class ExaminationView : public QWidget {
    Q_OBJECT

public:
    explicit ExaminationView(QWidget* parent = nullptr);

private slots:
    void nextQuestion();
    void checkAnswer();
    void closeWindow() { close(); }
    void closeEvent(QCloseEvent* event) override; // Override close event
    void onCountdownFinished();

private:
    void setCurrentQuestionView();
    void clearPreviousQuestionView();

    QLabel* amountOfQuestionsAnswered;
    QPushButton* submitButton;
    QPushButton* closeButton;
    QPushButton* nextQuestionButton;
    QPushButton* endExaminationButton;

    FileManager m_fileManager;
    CountdownTimer* timePerQuestion;
    MultipleChoiceExaminationView multipleChoiceView = MultipleChoiceExaminationView();
    FlashcardExaminationView flashcardView = FlashcardExaminationView();
    FillInExaminationView fillInView = FillInExaminationView();

    QVector<Question*> m_questions;
    int m_currentQuestionIndex;
    int m_totalAmountQuestions;
    bool m_closeFromExaminationEnd = false;         // Deze bool houd bij als er op de "end examination" knop is geklikt, zodat je de close event kunt bypassen
};

#endif // EXAMINATIONWINDOW_H
