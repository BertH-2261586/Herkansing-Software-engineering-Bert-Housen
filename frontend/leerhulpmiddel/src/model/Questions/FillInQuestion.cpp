#include "FillInQuestion.h"

FillInQuestion::FillInQuestion(string name, string question, string answer) : Question(name), m_question(question), m_answer(answer){}

string FillInQuestion::getQuestion() const
{
    return m_question;
}

string FillInQuestion::getAnswer() const
{
    return m_answer;
}
