#pragma once

#include <string>
#include "Question.h"


using namespace std;

class MultipleChoiceQuestion : public Question
{
public:
    MultipleChoiceQuestion(string name, string question, string answer);
    string getQuestion() const override;
    string getAnswer() const override;

private:
    string m_question;
    string m_answer;
};

