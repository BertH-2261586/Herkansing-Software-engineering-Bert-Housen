#pragma once 


#include <string>
#include <QString>

#include "Answer.h"
#include "questionType.h"

using namespace std;

class Question : public QObject {
    Q_OBJECT

public:

    Question(QString name, QuestionType questionType) : m_name(name), m_questionType(questionType) {}

    virtual QString getQuestion() const = 0;
    virtual Answer getAnswer() const = 0;
    QString getName() const { return (m_name); }
    QString questionTypeToString() const;
    static QuestionType stringToQuestionType(const QString questionType);

    void setName(QString newName);

signals:
    void changed();

private:
    QString m_name;
    QuestionType m_questionType;

};
