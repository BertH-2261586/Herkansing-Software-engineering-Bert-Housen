#ifndef EXAMINATION_H
#define EXAMINATION_H

#include "Questions/Question.h"

using namespace std;

class Examination
{
public:
    Examination() : m_currentQuestionIndex(0) {}

    void setQuestions(QVector<shared_ptr<Question>> questions) { m_questions = questions; }
    void nextQuestion() { ++m_currentQuestionIndex; }
    shared_ptr<Question> getCurrentQuestion() const{ return m_questions[m_currentQuestionIndex]; }

private:
    QVector<shared_ptr<Question>> m_questions;
    int m_currentQuestionIndex;

};

#endif 