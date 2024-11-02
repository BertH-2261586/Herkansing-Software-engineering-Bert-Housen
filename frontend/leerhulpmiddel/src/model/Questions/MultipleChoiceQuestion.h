#pragma once

#include <QString>
#include "Question.h"


using namespace std;

class MultipleChoiceQuestion : public Question
{
public:
    MultipleChoiceQuestion(QString name, QString question, QString answer);
    QString getQuestion() const override;
    QString getAnswer() const override;

private:
    QString m_question;
    QString m_answer;
};

