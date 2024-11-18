
#ifndef QUESTIONCONTROLLER_H
#define QUESTIONCONTROLLER_H

#include "../model/Questions/Question.h"


class QuestionController
{
public:
    QuestionController(Question* question);


    void changeName(QString newName);


private:
    Question* m_question;

};

#endif // QUESTIONCONTROLLER_H
