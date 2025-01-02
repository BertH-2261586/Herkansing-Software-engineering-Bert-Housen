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
#include "scoreCardExaminationView.h"

class ExaminationController;

class ExaminationView : public QWidget {
    Q_OBJECT

public:
    explicit ExaminationView(QWidget* parent = nullptr, bool questionSelectOnly = true);
    void startExamination(QString path, QTime timeLimit = QTime(-1, -1, -1));
    void startExamination(QList<QString> path, QTime timeLimit = QTime(-1, -1, -1));

signals:
    void examinationStarted(const QString questionSetPath);
    void examinationStartedL(const QList<QString> questionSetPath);
    void nextQuestion(bool showWrong);
    void getExaminationData();

public slots:
    void questionLoadedView();
    void receiveExaminationData(QMap<QString, QString> data);
    void closeWindow() { this->close(); }

private slots:
    void nextQuestionView();
    void showAnswer(bool timeout = false);
    void closeEvent(QCloseEvent* event) override; // Override close event

private:
    void next();
    bool m_questionSelectOnly;

    void setCurrentQuestionView();
    void clearPreviousQuestionView();

    // GUI Setup
    void setupAmountQuestionsAnswered();
    void setupTimer();
    void setupSubmitButton();
    void setupCloseButton();
    void setupNextQuestionButton();
    void setupEndExaminationButton();
    void initializeLayouts();

    void hideAllWidgets(QLayout* layout);

    QVBoxLayout* m_mainLayout;
    QHBoxLayout* m_questionInfoLayout;
    QLabel* m_amountOfQuestionsAnswered;
    QPushButton* m_submitButton;
    QPushButton* m_closeButton;
    QPushButton* m_nextQuestionButton;
    QPushButton* m_endExaminationButton;

    // Sub views
    CountdownTimer* m_timePerQuestion;
    MultipleChoiceView m_multipleChoiceView = MultipleChoiceView();
    FlashcardView m_flashcardView = FlashcardView();
    FillInView m_fillInView = FillInView();
    scoreCardExaminationView* m_scoreCard;

    // Controller
    ExaminationController* m_examinationController;

    bool m_closeFromExaminationEnd = false;         // Deze bool houd bij als er op de "end examination" knop is geklikt, zodat je de close event kunt bypassen
};

#endif
