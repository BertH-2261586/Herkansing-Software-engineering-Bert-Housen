#pragma once 

#include <string>
#include <QString>

using namespace std;

class Question {
public:
    Question(string name);

    virtual string getQuestion() const = 0;
    virtual string getAnswer() const = 0;

    QString getName(){return QString(m_name.data());};
private:
    string m_name;
};
