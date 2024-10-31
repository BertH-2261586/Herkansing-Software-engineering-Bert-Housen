#pragma once 

#include <string>

using namespace std;

class Question {
public:
    Question(string name);

    virtual string getQuestion() const = 0;
    virtual string getAnswer() const = 0;
private:
    string m_name;
};