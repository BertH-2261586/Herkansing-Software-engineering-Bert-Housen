#pragma once 

#include <string>
#include <QString>

#include "../../questionType.h"

using namespace std;

class Question {
public:
    Question(string name, QuestionType questionType) : m_name(name), m_questionType(questionType) {}

    virtual string getQuestion() const = 0;
    virtual string getAnswer() const = 0;
    string getName() const { return m_name; }
    string questionTypeToString() const;
    static QuestionType stringToQuestionType(const QString questionType);
private:
    string m_name;
    QuestionType m_questionType;
};