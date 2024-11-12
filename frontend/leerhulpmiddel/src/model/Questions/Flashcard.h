#include "Question.h"
#include "Answer.h"

#include <QString>

using namespace std;

class Flashcard : public Question {
public:
    Flashcard(QString name, QString question, Answer answer) : Question(name, QuestionType::FillIn), m_question(question), m_answer(answer) {}

    QString getQuestion() const override { return m_question; }
    Answer getAnswer() const override { return m_answer; }

private:
    QString m_question;
    Answer m_answer;
};