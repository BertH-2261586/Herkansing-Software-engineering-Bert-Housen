#ifndef EXAMINATIONMANAGER_H
#define EXAMINATIONMANAGER_H

#include "../examination.h"

class ExaminationManager : public QObject
{
    Q_OBJECT

public:
    ExaminationManager() : m_examination(Examination()) {}
    bool finishedExamination() const { return m_examination.finishedExamination(); }

signals:
    void firstQuestion(shared_ptr<Question> question, int totalAmountOfQuestions);
    void questionLoadedModel(shared_ptr<Question> question);
    void wrongQuestionsLoadedModel(shared_ptr<Question> question, int newSize);

public slots:
    void examinationStarted(const QString questionSetPath);
    void nextQuestion();
    void answeredWrong() { m_examination.addWrongCurrentQuestion(); }

private:
    Examination m_examination;

};

#endif 