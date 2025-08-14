#ifndef EXAMINATIONCONTROLLER_H
#define EXAMINATIONCONTROLLER_H

#include "../model/manager/examinationManager.h"

class ExaminationView;

class ExaminationController : public QObject
{
    Q_OBJECT

public:
    ExaminationController(ExaminationView* examinationView);
    ~ExaminationController() { delete m_examinationManager; }

    // Getters
    int getTotalAmountOfQuestions() const { return m_totalAmountOfQuestions; }
    int getCurrentQuestionNumber() const { return m_currentQuestionIndex; }
    QuestionType getCurrentQuestionType() const { return m_currentQuestion->getQuestionType(); }
    const Question* getQuestion() const { return m_currentQuestion.get(); }
    bool finishedExamination() {
        emit ended();
        return m_examinationManager->finishedExamination(); }
    bool showTimer() const { return m_showTimer; }

    //int getScore(){return m_examinationManager->getExaminationData()}

    void setShowTimer(bool showTimer) { m_showTimer = showTimer; }

    // Answer checker
    int checkMultipleChoiceAnswer(QString checkedAnswers, bool timeout);
    QVector<int> checkFillInAnswer(QVector<QString> answerText, bool timeout);
    void checkFlashCardAnswer(bool repeat, bool timeout);

signals:
    void questionLoadedView();
    void answeredWrong(bool timeout);
    void wrongQuestionsLoadedView();

    void ended();

public slots:
    void firstQuestion(shared_ptr<Question> question, int totalAmountOfQuestions);
    void questionLoadedModel(shared_ptr<Question> question);
    void wrongQuestionsLoadedModel(shared_ptr<Question> question, int totalSize);

private:
    ExaminationView* m_examinationView;
    ExaminationManager* m_examinationManager = new ExaminationManager();
    
    bool m_showTimer;

    // All the information the view needs about the current question
    shared_ptr<Question> m_currentQuestion;
    int m_currentQuestionIndex;
    int m_totalAmountOfQuestions;
    bool m_examinationStatus;
};

#endif 
