#include "Question.h"

#include <QString>

using namespace std;

class Flashcard : public Question {
public:
<<<<<<< HEAD
    Flashcard(QString name, QString question, QString answer);

    QString getQuestion() const override;
    QString getAnswer() const override;
=======
    Flashcard(string name, string question, string answer) : Question(name, QuestionType::FillIn), m_question(question), m_answer(answer) {}

    string getQuestion() const override { return m_question; }
    string getAnswer() const override { return m_answer; }
>>>>>>> main
private:
    QString m_question;
    QString m_answer;
};