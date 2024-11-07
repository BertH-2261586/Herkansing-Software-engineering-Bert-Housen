#pragma once

#include <QString>
#include "Question.h"


using namespace std;

class MultipleChoiceQuestion : public Question
{
public:

    MultipleChoiceQuestion(QString name, QString question, QString answer) : Question(name, QuestionType::MultipleChoice), m_question(question), m_answer(answer) {}

    QString getQuestion() const override { return m_question; }
    QString getAnswer() const override { return m_answer; }

private:
    QString m_question;
    QString m_answer;
};

