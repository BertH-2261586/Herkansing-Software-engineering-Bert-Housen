#ifndef EXAMINATIONWINDOW_H
#define EXAMINATIONWINDOW_H

#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QLineEdit>
#include <QVBoxLayout>
#include <QMessageBox>

#include "../../model/countdownTimer.h"
#include "FlashcardExaminationView.h"
#include "FillInExaminationView.h"
#include "MultipleChoiceExaminationView.h"

class ExaminationController;

class ExaminationView : public QWidget {
    Q_OBJECT

public:
    explicit ExaminationView(QWidget* parent = nullptr);
    void startExamination(QString path) { emit examinationStarted(path); }

signals:
    void examinationStarted(const QString questionSetPath);
    void nextQuestion();

public slots:
    void flashcardHasBeenFlipped();
    void questionLoadedView();

private slots:
    void nextQuestionView();
    void checkAnswer();
    void closeWindow() { this->close(); }
    void closeEvent(QCloseEvent* event) override; // Override close event

private:
    void setCurrentQuestionView();
    void clearPreviousQuestionView();

    QLabel* amountOfQuestionsAnswered;
    QPushButton* submitButton;
    QPushButton* closeButton;
    QPushButton* nextQuestionButton;
    QPushButton* endExaminationButton;

    CountdownTimer* timePerQuestion;
    MultipleChoiceExaminationView multipleChoiceView = MultipleChoiceExaminationView();
    FlashcardExaminationView flashcardView = FlashcardExaminationView();
    FillInExaminationView fillInView = FillInExaminationView();

    ExaminationController* m_examinationController;

    bool m_closeFromExaminationEnd = false;         // Deze bool houd bij als er op de "end examination" knop is geklikt, zodat je de close event kunt bypassen
};

#endif
