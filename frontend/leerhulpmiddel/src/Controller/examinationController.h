#ifndef EXAMINATIONCONTROLLER_H
#define EXAMINATIONCONTROLLER_H

#include "../model/manager/ExaminationManager.h"

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
    bool finishedExamination() const { return m_examinationManager->finishedExamination(); }
    int checkMultipleChoiceAnswer(QString checkedAnswers);
    QVector<int> checkFillInAnswer(QVector<QString> answerText);

signals:
    void questionLoadedView();
    void answeredWrong();
    void wrongQuestionsLoadedView();

public slots:
    void firstQuestion(shared_ptr<Question> question, int totalAmountOfQuestions);
    void questionLoadedModel(shared_ptr<Question> question);
    void wrongQuestionsLoadedModel(shared_ptr<Question> question, int totalSize);

private:
    ExaminationView* m_examinationView;
    ExaminationManager* m_examinationManager = new ExaminationManager();
    
    // All the information the view needs about the current question
    shared_ptr<Question> m_currentQuestion;
    int m_currentQuestionIndex;
    int m_totalAmountOfQuestions;
    bool m_examinationStatus;
};

#endif 