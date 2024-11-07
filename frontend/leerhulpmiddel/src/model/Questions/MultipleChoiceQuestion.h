#pragma once

#include <QString>
#include "Question.h"


using namespace std;

class MultipleChoiceQuestion : public Question
{
public:
<<<<<<< HEAD
    MultipleChoiceQuestion(QString name, QString question, QString answer);
    QString getQuestion() const override;
    QString getAnswer() const override;
=======
    MultipleChoiceQuestion(string name, string question, string answer) 
        : Question(name, QuestionType::MultipleChoice), m_question(question), m_answer(answer) {}

    string getQuestion() const override { return m_question; }
    string getAnswer() const override { return m_answer; }
>>>>>>> main

private:
    QString m_question;
    QString m_answer;
};

