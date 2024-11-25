#ifndef EXAMINATIONWINDOW_H
#define EXAMINATIONWINDOW_H

#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QLineEdit>
#include <QVBoxLayout>
#include <QMessageBox>

#include "../../model/countdownTimer.h"
#include "../questions/FlashcardView.h"
#include "../questions/FillInView.h"
#include "../questions/MultipleChoiceView.h"
#include "../../Controller/questionmanagercontroller.h"

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
    void showAnswer();
    void closeWindow() { this->close(); }
    void closeEvent(QCloseEvent* event) override; // Override close event

private:
    void setCurrentQuestionView();
    void clearPreviousQuestionView();

    QLabel* m_amountOfQuestionsAnswered;
    QPushButton* m_submitButton;
    QPushButton* m_closeButton;
    QPushButton* m_nextQuestionButton;
    QPushButton* m_endExaminationButton;

    CountdownTimer* m_timePerQuestion;
    MultipleChoiceExaminationView m_multipleChoiceView = MultipleChoiceExaminationView();
    FlashcardExaminationView m_flashcardView = FlashcardExaminationView();
    FillInExaminationView m_fillInView = FillInExaminationView();

    ExaminationController* m_examinationController;

    bool m_closeFromExaminationEnd = false;         // Deze bool houd bij als er op de "end examination" knop is geklikt, zodat je de close event kunt bypassen
};

#endif
