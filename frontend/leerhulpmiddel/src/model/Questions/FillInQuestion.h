#include "Question.h"

#include <QString>

using namespace std;

class FillInQuestion : public Question {
public:
    FillInQuestion(QString name, QString question, QString answer);

    QString getQuestion() const override;
    QString getAnswer() const override;
private:
    QString m_question;
    QString m_answer;
};