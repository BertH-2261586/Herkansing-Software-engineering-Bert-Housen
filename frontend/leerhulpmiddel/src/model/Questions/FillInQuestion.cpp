<<<<<<< HEAD
#include "FillInQuestion.h"

FillInQuestion::FillInQuestion(QString name, QString question, QString answer) : Question(name), m_question(question), m_answer(answer){}

QString FillInQuestion::getQuestion() const
{
    return m_question;
}

QString FillInQuestion::getAnswer() const
{
    return m_answer;
}
=======
#include "FillInQuestion.h"
>>>>>>> main
