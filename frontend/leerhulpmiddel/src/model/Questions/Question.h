#pragma once 
#include <QString>

#include "Answer.h"
#include "questionType.h"

class Question {
public:
    Question(QString name, QuestionType questionType) : m_name(name), m_questionType(questionType) {}

    virtual QString getQuestion() const = 0;
    virtual Answer getAnswer() const = 0;

    // Getters
    QString getName() const { return (m_name); }
    QuestionType getQuestionType() const { return m_questionType; }

    // Convertors of the questionType
    QString questionTypeToString() const;
    static QuestionType stringToQuestionType(const QString questionType);

private:
    QString m_name;
    QuestionType m_questionType;

};
