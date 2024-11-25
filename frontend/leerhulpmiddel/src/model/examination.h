#ifndef EXAMINATION_H
#define EXAMINATION_H

#include "Questions/Question.h"

using namespace std;

class Examination
{
public:
    Examination() : m_currentQuestionIndex(0), m_totalWrongAnswers(0), m_examinationDone(false) {}

    void setQuestions(QVector<shared_ptr<Question>> questions);
    bool nextQuestion();
    shared_ptr<Question> getCurrentQuestion() const { return m_questions[m_currentQuestionIndex]; }
    void addWrongCurrentQuestion();
    int getTotalSize() const { return m_totalSize; }
    bool getExaminationStatus() const { return m_examinationDone; }
    bool finishedExamination() const;

private:
    void randomizeQuestions(QVector<shared_ptr<Question>>& questions);

    QVector<shared_ptr<Question>> m_questions;
    int m_currentQuestionIndex;

    QVector<shared_ptr<Question>> m_wrongQuestions;
    int m_totalWrongAnswers;
    int m_totalSize;

    bool m_examinationDone;
};

#endif 