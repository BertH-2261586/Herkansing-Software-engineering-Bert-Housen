#ifndef EXAMINATION_H
#define EXAMINATION_H

#include "Questions/Question.h"

using namespace std;

class Examination
{
public:
    Examination() : m_currentQuestionIndex(0), m_totalWrongAnswers(0), m_examinationDone(false), m_mostRetries(0), m_totalTimeouts(0) {}

    void setQuestions(QVector<shared_ptr<Question>> questions);
    bool nextQuestion(bool showWrong);
    shared_ptr<Question> getCurrentQuestion() const { return m_questions[m_currentQuestionIndex]; }
    void addWrongCurrentQuestion(bool timeout);
    int getTotalSize() const { return m_totalSize; }
    bool getExaminationStatus() const { return m_examinationDone; }
    bool finishedExamination() const;
    QMap<QString, QString> getExaminationData() const;

private:
    void randomizeQuestions(QVector<shared_ptr<Question>>& questions);

    QVector<shared_ptr<Question>> m_questions;
    int m_currentQuestionIndex;
    int m_amountOfQuestions;

    QVector<shared_ptr<Question>> m_wrongQuestions;

    // Examination Info
    int m_totalWrongAnswers;
    int m_totalSize;
    int m_mostRetries;
    int m_totalTimeouts;
    bool m_examinationDone;
    bool m_showWrong = true;
};

#endif 