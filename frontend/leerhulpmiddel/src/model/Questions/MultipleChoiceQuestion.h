#pragma once

#include <string>
#include "Question.h"


using namespace std;

class MultipleChoiceQuestion : public Question
{
public:
    MultipleChoiceQuestion(string name, string question, string answer) 
        : Question(name, QuestionType::MultipleChoice), m_question(question), m_answer(answer) {}

    string getQuestion() const override { return m_question; }
    string getAnswer() const override { return m_answer; }

private:
    string m_question;
    string m_answer;
};

