#pragma once 

<<<<<<< HEAD
#include <QString>
=======
#include <string>
#include <QString>

#include "../../questionType.h"
>>>>>>> main

using namespace std;

class Question {
public:
<<<<<<< HEAD
    Question(QString name);

    virtual QString getQuestion() const = 0;
    virtual QString getAnswer() const = 0;
private:
    QString m_name;
=======
    Question(string name, QuestionType questionType) : m_name(name), m_questionType(questionType) {}

    virtual string getQuestion() const = 0;
    virtual string getAnswer() const = 0;
    string getName() const { return m_name; }
    string questionTypeToString() const;
    static QuestionType stringToQuestionType(const QString questionType);
private:
    string m_name;
    QuestionType m_questionType;
>>>>>>> main
};