#include "MultipleChoiceQuestion.h"


MultipleChoiceQuestion::MultipleChoiceQuestion(QString name, QString question, QString answer) : Question(name), m_question(question), m_answer(answer){}

QString MultipleChoiceQuestion::getQuestion() const
{
    return m_question;
}

QString MultipleChoiceQuestion::getAnswer() const
{
    return m_answer;
}

