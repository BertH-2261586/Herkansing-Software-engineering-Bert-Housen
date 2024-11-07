#pragma once 


#include <string>
#include <QString>

#include "../../questionType.h"

using namespace std;

class Question {
public:



    Question(QString name, QuestionType questionType) : m_name(name), m_questionType(questionType) {}

    virtual QString getQuestion() const = 0;
    virtual QString getAnswer() const = 0;
    QString getName() const { return (m_name); }
    QString questionTypeToString() const;
    static QuestionType stringToQuestionType(const QString questionType);
private:
    QString m_name;
    QuestionType m_questionType;

};