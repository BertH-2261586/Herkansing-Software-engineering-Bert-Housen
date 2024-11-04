#include "Question.h"

#include <string>

using namespace std;

class Flashcard : public Question {
public:
    Flashcard(string name, string question, string answer) : Question(name, QuestionType::FillIn), m_question(question), m_answer(answer) {}

    string getQuestion() const override { return m_question; }
    string getAnswer() const override { return m_answer; }
private:
    string m_question;
    string m_answer;
};