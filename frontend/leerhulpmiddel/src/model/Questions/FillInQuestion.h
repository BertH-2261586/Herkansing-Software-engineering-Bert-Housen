#include "Question.h"

#include <string>

using namespace std;

class FillInQuestion : public Question {
public:
    FillInQuestion(string name, string question, string answer);

    string getQuestion() const override;
    string getAnswer() const override;
private:
    string m_question;
    string m_answer;
};