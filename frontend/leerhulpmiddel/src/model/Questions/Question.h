#pragma once 


#include <string>
#include <QString>
<<<<<<< HEAD
=======

#include "Answer.h"
#include "questionType.h"
>>>>>>> 2bff5932969b14142911bf189bda85c1729be607

using namespace std;

class Question {
public:

<<<<<<< HEAD
    virtual string getQuestion() const = 0;
    virtual string getAnswer() const = 0;

    QString getName(){return QString(m_name.data());};
private:
    string m_name;
};
=======


    Question(QString name, QuestionType questionType) : m_name(name), m_questionType(questionType) {}

    virtual QString getQuestion() const = 0;
    virtual Answer getAnswer() const = 0;
    QString getName() const { return (m_name); }
    QString questionTypeToString() const;
    static QuestionType stringToQuestionType(const QString questionType);
private:
    QString m_name;
    QuestionType m_questionType;

};
>>>>>>> 2bff5932969b14142911bf189bda85c1729be607
