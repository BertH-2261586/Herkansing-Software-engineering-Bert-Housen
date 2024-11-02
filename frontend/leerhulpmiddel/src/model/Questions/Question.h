#pragma once 

#include <QString>

using namespace std;

class Question {
public:
    Question(QString name);

    virtual QString getQuestion() const = 0;
    virtual QString getAnswer() const = 0;
private:
    QString m_name;
};