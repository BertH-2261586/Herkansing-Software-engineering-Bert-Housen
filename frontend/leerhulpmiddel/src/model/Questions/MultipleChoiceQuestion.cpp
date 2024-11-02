#include "MultipleChoiceQuestion.h"


MultipleChoiceQuestion::MultipleChoiceQuestion(string name, string question, string answer) : Question(name), m_question(question), m_answer(answer){}

string MultipleChoiceQuestion::getQuestion() const
{
    return m_question;
}

string MultipleChoiceQuestion::getAnswer() const
{
    return m_answer;
}

