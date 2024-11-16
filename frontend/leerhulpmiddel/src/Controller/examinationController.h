#ifndef EXAMINATIONCONTROLLER_H
#define EXAMINATIONCONTROLLER_H

#include "../model/manager/ExaminationManager.h"

class ExaminationView;

class ExaminationController : public QObject
{
    Q_OBJECT

public:
    ExaminationController(ExaminationView* examinationView);

    int getTotalAmountOfQuestions() const { return m_totalAmountOfQuestions; }
    int getCurrentQuestionNumber() const { return m_currentQuestionIndex; }
    QuestionType getCurrentQuestionType() const { return m_currentQuestion->getQuestionType(); }
    const Question* getQuestion() const { return m_currentQuestion.get(); }

signals:
    void questionLoadedView();

public slots:
    void firstQuestion(shared_ptr<Question> question, int totalAmountOfQuestions);
    void questionLoadedModel(shared_ptr<Question> question);

private:
    ExaminationView* m_examinationView;
    ExaminationManager* m_examinationManager = new ExaminationManager();
    
    shared_ptr<Question> m_currentQuestion;
    int m_currentQuestionIndex;
    int m_totalAmountOfQuestions;

};

#endif 