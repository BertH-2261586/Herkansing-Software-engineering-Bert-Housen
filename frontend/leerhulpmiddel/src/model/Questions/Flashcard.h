#include "Question.h"

#include <QString>

using namespace std;

class Flashcard : public Question {
public:
    Flashcard(QString name, QString question, QString answer);

    QString getQuestion() const override;
    QString getAnswer() const override;
private:
    QString m_question;
    QString m_answer;
};